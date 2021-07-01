#include <iostream>
#include <mysql.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

int main07(int argc, char *argv[])
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

	//���������������ı�ʹ�õĴ洢����ΪInnoDB��������֧�������ܣ�Ĭ�ϴ����ı�Ҳ��ʹ�ø����棩
	string sql = "";
	sql = "CREATE TABLE IF NOT EXISTS `t_speed` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `size` int, PRIMARY KEY(`id`)) ENGINE InnoDB;";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "CREATE TABLE failed!" << mysql_error(&mysql) << endl;
	}

	//ÿ������ʱ��ձ���������id��1
	sql = "truncate t_speed";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	/**********************************************************���ֲ��뷽ʽ�ٶȶԱ�*********************************************************/
	//1 ����ÿ���õ���������1000����������ʱ��
	auto start = system_clock::now();	//��¼��ʼʱ��
	for (int i = 0; i < 1000; i++)
	{
		sql = "insert into `t_speed` (`name`, `size`) values ('insert speed test 1', '100')";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "mysql_query 'single insert' failed!" << mysql_error(&mysql) << endl;
		}
	}
	auto end = system_clock::now();	//��¼����ʱ��
	auto timeCost = duration_cast<milliseconds>(end - start);  //�����ʱ ����
	cout << "����������1000����������ʱ��Ϊ��" << timeCost.count() << endl;

	//2 ���Զ������һ�����ύ����1000����������ʱ��
	{
		auto start = system_clock::now();	//��¼��ʼʱ��
		//���1000��sql���
		sql = "";
		for (int i = 0; i < 1000; i++)
		{
			sql += "insert into `t_speed` (`name`, `size`) values ('insert speed test 2', '100');";
		}
		//ע��˴�ִ��sql����Ϊ�ύ���������������أ�����ʱ��
		re = mysql_query(&mysql, sql.c_str());  
		if (re != 0)
		{
			cout << "mysql_query 'batch insert' failed!" << mysql_error(&mysql) << endl;
		}
		//�ȴ�server��ִ����1000��sql��䣬�Լ�¼��ʵ����ʱ��
		do 
		{
			//cout << mysql_affected_rows(&mysql) << flush;
			mysql_affected_rows(&mysql);
		} while (mysql_next_result(&mysql) == 0);  //mysql_next_result��䷵��0��ʾ����һ���������-1��ʾû����һ���������>1��ʾ��������
		auto end = system_clock::now();	//��¼����ʱ��
		auto timeCost = duration_cast<milliseconds>(end - start);  //�����ʱ ����
		cout << "�������һ�����ύ����1000����������ʱ��Ϊ��" << timeCost.count() << endl;
	}

	//3 ���������������1000����������ʱ��
	{
		auto start = system_clock::now();	//��¼��ʼʱ��

		//��ʼ����
		sql = "START TRANSACTION";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "START TRANSACTION failed!" << mysql_error(&mysql) << endl;
		}

		//�������ύ��ʽ��Ϊ�ֶ�
		sql = "set autocommit = 0";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "set autocommit = 0 failed!" << mysql_error(&mysql) << endl;
		}

		//����1000������
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
				mysql_affected_rows(&mysql);  //�ȴ�server��ִ����sql���
			}
		}

		//�ύ����
		sql = "COMMIT";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "COMMIT failed!" << mysql_error(&mysql) << endl;
		}

		//�������ύ��ʽ��Ϊ�Զ�
		sql = "set autocommit = 1";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "set autocommit = 1 failed!" << mysql_error(&mysql) << endl;
		}

		auto end = system_clock::now();	//��¼����ʱ��
		auto timeCost = duration_cast<milliseconds>(end - start);  //�����ʱ ����
		cout << "�����������1000����������ʱ��Ϊ��" << timeCost.count() << endl;
	}


	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}