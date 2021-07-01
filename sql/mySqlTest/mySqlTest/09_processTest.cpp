#include <iostream>
#include <mysql.h>
#include <thread>
#include <fstream>

using namespace std;

int main09(int argc, char *argv[])
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

	//1 �����洢����
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

	//2 �������������
	cout << "����ı���ֵΪ��" << endl << "in=1 out=2 inout=3" << endl;
	sql = "SET @A=1;SET @B=2;SET @C=3;";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << mysql_error(&mysql) << endl;
	}
	//ȡ������������������Ľ����
	do
	{
		mysql_affected_rows(&mysql);
	}
	//0 ���н���� -1 û�н�� >1����
	while (mysql_next_result(&mysql) == 0);

	//3 ���ô洢���� call 
	sql = "call p_test(@A, @B, @C)";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "call failed!" << mysql_error(&mysql) << endl;
	}
	cout << "�ڴ洢�����У�" << endl;
	//ȡ�����ô洢���̲����Ľ����
	do
	{
		MYSQL_RES *res = mysql_store_result(&mysql);
		if (!res)
			continue;

		//�ֶ�����
		int fcount = mysql_num_fields(res);

		//��ӡ�����
		for (;;)
		{
			//��ȡһ�м�¼
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
	//0 ���н���� -1 û�н�� >1����
	while (mysql_next_result(&mysql) == 0);

	//4 ��ȡ�洢����ִ�к�����������ֵ
	sql = "select @A, @B, @C";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << mysql_error(&mysql) << endl;
	}
	MYSQL_RES *res = mysql_store_result(&mysql);
	cout << "�洢����ִ�к�: " << endl;
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