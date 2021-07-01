#include <iostream>
#include <mysql.h>
#include <thread>

using namespace std;

int main06(int argc, char *argv[])
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
	const char* db = "wang";
	if (!mysql_real_connect(&mysql, host, user, pass, db, 3306, 0, 0))
	{
		cout << "mysql connect failed!" << mysql_error(&mysql) << endl;
	}
	else
	{
		cout << "mysql connect " << host << " success!" << endl;
	}

	//���������������ı�ʹ�õĴ洢����ΪInnoDB��������֧�������ܣ�Ĭ�ϴ����ı�Ҳ��ʹ�ø����棩
	string sql = "";
	sql = "CREATE TABLE IF NOT EXISTS `t_video` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `path` varchar(2046), `size` int, PRIMARY KEY(`id`)) ENGINE InnoDB;";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "CREATE TABLE failed!" << mysql_error(&mysql) << endl;
	}

	//ÿ������ʱ��ձ���������id��1
	sql = "truncate t_video";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	/********************������*********************/
	//1.��ʼ���� START TRANSACTION
	sql = "START TRANSACTION";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "START TRANSACTION failed!" << mysql_error(&mysql) << endl;
	}

	//2.�������ύ��ʽ��Ϊ�ֶ��ύ set autocommit = 0����������ͨ��commit������ͳһִ��һ��ִ�е�Ԫ
	sql = "set autocommit = 0";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "set autocommit = 0 failed!" << mysql_error(&mysql) << endl;
	}

	//3.ִ���������ڻع����Ե�sql���
	for (int i = 0; i < 3; i++)
	{
		sql = "insert into `t_video` (`name`, `path`, `size`) values ('transactionTest', 'd:/video/', '102400')";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "insert transactionTest failed!" << mysql_error(&mysql) << endl;
		}
	}

	//4.�ع����� ROLLBACK 
	//ע�⣺MYISAM�洢���治֧�֣���Ҫȷ���ñ�Ĵ洢����ΪInnoDB
	//�ع������������Ų��ָ�
	sql = "ROLLBACK";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "ROLLBACK failed!" << mysql_error(&mysql) << endl;
	}

	//4.ִ��1000�������ύ���Ե�sql���
	for (int i = 0; i < 1000; i++)
	{
		sql = "insert into `t_video` (`name`, `path`, `size`) values ('commitTest', 'd:/video/', '102400')";
		re = mysql_query(&mysql, sql.c_str());
		if (re != 0)
		{
			cout << "insert commitTest failed!" << mysql_error(&mysql) << endl;
		}
	}

	//5.�ύ���� COMMIT
	sql = "COMMIT";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "COMMIT failed!" << mysql_error(&mysql) << endl;
	}

	//6.�������ύ��ʽ�ָ�Ϊ�Զ��ύ set autocommit = 1
	sql = "set autocommit = 1";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "set autocommit = 1 failed!" << mysql_error(&mysql) << endl;
	}

	//�ֶ���ѯһ�����е���������
	sql = "select count(*) from t_video";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "select count(*) failed!" << mysql_error(&mysql) << endl;
	}
	//��ȡ�����
	MYSQL_RES* result = mysql_store_result(&mysql);
	MYSQL_ROW row = mysql_fetch_row(result);
	//��ӡ��ǰ�ж���������
	cout << "t_video count(*) = " << row[0] << endl;
	

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}