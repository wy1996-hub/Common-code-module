#include "LXMysql.h"

#include <iostream>
#include <mysql.h>

using namespace std;

//初始化MySQL API
bool LXMysql::Init() 
{
	//初始化前先执行一次清理资源操作，防止重连时引起的资源重复申请
	Close();
	//新创建一个mysql对象
	mysql = mysql_init(0);
	//mysql_init(mysql);
	if (!mysql)
	{
		cerr << "LXMysql::Init() failed!" << endl;
		return false;
	}

	//cout << "LXMysql::Init() success!" << endl;
	return true;
}

//清理MySQL占用的所有资源
void LXMysql::Close()
{
	//先清理一下已存在的结果集
	FreeResult();
	//若mysql指针不为空，则执行清理
	if (mysql)
	{
		mysql_close(mysql);		//关闭整个mysql，且清理掉mysql所占用的空间
		mysql = NULL;			//防止再次关闭时出错
	}
	//cout << "LXMysql::Close() success!" << endl;
}

//mysql参数设定 Connect之前调用
bool LXMysql::Options(LX_OPT opt, const void* arg)
{
	//首先检查一下mysql是否为空
	if (!mysql)
	{
		cerr << "LXMysql::Options failed! mysql is NULL!" << endl;
		return false;
	}
	//调用mysql_options进行参数设定，LX_OPT为自定义枚举结构，需要与MySQL内部定义的mysql_option枚举结构保持一致
	int re = mysql_options(mysql, (mysql_option)opt, arg);  //LX_OPT枚举结构需要强转为mysql_option枚举结构
	if (re != 0)
	{
		cerr << "LXMysql::Options() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	return true;
}

//数据库连接（不考虑线程安全） 设置flag标志位可以支持一次执行多条sql语句
bool LXMysql::Connect(const char* host, const char* user, const char* pass, const char* db, unsigned short port, unsigned long flag)
{
	//在实际连接前检测mysql是否为空，若为空则执行一次初始化操作，确保存在MYSQL对象才能进行连接操作
	if (!mysql)
	{
		if (!Init())
		{
			cerr << "LXMysql::Connect() failed! MYSQL struct can not init!" << endl;
			return false;
		}
	}
	//完成数据库连接操作
	if (!mysql_real_connect(mysql, host, user, pass, db, port, 0, flag))
	{
		cerr << "LXMysql::Connect() failed! The reason is: " << endl << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::Connect() '" << host << "' success!" << endl;
	return true;
}

//执行sql语句  如果采用默认sqllen=0 则通过strlen获取字符长度（内部用mysql_real_query执行sql语句）
bool LXMysql::Query(const char* sql, unsigned long sqllen)
{
	//首先检查一下mysql是否为空
	if (!mysql)
	{
		cerr << "LXMysql::Query failed! mysql is NULL!" << endl;
		return false;
	}
	//判断传入的sql语句是否为空
	if (!sql)
	{
		cerr << "LXMysql::Query failed! sql is NULL！" << endl;
		return false;
	}
	//判断用户是否选择传入sql语句的长度
	if (sqllen <= 0)  //默认不传入sql长度时，sqllen为0，内部通过strlen函数获取
		sqllen = (unsigned long)strlen(sql);
	//再次判断sql语句的长度是否正常
	if (sqllen <= 0)  //sql长度值仍<=0，则为异常状态
	{
		cerr << "LXMysql::Query failed! sql is empty or wrong format!" << endl;
		return false;
	}
	//执行该sql语句
	int re = mysql_real_query(mysql, sql, sqllen);
	if (re != 0)
	{
		cerr << "LXMysql::Query() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::Query() success!" << endl;
	return true;
}

//设置连接超时时间（秒/s）
bool LXMysql::SetConnectTimeout(int sec)
{
	//cout << "LXMysql::SetConnectTimeout() success!" << endl;
	return Options(LX_OPT_CONNECT_TIMEOUT, &sec);
}

//设置自动重连，mysql默认配置是flase
bool LXMysql::SetReconnect(bool ifReconnect)
{
	//cout << "LXMysql::SetReconnect() success!" << endl;
	return Options(LX_OPT_RECONNECT, &ifReconnect);
}

//结果集获取方式1 一次性存储所有结果集
bool  LXMysql::StoreResult()
{
	//首先检查一下mysql是否为空
	if (!mysql)
	{
		cerr << "LXMysql::StoreResult failed! mysql is NULL!" << endl;
		return false;
	}
	//获取前先清理一次结果集
	FreeResult();
	//获取结果集
	this->result = mysql_store_result(mysql);
	//判断获取后结果集中是否存有内容
	if (!this->result)
	{
		cerr << "LXMysql::StoreResult() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::StoreResult() success!" << endl;
	return true;
}

//结果集获取方式2 标识开始获取结果集，通过Fetch接口实际获取结果集
bool LXMysql::UseResult()
{
	//首先检查一下mysql是否为空
	if (!mysql)
	{
		cerr << "LXMysql::UseResult failed! mysql is NULL!" << endl;
		return false;
	}
	//获取前先清理一次结果集
	FreeResult();
	//标识开始获取结果集
	result = mysql_use_result(mysql);
	if (!result)
	{
		cerr << "LXMysql::UseResult() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::UseResult() success!" << endl;
	return true;
}

//清理结果集，释放结果集占用的空间
void LXMysql::FreeResult()
{
	//判断结果集有内容时执行清理操作
	if (result)
	{
		mysql_free_result(result);
		result = NULL;
	}
}

//从结果集中获取一行数据,数据存储在自定义数据类型LXData的容器数组中
std::vector<LXData> LXMysql::FetchRow()
{
	//定义存储该行数据中每个字段内容的LXData容器
	std::vector<LXData> resultRow;
	//判断当前对象的结果集中是否为空
	if (!result)
	{
		return resultRow;  //若结果集为空，则返回一个空的std::vector<LXData>对象
	}
	//从结果集中取出一行数据
	MYSQL_ROW row = mysql_fetch_row(result);
	if (!row)
	{
		return resultRow;  //若结果集中当前行无内容，则返回一个空的std::vector<LXData>对象
	}
	//计算该结果集的字段数量
	int colNum = mysql_num_fields(result);
	//获取结果集中该行数据每个字段数据的大小
	unsigned long* lens = mysql_fetch_lengths(result);
	//遍历获取到的一行数据中每个字段的内容，放入std::vector<LXData>数组中
	for (int i = 0; i < colNum; i++)
	{
		LXData tempData;
		tempData.data = row[i];
		tempData.size = lens[i];
		//调用mysql的API获取当前行结果集中该列的信息
		auto field = mysql_fetch_field_direct(result, i);
		//将mysql的枚举标识强转为自定义的FIELD_TYPE枚举类型
		tempData.type = (FIELD_TYPE)field->type;
		resultRow.push_back(tempData);
	}

	return resultRow;
}

//生成insert sql语句
std::string LXMysql::GetInsertSql(XDATA kv, std::string table)
{
	string sql = "";
	//判断接口参数是否为空
	if (kv.empty() || table.empty()) 
		return sql;
	//组合sql语句 示例：insert into `t_video` (`name`,`size`) values ('name1','1024')
	sql = "insert into `" + table + "` (";
	string keys = "";
	string vals = "";
	//利用迭代器遍历kv组合各字段名与对应的键值
	for (XDATA::iterator ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		//组合字段名
		keys += "`";
		keys += ptr->first;
		keys += "`,";
		//组合字段名对应的值
		vals += "'";
		vals += ptr->second.data;
		vals += "',";
	}
	//去除多余的逗号
	keys[keys.size() - 1] = ' ';
	vals[vals.size() - 1] = ' ';
	//继续组合sql语句
	sql += keys;
	sql += ") values (";
	sql += vals;
	sql += ")";

	return sql;
}

//插入非二进制数据（只插入简单字串）
bool LXMysql::Insert(XDATA kv, std::string table)
{
	//首先检查一下mysql是否为空
	if (!mysql)
	{
		cerr << "LXMysql::Insert failed! mysql is NULL!" << endl;
		return false;
	}
	//利用内部接口GetInsertSql生成sql语句
	string sql = GetInsertSql(kv, table);
	if (sql.empty())
	{
		cerr << "LXMysql::Insert failed! sql generated is empty!" << endl;
		return false;
	}
	//执行sql语句
	if (!Query(sql.c_str()))
	{
		cerr << "LXMysql::Insert failed! sql execute wrong!" << endl;
		return false;
	}
	//根据该语句影响的行数判断是否是insert语句执行成功（避免其他类型sql成功执行引入干扰）
	int affctedRows = mysql_affected_rows(mysql);
	if (affctedRows <= 0)
	{
		cerr << "LXMysql::Insert failed! mysql_affected_rows <= 0!" << endl;
		return false;
	}

	return true;
}

//插入二进制数据
bool LXMysql::InsertBin(XDATA kv, std::string table)
{
	//首先生成mysql_stmt_prepare用的sql语句
	string sql = "";
	//判断接口参数是否为空，mysql是否为空
	if (kv.empty() || table.empty() || !mysql)
		return false;
	//组包mysql_stmt_prepare用的sql语句 示例：insert into `t_video` (`name`,`size`) values (?,?)
	sql = "insert into `" + table + "` (";
	string keys = "";
	string vals = "";
	//定义用于绑定字段的MYSQL_BIND结构体数组（可以绑定的字段数量上限初步设定为256个）
	MYSQL_BIND bind[256] = { 0 };
	int i = 0;
	//利用迭代器遍历kv组合各字段名与对应的键值
	for (XDATA::iterator ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		//组合字段名
		keys += "`";
		keys += ptr->first;
		keys += "`,";
		//组合字段名对应的?，后面绑定响应的内容
		vals += "?,";
		bind[i].buffer = (char*)ptr->second.data;
		bind[i].buffer_length = ptr->second.size;
		bind[i].buffer_type = (enum_field_types)ptr->second.type;
		i++;
	}
	//去除多余的逗号
	keys[keys.size() - 1] = ' ';
	vals[vals.size() - 1] = ' ';
	//继续组合sql语句
	sql += keys;
	sql += ") values (";
	sql += vals;
	sql += ")";

	//创建一个stmt对象
	MYSQL_STMT* stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		cerr << "LXMysql::InsertBin() mysql_stmt_init failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}
	//预处理sql语句
	if (mysql_stmt_prepare(stmt, sql.c_str(), sql.length()) != 0)
	{
		//清理stmt申请的内存资源
		mysql_stmt_close(stmt);
		cerr << "LXMysql::InsertBin() mysql_stmt_prepare failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return false;
	}
	//绑定字段
	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		//清理stmt申请的内存资源
		mysql_stmt_close(stmt);
		cerr << "LXMysql::InsertBin() mysql_stmt_bind_param failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return false;
	}
	//执行stmt
	if (mysql_stmt_execute(stmt) != 0)
	{
		//清理stmt申请的内存资源
		mysql_stmt_close(stmt);
		cerr << "LXMysql::InsertBin() mysql_stmt_execute failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return false;
	}
	//清理stmt申请的内存资源
	mysql_stmt_close(stmt);

	return true;
}

//生成更新一行数据的sql语句（where语句中，用户传入的字串中要包含where）
std::string LXMysql::GetUpdateSql(XDATA kv, std::string table, std::string where)
{
	//组包sql语句 示例：update `t_video` set `name`='update001',`size`='1000' where `id`='10'
	string sql = "";
	//判断传入的键值对与表名是否为空
	if (kv.empty() || table.empty())
		return "";
	//拼接update语句的前部分
	sql = "update `";
	sql += table;
	sql += "` set ";
	//遍历键值对map，组合好需要更新的字段以及对应内容
	for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		sql += "`";
		sql += ptr->first;
		sql += "`='";
		sql += ptr->second.data;
		sql += "',";
	}
	//去除多余的逗号
	sql[sql.size() - 1] = ' ';
	//拼接用户传入的where语句，用于定位需要更新哪行数据
	sql += " ";
	sql += where;

	return sql;
}

//更新非二进制数据，返回更新的行数，更新失败返回-1
int LXMysql::Update(XDATA kv, std::string table, std::string where)
{
	//首先检查一下mysql是否为空
	if (!mysql)
	{
		cerr << "LXMysql::Update failed! mysql is NULL!" << endl;
		return -1;
	}
	//调用内部接口生成更新数据的sql语句
	string sql = GetUpdateSql(kv, table, where);
	//判断是否成功生成sql语句
	if (sql.empty())
	{
		cerr << "LXMysql::Update failed! GetUpdateSql failed!" << endl;
		return -1;
	}
	//执行更新数据的sql语句
	if (!Query(sql.c_str()))
	{
		cerr << "LXMysql::Update failed! UpdateSql Query failed!" << endl;
		return -1;
	}

	//返回更新的行数
	return mysql_affected_rows(mysql);
}

//更新二进制数据，返回更新的行数，更新失败返回-1
int LXMysql::UpdateBin(XDATA kv, std::string table, std::string where)
{
	//首先检查一下mysql是否为空
	if (!mysql)
	{
		cerr << "LXMysql::UpdateBin failed! mysql is NULL!" << endl;
		return -1;
	}
	//判断传入的键值对与表名是否为空
	if (kv.empty() || table.empty())
	{
		cerr << "LXMysql::UpdateBin failed! kv or table name is empty!" << endl;
		return -1;
	}
	//组包mysql_stmt_prepare用的sql语句 示例：update `t_video` set `name`=?,`size`=? where `id` = 1
	string sql = "";
	sql = "update `";
	sql += table;
	sql += "` set ";
	//定义用于绑定字段的MYSQL_BIND结构体数组（可以绑定的字段数量上限初步设定为256个）
	MYSQL_BIND bind[256] = { 0 };
	int i = 0;
	//利用迭代器遍历kv组合各字段名与对应的键值
	for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		sql += "`";
		sql += ptr->first;
		sql += "`=?,";
		bind[i].buffer = (char*)ptr->second.data;
		bind[i].buffer_length = ptr->second.size;
		bind[i].buffer_type = (enum_field_types)ptr->second.type;
		i++;
	}
	//去除多余的逗号
	sql[sql.size() - 1] = ' ';
	sql += " ";
	sql += where;

	//创建一个stmt对象
	MYSQL_STMT *stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_init failed! The reason is: " << mysql_error(mysql) << endl;
		return -1;
	}
	//预处理sql语句
	if (mysql_stmt_prepare(stmt, sql.c_str(), sql.length()) != 0)
	{
		mysql_stmt_close(stmt);
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_prepare failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return -1;
	}
	//为字段绑定内容
	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		mysql_stmt_close(stmt);
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_bind_param failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return -1;
	}
	//执行stmt
	if (mysql_stmt_execute(stmt) != 0)
	{
		mysql_stmt_close(stmt);
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_execute failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return -1;
	}
	//清理stmt申请的内存资源
	mysql_stmt_close(stmt);

	//返回更新的行数
	return mysql_stmt_affected_rows(stmt);
}

