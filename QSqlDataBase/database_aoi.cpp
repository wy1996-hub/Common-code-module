#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlerror>
#include <QSqlQuery>
#include <QRegExp>
#include <QStringList>
#include <QDateTime>
#include <QTextCodec>
#include <QBitArray>

#include <stdio.h>
#include <time.h>
#include <ctime>

#include "database_aoi.h"

JS_DATABASE::JS_DATABASE(QString connection_name, QString host_IP, QString user_name, QString passward, QString database_name)
{
	// 检测算子对应表名的索引map初始化
	defect_index_map_init();

	// 检测算子对应输出值数量map初始化
	value_num_map_init();
	// 数据库连接配置信息初始化
	this->host_IP = host_IP;
	this->user_name = user_name;
	this->passward = passward;
	this->database_name = database_name;

	// 添加MYSQL数据库驱动
	this->db = QSqlDatabase::addDatabase("QMYSQL", connection_name);
	// 查看数据库驱动是否有效
	this->is_driver_valid = this->db.isValid();
}

// 析构函数
JS_DATABASE::~JS_DATABASE()
{
	if (this->db.isOpen())
	{
		this->db.close();
	}
}

// 建立数据库连接
bool JS_DATABASE::connect()
{
	// 判断数据库驱动是否有效
	if (!is_driver_valid)
	{
		qDebug() << "Connect failed, the QMYSQL driver is invalid!";
		this->is_connected = false;
		return false;
	}

	// 配置连接信息
	this->db.setHostName(this->host_IP);
	this->db.setUserName(this->user_name);
	this->db.setPassword(this->passward);
	this->db.setDatabaseName(this->database_name);

	// 尝试建立连接
	if (!this->db.open())
	{
		qDebug() << this->db.lastError().text();
		this->is_connected = false;
		return false;
	}

	this->is_connected = true;
	return true;
}

// 插入结果数据
bool JS_DATABASE::insert_detect_result(DETECT_INFO_INSERT& result_info)
{
	// 判断数据库对象是否已经连接
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 1.执行操作前对输入值的有效性进行判断

	// 1.1 ng_type与ng_triger的长度应该与map一致
	if (result_info.ng_type.size() != defect_index_map.size() || result_info.ng_triger.size() != defect_index_map.size())
	{
		qDebug() << "Insert failed! The length of ng_type or ng_triger is wrong!";
		return false;
	}

	// 1.2 ng_type与ng_triger应该只包含0和1
	if (result_info.ng_type.contains(QRegExp("[^0,1]")) || result_info.ng_triger.contains(QRegExp("[^0,1]")))
	{
		qDebug() << "Insert failed! The content of ng_type or ng_triger should be only 0 or 1!";
		return false;
	}

	// 1.3 检查时间戳的格式是否正确
	QRegExp re_timestamp("\\s*\\d\\d\\d\\d[/-]?\\d\\d[/-]?\\d\\d[ ]*\\d\\d[:]?\\d\\d[:]?\\d\\d\\s*");
	if (re_timestamp.indexIn(result_info.timestamp) == -1)
	{
		qDebug() << "Insert failed! The format of timestamp is wrong!";
		return false;
	}

	// 1.4 检查待检测的NG数量与存储输出值容器的大小是否一致
	int ng_num_needcheck = 0;
	std::vector<int> vec_ng_needcheck_index;
	for (int i = 0; i < result_info.ng_type.size(); i++)
	{
		if (result_info.ng_type[i] == "1")
		{
			ng_num_needcheck += 1;
			vec_ng_needcheck_index.push_back(i);
		}
	}
	if (ng_num_needcheck != result_info.vec_values.size())
	{
		qDebug() << "Insert failed! The num of NG that need check doesn't match the size of vec_values!";
		return false;
	}


	// 2.在表production_result中插入该元件的测试结果信息
	QSqlQuery query(this->db);
	QString sql;
	bool isOK;

	// 2.1 拼接sql语句
	sql = "insert into `production_result` values (default, ";
	sql += "'" + result_info.board_name + "', ";
	sql += "'" + result_info.ele_index + "', ";
	sql += "'" + result_info.timestamp + "', ";
	sql += "'" + result_info.ele_name + "', ";
	sql += result_info.is_ng ? "1, " : "0, ";
	sql += "'" + result_info.ng_type + "', ";
	sql += "'" + result_info.ng_triger + "', ";
	sql += "'" + result_info.pic_path + "', ";
	sql += QString::number(result_info.loc_x) + ", ";
	sql += QString::number(result_info.loc_y) + ", ";
	sql += QString::number(result_info.width) + ", ";
	sql += QString::number(result_info.height) + ")";

	// 2.2 执行sql语句向表production_result中插入
	isOK = query.exec(sql);
	if (!isOK)
	{
		qDebug() << "Insert into `production_result` failed! The reason is" << this->db.lastError().text();
		return false;
	}

	// 3.分析result_info.ng_type信息，将该元件需检测的NG信息输出到对应的表中

	// 3.1 根据先前步骤获取的字串中1的位置作为索引号从defect_index_map获取对应需操作的表名
	std::vector<QString> vec_tablename;
	for (int i = 0; i < ng_num_needcheck; i++)
	{
		vec_tablename.push_back(defect_index_map[vec_ng_needcheck_index[i]]);
	}

	// 3.2 生成操作每张表需要的sql语句
	std::vector<QString> vec_sql;
	for (int i = 0; i < ng_num_needcheck; i++)
	{
		QString sql_tmp = "insert into `" + vec_tablename[i] + "` values (default, ";
		sql_tmp += "'" + result_info.board_name + "', ";
		sql_tmp += "'" + result_info.ele_index + "', ";
		sql_tmp += "'" + result_info.timestamp + "', ";
		sql_tmp += result_info.ng_triger[vec_ng_needcheck_index[i]] == "1" ? "1, " : "0, ";

		// 判断该缺陷的输出信息是否为多个（如果有多个则按','分隔，因此直接判断有无字符','确认是否为多输出值）
		if (result_info.vec_values[i].contains(QRegExp("[,]")))
		{
			QStringList value_list = result_info.vec_values[i].split(",");
			for (int i = 0; i < value_list.size();i++)
			{
				if (i == value_list.size() - 1)
				{
					sql_tmp += "'" + value_list[i] + "') ";
					break;
				}
				sql_tmp += "'" + value_list[i] + "', ";
			}
			vec_sql.push_back(sql_tmp);
		}
		// 如果输出信息只有1个
		else
		{
			sql_tmp += "'" + result_info.vec_values[i] + "') ";
			vec_sql.push_back(sql_tmp);
		}
	}

	// 3.3执行sql语句将每个缺陷检测算子的详细结果存储到对应的表中
	for (int i = 0; i < vec_sql.size(); i++)
	{
		// qDebug() << vec_sql[i];  // 输出生成的每一条sql语句（调试用）
		isOK = query.exec(vec_sql[i]);
		if (!isOK)
		{
			qDebug() << vec_sql[i] <<  "failed! The reason is" << this->db.lastError().text();
			return false;
		}
	}

	return true;
}

