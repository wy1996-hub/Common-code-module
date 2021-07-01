#include <iostream>
#include <mysql.h>
#include <thread>

using namespace std;

int main02(int argc, char *argv[])
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
	const char* db = "mysql";
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, 0))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	//1 执行sql语句进行查询操作,查询后必须获取结果集并清理
	const char* sql = "select * from user";
	re = mysql_real_query(&mysql, sql, strlen(sql));
	if (re != 0)
	{
		cout << "mysql_real_query failed!" << sql << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql_real_query success!  " << sql << endl;
	}

	//2 获取结果集
	MYSQL_RES* result = mysql_use_result(&mysql);  //不实际读取
	//MYSQL_RES* result = mysql_store_result(&mysql);  //会全部读取，注意缓存大小，默认64MB
	if (!result)
	{
		cout << "mysql_use_result failed!" << sql << mysql_error(&mysql) << endl;
	}

	//3 遍历结果集
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result))
	{
		unsigned long* lens = mysql_fetch_lengths(result);
		cout << lens[0] << "[" << row[0] << ", " << row[1] << "]" << endl;
	}

	//4 清理结果集
	mysql_free_result(result);


	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}