//事务接口 设置手动提交事务
bool LXMysql::StartTransaction()
{
	//标志事务的起点，回滚操作会撤销该sql语句后的所有数据库sql操作语句
	Query("START TRANSACTION");
	//Query("BEGIN");  //同样也是标志事务操作起点

	return Query("set autocommit=0");
}

//事务接口 还原自动提交事务
bool LXMysql::StopTransaction()
{
	return Query("set autocommit=1");
}

//事务接口 提交事务操作
bool LXMysql::Commit()
{
	return Query("commit");
}

//事务接口 回滚操作（撤销）只有存贮引擎为Innodb的数据库才支持
bool LXMysql::Rollback() 
{
	return Query("rollback");
}

//简易结果集获取接口,直接返回select语句产生的结果集中每一行数据，每次调用清理上一次的结果集
XROWS LXMysql::GetResult(const char *sql)
{
	//每次调用前清理上一次的结果集
	FreeResult();
	//定义用于存储返回值的XROWS对象，将结果集中每一行数据存入容器中
	XROWS rows;
	//执行sql语句
	if (!Query(sql))
		return rows;
	//存储产生的结果集到this->result中
	if (!StoreResult())
		return rows;
	//将结果集中的所有数据放入XROWS容器中
	for (;;)
	{
		//将一行中每一个字段内容存入vector<LXData>容器中
		auto row = FetchRow();
		//若该行所有字段为空，说明已经遍历完所有行
		if (row.empty())break;
		//将一行数据构成的容器放入嵌套容器vector<std::vector<LXData>>中
		rows.push_back(row);
	}

	return rows;
}