// 查询具体时间戳下，某块板的某个元件检测结果信息
bool JS_DATABASE::inquire_detect_result(QString timestamp, QString board, QString ele_index, DETECT_INFO_REQUIRE& result)
{
	// 判断数据库对象是否已经连接
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 1.检查时间戳的格式是否正确
	QRegExp re_timestamp("\\s*\\d\\d\\d\\d[/-]?\\d\\d[/-]?\\d\\d[ ]*\\d\\d[:]?\\d\\d[:]?\\d\\d\\s*");
	if (re_timestamp.indexIn(timestamp) == -1)
	{
		qDebug() << "Insert failed! The format of timestamp is wrong!";
		return false;
	}

	// 2.从主表中查询该元件的检测结果
	QSqlQuery query(this->db);
	QString sql;
	bool isOK;

	// 2.1 拼接并执行sql语句
	sql = "select * from `production_result` where ";
	sql += "timestamp = '" + timestamp + "' ";
	sql += "and board = '" + board + "' ";
	sql += "and ele_index = '" + ele_index + "'";
	isOK = query.exec(sql);
	qDebug() << sql;
	if (!isOK)
	{
		qDebug() << "Inquire option on `production_result` execute failed! The reason is" << this->db.lastError().text();
		return false;
	}

	// 2.2 获取结果集，并填写从主表中获取的信息
	if (!query.next())
	{
		qDebug() << "The inquire result in `production_result` is NULL!";
		return false;
	}
	// 读取数据库中的图片路径（QString转为std::string）
	QString path = query.value("pic_path").toString();
	QTextCodec* code = QTextCodec::codecForName("gb18030");
	std::string std_path = code->fromUnicode(path).data();
	result.pic_path = std_path;
	result.ele_name = query.value("ele_name").toString();
	result.b_isNG = query.value("is_ng").toInt() == 1 ? 1 : 0;
	result.ng_triger = query.value("ng_triger").toString();
	result.loc_x = query.value("loc_x").toInt();
	result.loc_y = query.value("loc_y").toInt();
	result.width = query.value("width").toInt();
	result.height = query.value("height").toInt();


	// 3.根据主表中查到的ng_type信息，进一步从各分表中查找具体的算子输出信息

	// 3.1 获取元件应该检测的算子索引号，进一步获取应该查询的表名以及对应输出的value数量
	QString ng_type = query.value("ng_type").toString();
	std::vector<int> vec_ng_needcheck;			// 原件应该进行检测的算子索引号
	std::vector<QString> vec_table_needinquire;	// 原件应该进行检测的算子索引号对应的表名
	std::vector<int> vec_value_num;				// 原件应该进行检测的算子索引号对应的输出value数量
	for (int i = 0; i < ng_type.size(); i++)	// 遍历解析ng_type字串
	{
		if (ng_type[i] == '1')
		{
			vec_ng_needcheck.push_back(i);
			vec_table_needinquire.push_back(defect_index_map[i]);
			vec_value_num.push_back(value_num_map[i]);
		}
	}
	
	// 3.2 根据算子索引号对应的表名生成并执行用于查询不同表的sql语句，并根据应该输出的value数量配置输出信息的格式
	for (int i = 0; i < vec_table_needinquire.size(); i++)
	{
		// 生成执行分表查询任务的sql语句
		sql = "select * from ";
		sql += "`" + vec_table_needinquire[i] + "` where ";
		sql += "timestamp = '" + timestamp + "' ";
		sql += "and board = '" + board + "' ";
		sql += "and ele_index = '" + ele_index + "'";
		isOK = query.exec(sql);

		// 查询语句执行失败或查到的结果集为空，将该算子对应分表的输出信息置空""
		if (!isOK)
		{
			qDebug() << "Inquire option on `" + vec_table_needinquire[i] + "` execute failed! The reason is" << this->db.lastError().text();
			result.detect_value_map[vec_ng_needcheck[i]] = "";
			continue;
		}
		if (!query.next())
		{
			qDebug() << "The result inquired in `" + vec_table_needinquire[i] + "` is NULL!";
			result.detect_value_map[vec_ng_needcheck[i]] = "";
			continue;
		}

		// 判断value数量，如有多个则需要将多个value信息用","分隔拼接成一个字串填入输出信息中
		if (vec_value_num[i] == 1)
		{
			result.detect_value_map[vec_ng_needcheck[i]] = query.value(5).toString();
		} 
		else
		{
			QString multi_values;
			for (int j = 0; j < vec_value_num[i]; j++)
			{
				multi_values += query.value(5 + j).toString();
				if (j != vec_value_num[i] - 1)
				{
					multi_values += ",";
				}
			}
			result.detect_value_map[vec_ng_needcheck[i]] = multi_values;
		}
	}

	return true;
}

