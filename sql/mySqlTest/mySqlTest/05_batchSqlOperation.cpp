#include <iostream>
#include <mysql.h>
#include <thread>
#include <sstream>
#include <map>

using namespace std;

int main05(int argc, char *argv[])
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
	const char* host = "192.168.8.121";		//目标主机ip地址
	const char* user = "root";				//登录用户名
	const char* pass = "qq489632250.";		//登录密码
	const char* db = "wang";				//数据库名称
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, CLIENT_MULTI_STATEMENTS))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	string sql = "";
	//1 创建表
	sql = "CREATE TABLE IF NOT EXISTS `t_image` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `path` varchar(2046), `size` int, PRIMARY KEY(`id`));";
	//每次创建表时清空一次表,并将id号置1
	sql += "truncate t_image;";

	//2 插入数据
	for (int i = 0; i < 100; i++)
	{
		//组包sql语句
		stringstream ss;
		ss << "insert `t_image` (`name`,`path`,`size`)values('image";
		ss << i << ".jpg','d:/img/', 10240);";
		sql += ss.str();
	}

	//3 修改数据
	sql += "update t_image set `name`='test2.png', `size`='3000' where id = 2;";

	//4 删除数据，delete语句并没有真实删除实际数据，只是进行了标识与显示调整，id索引并没有清空
	//sql = "delete from t_image";  //删除表中所有内容
	sql += "delete from t_image where id = 1;";

	//5 查询数据库wang中表t_image的所有内容
	sql += "select * from t_image;";

	//执行sql语句后会立即返回，此时只是向server提交了所有待执行语句，并没有全部执行好，需要获取结果集
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_batch_query failed!" << mysql_error(&mysql) << endl;
	}

	//server一条条执行sql语句，返回结果
	do 
	{
		cout << "[result]";
		MYSQL_RES* result = mysql_store_result(&mysql);
		if (result)//只有select语句有结果集
		{
			cout << "SELECT mysql_num_rows = " << mysql_num_rows(result) << endl;
		} 
		else
		{
			if (mysql_field_count(&mysql) > 0)  //若有字段但是无结果集返回，判定为select语句执行出错
			{
				cout << "Not retrieve result! " << mysql_error(&mysql) << endl;
			} 
			else
			{
				//等待服务器处理当前sql语句
				cout << mysql_affected_rows(&mysql) << " rows affcted!" << endl;
			}
		}
	}
	while (mysql_next_result(&mysql) == 0);  //若下一条语句执行后有结果，则返回值为0，所有语句执行完毕后返回非0

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}