#include <iostream>
#include <mysql.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

int main07(int argc, char *argv[])
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
	const char* host = "192.168.8.121";
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
	sql = "CREATE TABLE IF NOT EXISTS `t_speed` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `size` int, PRIMARY KEY(`id`)) ENGINE InnoDB;";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "CREATE TABLE failed!" << mysql_error(&mysql) << endl;
	}

	//每次运行时清空表，并将索引id置1
	sql = "truncate t_speed";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	/**********************************************************三种插入方式速度对比*********************************************************/
	//1 测试每次用单条语句插入1000条数据所用时间
	auto start = system_clock::now();	//记录开始时间
	for (int i = 0; i < 1000; i++)
	{
		sql = "insert into `t_speed` (`name`, `size`) values ('insert speed test 1', '100')";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query 'single insert' failed!" << mysql_error(&mysql) << endl;
		}
	}
	auto end = system_clock::now();	//记录结束时间
	auto timeCost = duration_cast<milliseconds>(end - start);  //计算耗时 毫秒
	cout << "单条语句插入1000条数据所用时间为：" << timeCost.count() << endl;

	//2 测试多条语句一次性提交插入1000条数据所用时间
	{
		auto start = system_clock::now();	//记录开始时间
		//组包1000条sql语句
		sql = "";
		for (int i = 0; i < 1000; i++)
		{
			sql += "insert into `t_speed` (`name`, `size`) values ('insert speed test 2', '100');";
		}
		//注意此处执行sql语句仅为提交操作，会立即返回，不耗时间
		re = mysql_query(&mysql, sql.c_str());  
		if (re != 0)
		{
			cout << "mysql_query 'batch insert' failed!" << mysql_error(&mysql) << endl;
		}
		//等待server端执行完1000条sql语句，以记录真实处理时间
		do 
		{
			//cout << mysql_affected_rows(&mysql) << flush;
			mysql_affected_rows(&mysql);
		} while (mysql_next_result(&mysql) == 0);  //mysql_next_result语句返回0表示有下一个结果集，-1表示没有下一个结果集，>1表示发生错误
		auto end = system_clock::now();	//记录结束时间
		auto timeCost = duration_cast<milliseconds>(end - start);  //计算耗时 毫秒
		cout << "多条语句一次性提交插入1000条数据所用时间为：" << timeCost.count() << endl;
	}

	//3 测试利用事务插入1000条数据所用时间
	{
		auto start = system_clock::now();	//记录开始时间

		//开始事务
		sql = "START TRANSACTION";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "START TRANSACTION failed!" << mysql_error(&mysql) << endl;
		}

		//将事务提交方式改为手动
		sql = "set autocommit = 0";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "set autocommit = 0 failed!" << mysql_error(&mysql) << endl;
		}

		//插入1000条数据
		for (int i = 0; i < 1000; i++)
		{
			sql = "insert into `t_speed` (`name`, `size`) values ('insert speed test 2', '100');";
			re = mysql_query(&mysql, sql.c_str());
			if (re != 0)
			{
				cout << "mysql_query 'batch insert' failed!" << mysql_error(&mysql) << endl;
			}
			else
			{
				mysql_affected_rows(&mysql);  //等待server端执行完sql语句
			}
		}

		//提交事务
		sql = "COMMIT";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "COMMIT failed!" << mysql_error(&mysql) << endl;
		}

		//将事务提交方式改为自动
		sql = "set autocommit = 1";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "set autocommit = 1 failed!" << mysql_error(&mysql) << endl;
		}

		auto end = system_clock::now();	//记录结束时间
		auto timeCost = duration_cast<milliseconds>(end - start);  //计算耗时 毫秒
		cout << "利用事务插入1000条数据所用时间为：" << timeCost.count() << endl;
	}


	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}