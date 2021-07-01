#include <iostream>
#include <mysql.h>
#include <thread>

using namespace std;

int main01(int argc, char *argv[])
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
		cout << "mysql connect" << host << "success!" << endl;
	}

	for (int i = 0; i < 1000; i++)
	{
		int re = mysql_ping(&mysql);
		if (re == 0)
		{
			cout << host << ":myaql ping success!" << endl;
		} 
		else
		{
			cout << host << ":myaql ping failed!" << mysql_error(&mysql) << endl;
		}
		this_thread::sleep_for(1s);
	}

	
	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}