// 清空指定的数据表
bool JS_DATABASE::truncate_table(QString table_name)
{
	// 判断数据库对象是否已经连接
	if (!is_connected)
	{
		qDebug() << "The database is disconnected! Please connect first!";
		return false;
	}

	// 执行清空数据表的操作
	QSqlQuery query(this->db);
	QString sql = "truncate table `" + table_name + "`";
	bool isOK = query.exec(sql);
	if (!isOK)
	{
		qDebug() << "truncate_table `" << table_name << "` failed! The reason is" << this->db.lastError().text();
		return false;
	}
	return true;
}

// 获取时间戳函数 格式示例：2020/08/29 16:59:28
QString JS_DATABASE::get_timestamp_now()
{
	time_t curtime = time(NULL);
	tm *ptm = localtime(&curtime);
	char buf[20];
	sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	QString time(buf);

	return time;
}

// 检测算子对应表名的索引map初始化
void JS_DATABASE::defect_index_map_init()
{
	this->defect_index_map[0] = "op_ending_extrude";	// 末端突出
	this->defect_index_map[1] = "op_wrong_element";		// 错料
	this->defect_index_map[2] = "op_backwards";			// 装反
	this->defect_index_map[3] = "op_ending_overlap";	// 末端重叠
	this->defect_index_map[4] = "op_test";
	this->defect_index_map[5] = "op5";
	this->defect_index_map[6] = "op6";
	this->defect_index_map[7] = "op7";
	this->defect_index_map[8] = "op8";
	this->defect_index_map[9] = "op9";
	this->defect_index_map[10] = "op10";
	this->defect_index_map[11] = "op11";
	this->defect_index_map[12] = "op12";
	this->defect_index_map[13] = "op13";
	this->defect_index_map[14] = "op14";
	this->defect_index_map[15] = "op15";
	this->defect_index_map[16] = "op16";
	this->defect_index_map[17] = "op17";
	this->defect_index_map[18] = "op18";
	this->defect_index_map[19] = "op19";
	this->defect_index_map[20] = "op20";
	this->defect_index_map[21] = "op21";
	this->defect_index_map[22] = "op22";
	this->defect_index_map[23] = "op23";
}

// 检测算子对应输出值数量的map初始化
void JS_DATABASE::value_num_map_init()
{
	this->value_num_map[0] = 1;
	this->value_num_map[1] = 1;
	this->value_num_map[2] = 1;
	this->value_num_map[3] = 1;
	this->value_num_map[4] = 2;
	this->value_num_map[5] = 1;
	this->value_num_map[6] = 1;
	this->value_num_map[7] = 1;
	this->value_num_map[8] = 1;
	this->value_num_map[9] = 1;
	this->value_num_map[10] = 1;
	this->value_num_map[11] = 1;
	this->value_num_map[12] = 1;
	this->value_num_map[13] = 1;
	this->value_num_map[14] = 1;
	this->value_num_map[15] = 1;
	this->value_num_map[16] = 1;
	this->value_num_map[17] = 1;
	this->value_num_map[18] = 1;
	this->value_num_map[19] = 1;
	this->value_num_map[20] = 1;
	this->value_num_map[21] = 1;
	this->value_num_map[22] = 1;
	this->value_num_map[23] = 1;
}

