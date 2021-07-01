#include <iostream>
#include <mysql.h>
#include <thread>
#include <sstream>
#include <map>

using namespace std;

int main05(int argc, char *argv[])
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
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, CLIENT_MULTI_STATEMENTS))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	string sql = "";
	//1 ������
	sql = "CREATE TABLE IF NOT EXISTS `t_image` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `path` varchar(2046), `size` int, PRIMARY KEY(`id`));";
	//ÿ�δ�����ʱ���һ�α�,����id����1
	sql += "truncate t_image;";

	//2 ��������
	for (int i = 0; i < 100; i++)
	{
		//���sql���
		stringstream ss;
		ss << "insert `t_image` (`name`,`path`,`size`)values('image";
		ss << i << ".jpg','d:/img/', 10240);";
		sql += ss.str();
	}

	//3 �޸�����
	sql += "update t_image set `name`='test2.png', `size`='3000' where id = 2;";

	//4 ɾ�����ݣ�delete��䲢û����ʵɾ��ʵ�����ݣ�ֻ�ǽ����˱�ʶ����ʾ������id������û�����
	//sql = "delete from t_image";  //ɾ��������������
	sql += "delete from t_image where id = 1;";

	//5 ��ѯ���ݿ�wang�б�t_image����������
	sql += "select * from t_image;";

	//ִ��sql������������أ���ʱֻ����server�ύ�����д�ִ����䣬��û��ȫ��ִ�кã���Ҫ��ȡ�����
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "mysql_batch_query failed!" << mysql_error(&mysql) << endl;
	}

	//serverһ����ִ��sql��䣬���ؽ��
	do 
	{
		cout << "[result]";
		MYSQL_RES* result = mysql_store_result(&mysql);
		if (result)//ֻ��select����н����
		{
			cout << "SELECT mysql_num_rows = " << mysql_num_rows(result) << endl;
		} 
		else
		{
			if (mysql_field_count(&mysql) > 0)  //�����ֶε����޽�������أ��ж�Ϊselect���ִ�г���
			{
				cout << "Not retrieve result! " << mysql_error(&mysql) << endl;
			} 
			else
			{
				//�ȴ�����������ǰsql���
				cout << mysql_affected_rows(&mysql) << " rows affcted!" << endl;
			}
		}
	}
	while (mysql_next_result(&mysql) == 0);  //����һ�����ִ�к��н�����򷵻�ֵΪ0���������ִ����Ϻ󷵻ط�0

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}