#include <iostream>
#include <mysql.h>
#include <thread>
#include <fstream>

using namespace std;

int main08(int argc, char *argv[])
{
	//初始化mysql上下文
	MYSQL mysql;
	mysql_init(&mysql);

	//设置连接超时为3秒
	int waitingTime = 3;
	int re = mysql_options(&mysql, MYSQL_OPT_CONNECT_TIMEOUT, &waitingTime);
	if (re != 0)
	{
		cout << "mysql_options failed!" << mysql_error(&mysql) << endl;
	}

	//设置自动重连时间为1秒
	int reConnectTime = 1;
	re = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reConnectTime);
	if (re != 0)
	{
		cout << "mysql_options failed!" << mysql_error(&mysql) << endl;
	}

	//连接登录数据库
	const char* host = "192.168.1.101";
	const char* user = "root";
	const char* pass = "qq489632250.";
	const char* db = "wang";
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, CLIENT_MULTI_STATEMENTS))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	//创建表（声明创建的表使用的存储引擎为InnoDB，该引擎支持事务功能，默认创建的表也是使用该引擎）
	string sql = "";
	sql = "CREATE TABLE IF NOT EXISTS `t_binData` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `data` blob, `size` int, PRIMARY KEY(`id`)) ENGINE InnoDB";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "CREATE TABLE failed!" << mysql_error(&mysql) << endl;
	}

	//每次运行时清空表，并将索引id置1
	sql = "truncate `t_binData`";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	//1 创建一个stmt对象
	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if ( !stmt )
	{
		cerr << "mysql_stmt_init failed!" << mysql_error(&mysql) << endl;
	}

	//2 预处理sql语句
	sql = "insert into `t_binData` (`name`, `data`, `size`) values (?,?,?)";
	re = mysql_stmt_prepare(stmt, sql.c_str(), sql.size());
	if (re != 0)
	{
		cerr << "mysql_stmt_prepare failed!" << mysql_stmt_error(stmt) << endl;
	}

	//3 获取待存储的图片内容
	//为待读取图片绑定文件流
	string filename = "mysql.jpg";  //图片名
	fstream in(filename, ios::in | ios::binary);  //为图片文件绑定文件流（读取二进制的方式）
	if (!in.is_open())  //如果打开文件失败
	{
		cerr << "picture file open failed!" << endl;
	}
	//获取图片文件的大小
	in.seekg(0, ios::end);  //文件指针移动到结尾处
	int filesize = in.tellg();  //获取文件大小
	in.seekg(0, ios::beg);  //文件指针移动回开头处
	//读取图片的字节流内容
	char* picData = new char[filesize];
	int readed = 0;
	while (!in.eof())  //只要还没有读到结尾处就继续读
	{
		in.read(picData + readed, filesize - readed);  //从已读的地方开始，读取总内容数减去已读的字节数
		if (in.gcount() <= 0)  //如果本次没有读取到内容
		{
			break;
		}
		readed += in.gcount();
	}
	//关闭文件流
	in.close();

	//4 为字段绑定内容
	MYSQL_BIND bind[3] = { 0 };  //一共有三个字段，每个字段内容绑定时需要填写3个属性，类型、内容、大小
	bind[0].buffer_type = MYSQL_TYPE_STRING;  //name类型为string
	bind[0].buffer = (char*)filename.c_str();  //name的内容，需要强转为char*,为数据块的地址
	bind[0].buffer_length = filename.size();  //name的大小，占的字节数

	bind[1].buffer_type = MYSQL_TYPE_BLOB;  //data类型
	bind[1].buffer = picData;  //data的内容
	bind[1].buffer_length = filesize;  //data的大小

	//int型的size不需要传递大小
	bind[2].buffer_type = MYSQL_TYPE_LONG;  //size类型
	bind[2].buffer = &filesize;  //size的内容

	re = mysql_stmt_bind_param(stmt, bind);
	if (re != 0)
	{
		cerr << "mysql_stmt_bind_param failed!" << mysql_stmt_error(stmt) << endl;
	}

	//5 执行stmt sql
	re = mysql_stmt_execute(stmt);
	if (re != 0)
	{
		cerr << "mysql_stmt_execute failed!" << mysql_stmt_error(stmt) << endl;
	}
	delete picData;  //删除前面动态申请的内存（用完后尽早释放）

	//6 清理stmt结构体
	re = mysql_stmt_close(stmt);
	if (re != 0)
	{
		cerr << "mysql_stmt_close failed!" << mysql_stmt_error(stmt) << endl;
	}

	//7 查询二进制数据，并保存成文件
	//查询表中的数据
	sql = "select * from `t_binData`";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cerr << "select * from `t_binData` failed!" << mysql_error(&mysql) << endl;
	}

	//获取查询语句得到的结果集
	MYSQL_RES* res = mysql_store_result(&mysql);
	if (!res)
	{
		cerr << "mysql_store_result failed!" << mysql_error(&mysql) << endl;
	}

	//取一行数据
	MYSQL_ROW row = mysql_fetch_row(res);
	if (!row)
	{
		cerr << "mysql_fetch_row failed!" << mysql_error(&mysql) << endl;
	}
	cout << row[0] << " " << row[1] << " " << row[3] << endl;

	//获取每列数据的大小
	unsigned long *lens = mysql_fetch_lengths(res);  //返回一个数组，记录该行数据中每个字段处的数据大小
	int colNum = mysql_num_fields(res);  //返回该行的字段数量，也就是列数
	for (int i = 0; i < colNum; i++)
	{
		cout << "[" << lens[i] << "]" << " ";
	}
	cout << endl;

	//将二进制数据保存成图片文件
	filename = "out_";
	filename += row[1];
	fstream out(filename, ios::out | ios::binary);
	if (!out.is_open())
	{
		cerr << "open out file failed!" << endl;
	}
	out.write(row[2], lens[2]);
	out.close();

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}