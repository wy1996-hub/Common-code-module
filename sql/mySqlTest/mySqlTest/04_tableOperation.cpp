#include <iostream>
#include <mysql.h>
#include <thread>
#include <sstream>
#include <map>

using namespace std;

int main04(int argc, char *argv[])
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
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, 0))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	string sql = "";
	//1 创建表
	sql = "CREATE TABLE IF NOT EXISTS `t_image` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `path` varchar(2046), `size` int, PRIMARY KEY(`id`)) ";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "create table failed!" << mysql_error(&mysql) << endl;
	}
	//每次创建表时清空一次表,并将id号置1
	sql = "truncate t_image";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	//2 插入数据
	for (int i = 0; i < 10; i++)
	{
		//组包sql语句
		stringstream ss;
		ss << "insert `t_image` (`name`,`path`,`size`)values('image";
		ss << i << ".jpg','d:/img/', 10240)";
		sql = ss.str();
		//执行sql语句
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "insert data failed!" << mysql_error(&mysql) << endl;
		}
		else
		{
			int count = mysql_affected_rows(&mysql);
			cout << "insert affected rows: " << count << " id = " << mysql_insert_id(&mysql) << endl;
		}
	}

	//3 修改数据
	sql = "update t_image set `name`='test3.png', `size`='3000' where id = 1";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "update data failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		int count = mysql_affected_rows(&mysql);
		cout << "update affected rows: " << count << endl;
	}
	//通过map来修改数据，后续封装时只需要将key和value值传入完成sql语句组包即可
	map<string, string> kv;
	kv.insert(make_pair("name", "image_update001.png"));
	kv.insert(make_pair("size", "20480"));
	string where = "where id=1";
	string tmp = "";
	for (map<string, string>::iterator ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		tmp += "`" + ptr->first + "`='" + ptr->second + "',";
	}
	tmp += " id=id ";  //避免最后多一个逗号问题
	sql = "update t_image set " + tmp + where;
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "use map update data failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		int count = mysql_affected_rows(&mysql);
		cout << "use map update affected rows: " << count << endl;
	}

	//4 删除数据
	//delete语句并没有真实删除实际数据，只是进行了标识与显示调整，id索引并没有清空
	//sql = "delete from t_image";  //删除表中所有内容
	sql = "delete from t_image where id = 1";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "delete data failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		int count = mysql_affected_rows(&mysql);
		cout << "delete affected rows: " << count << endl;
	}
	//可以通过语句OPTIMIZE TABLE进行优化，真实地清理数据,但是索引依然不会清空
	sql = "OPTIMIZE TABLE t_image";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "OPTIMIZE TABLE failed!" << mysql_error(&mysql) << endl;
	}

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}