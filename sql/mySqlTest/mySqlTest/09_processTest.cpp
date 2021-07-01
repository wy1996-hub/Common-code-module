#include <iostream>
#include <mysql.h>
#include <thread>
#include <fstream>

using namespace std;

int main09(int argc, char *argv[])
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
	const char* host = "127.0.0.1";
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

	//1 创建存储过程
	string sql = "";
	sql = "CREATE PROCEDURE `p_test` (IN p_in INT, OUT p_out INT, INOUT p_inout INT)\
			BEGIN\
			SELECT p_in, p_out, p_inout;\
			SET p_in = 100, p_out = 200,p_inout=300; \
			SELECT p_in,p_out,p_inout;\
			END";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "CREATE PROCEDURE failed!" << mysql_error(&mysql) << endl;
	}

	//2 定义变量并复制
	cout << "输入的变量值为：" << endl << "in=1 out=2 inout=3" << endl;
	sql = "SET @A=1;SET @B=2;SET @C=3;";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << mysql_error(&mysql) << endl;
	}
	//取出定义变量操作产生的结果集
	do
	{
		mysql_affected_rows(&mysql);
	}
	//0 还有结果， -1 没有结果 >1错误
	while (mysql_next_result(&mysql) == 0);

	//3 调用存储过程 call 
	sql = "call p_test(@A, @B, @C)";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "call failed!" << mysql_error(&mysql) << endl;
	}
	cout << "在存储过程中：" << endl;
	//取出调用存储过程产生的结果集
	do
	{
		MYSQL_RES *res = mysql_store_result(&mysql);
		if (!res)
			continue;

		//字段数量
		int fcount = mysql_num_fields(res);

		//打印结果集
		for (;;)
		{
			//提取一行记录
			MYSQL_ROW row = mysql_fetch_row(res);
			if (!row) break;
			for (int i = 0; i < fcount; i++)
			{

				if (row[i])
				{
					cout << row[i] << " ";
				}
				else
					cout << "NULL" << " ";
			}
			cout << endl;

		}
		mysql_free_result(res);
	}
	//0 还有结果， -1 没有结果 >1错误
	while (mysql_next_result(&mysql) == 0);

	//4 获取存储过程执行后三个变量的值
	sql = "select @A, @B, @C";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << mysql_error(&mysql) << endl;
	}
	MYSQL_RES *res = mysql_store_result(&mysql);
	cout << "存储过程执行后: " << endl;
	MYSQL_ROW row = mysql_fetch_row(res);
	cout << " in=" << row[0];
	cout << " out=" << row[1];
	cout << " inout=" << row[2];
	mysql_free_result(res);
	cout << endl;

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}