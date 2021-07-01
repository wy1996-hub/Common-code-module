#include <iostream>
#include <mysql.h>
#include <thread>

using namespace std;

int main06(int argc, char *argv[])
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
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, 0))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	//创建表（声明创建的表使用的存储引擎为InnoDB，该引擎支持事务功能，默认创建的表也是使用该引擎）
	string sql = "";
	sql = "CREATE TABLE IF NOT EXISTS `t_video` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `path` varchar(2046), `size` int, PRIMARY KEY(`id`)) ENGINE InnoDB;";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "CREATE TABLE failed!" << mysql_error(&mysql) << endl;
	}

	//每次运行时清空表，并将索引id置1
	sql = "truncate t_video";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	/********************事务处理*********************/
	//1.开始事务 START TRANSACTION
	sql = "START TRANSACTION";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "START TRANSACTION failed!" << mysql_error(&mysql) << endl;
	}

	//2.将事务提交方式改为手动提交 set autocommit = 0，这样必须通过commit语句才能统一执行一个执行单元
	sql = "set autocommit = 0";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "set autocommit = 0 failed!" << mysql_error(&mysql) << endl;
	}

	//3.执行三条用于回滚测试的sql语句
	for (int i = 0; i < 3; i++)
	{
		sql = "insert into `t_video` (`name`, `path`, `size`) values ('transactionTest', 'd:/video/', '102400')";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "insert transactionTest failed!" << mysql_error(&mysql) << endl;
		}
	}

	//4.回滚操作 ROLLBACK 
	//注意：MYISAM存储引擎不支持，需要确保该表的存储引擎为InnoDB
	//回滚操作后索引号不恢复
	sql = "ROLLBACK";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "ROLLBACK failed!" << mysql_error(&mysql) << endl;
	}

	//4.执行1000条用于提交测试的sql语句
	for (int i = 0; i < 1000; i++)
	{
		sql = "insert into `t_video` (`name`, `path`, `size`) values ('commitTest', 'd:/video/', '102400')";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "insert commitTest failed!" << mysql_error(&mysql) << endl;
		}
	}

	//5.提交操作 COMMIT
	sql = "COMMIT";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "COMMIT failed!" << mysql_error(&mysql) << endl;
	}

	//6.将事务提交方式恢复为自动提交 set autocommit = 1
	sql = "set autocommit = 1";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "set autocommit = 1 failed!" << mysql_error(&mysql) << endl;
	}

	//手动查询一下已有的数据行数
	sql = "select count(*) from t_video";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "select count(*) failed!" << mysql_error(&mysql) << endl;
	}
	//获取结果集
	MYSQL_RES* result = mysql_store_result(&mysql);
	MYSQL_ROW row = mysql_fetch_row(result);
	//打印当前有多少行数据
	cout << "t_video count(*) = " << row[0] << endl;
	

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}