#include <iostream>
#include <mysql.h>
#include <thread>

using namespace std;

int main03(int argc, char *argv[])
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
		cout << "mysql connect " << host << " success!" << endl;
	}

	//1 ִ��sql�����в�ѯ����,��ѯ������ȡ�����������
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

	//2 ��ȡ�����
	MYSQL_RES* result = mysql_use_result(&mysql);  //��ʵ�ʶ�ȡ
												   //MYSQL_RES* result = mysql_store_result(&mysql);  //��ȫ����ȡ��ע�⻺���С��Ĭ��64MB
	if (!result)
	{
		cout << "mysql_use_result failed!" << sql << mysql_error(&mysql) << endl;
	}

	//3 ��ȡ���ֶ�
	MYSQL_FIELD* field = 0;
	while (field = mysql_fetch_field(result))
	{
		cout << "key: " << field->name << endl;  //������������е��ֶ�����ӡ����
	}

	//4 ��ȡ���ֶ�����
	int fieldNum = mysql_num_fields(result);

	//4 ���������(���л�ȡ)
	MYSQL_ROW row;
	while (row = mysql_fetch_row(result))
	{
		for (int i = 0; i < fieldNum; i++)
		{
			cout << mysql_fetch_field_direct(result, i)->name << ": ";  //ֱ�Ӵӽ�����ĵ�ǰ���л�ȡ��i���������ֶ���
			if (row[i] != NULL)
			{
				cout << row[i];
			} 
			else
			{
				cout << "NULL";
			}		
			cout << endl;
		}
		cout << "===================================================" << endl;
	}

	//4 ��������
	mysql_free_result(result);

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}