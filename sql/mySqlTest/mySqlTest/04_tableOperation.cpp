#include <iostream>
#include <mysql.h>
#include <thread>
#include <sstream>
#include <map>

using namespace std;

int main04(int argc, char *argv[])
{
	//��ʼ��mysql������
	MYSQL mysql;
	mysql_init(&mysql);

	//�������ӳ�ʱΪ3��
	int waitingTime = 3;
	int re = mysql_options(&mysql, MYSQL_OPT_CONNECT_TIMEOUT, &waitingTime);
	if (re != 0)
	{
		cout << "mysql_options failed!" << mysql_error(&mysql) << endl;
	}
	//�����Զ�����ʱ��Ϊ1��
	int reConnectTime = 1;
	re = mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reConnectTime);
	if (re != 0)
	{
		cout << "mysql_options failed!" << mysql_error(&mysql) << endl;
	}

	//���ӵ�¼���ݿ�
	const char* host = "192.168.8.121";		//Ŀ������ip��ַ
	const char* user = "root";				//��¼�û���
	const char* pass = "qq489632250.";		//��¼����
	const char* db = "wang";				//���ݿ�����
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, 0))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	string sql = "";
	//1 ������
	sql = "CREATE TABLE IF NOT EXISTS `t_image` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `path` varchar(2046), `size` int, PRIMARY KEY(`id`)) ";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "create table failed!" << mysql_error(&mysql) << endl;
	}
	//ÿ�δ�����ʱ���һ�α�,����id����1
	sql = "truncate t_image";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	//2 ��������
	for (int i = 0; i < 10; i++)
	{
		//���sql���
		stringstream ss;
		ss << "insert `t_image` (`name`,`path`,`size`)values('image";
		ss << i << ".jpg','d:/img/', 10240)";
		sql = ss.str();
		//ִ��sql���
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

	//3 �޸�����
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
	//ͨ��map���޸����ݣ�������װʱֻ��Ҫ��key��valueֵ�������sql����������
	map<string, string> kv;
	kv.insert(make_pair("name", "image_update001.png"));
	kv.insert(make_pair("size", "20480"));
	string where = "where id=1";
	string tmp = "";
	for (map<string, string>::iterator ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		tmp += "`" + ptr->first + "`='" + ptr->second + "',";
	}
	tmp += " id=id ";  //��������һ����������
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

	//4 ɾ������
	//delete��䲢û����ʵɾ��ʵ�����ݣ�ֻ�ǽ����˱�ʶ����ʾ������id������û�����
	//sql = "delete from t_image";  //ɾ��������������
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
	//����ͨ�����OPTIMIZE TABLE�����Ż�����ʵ����������,����������Ȼ�������
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