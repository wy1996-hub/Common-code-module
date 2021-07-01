#include <iostream>
#include <mysql.h>
#include <thread>
#include <fstream>

using namespace std;

int main08(int argc, char *argv[])
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
	sql = "CREATE TABLE IF NOT EXISTS `t_binData` ( `id` int AUTO_INCREMENT, `name` varchar(1024), `data` blob, `size` int, PRIMARY KEY(`id`)) ENGINE InnoDB";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "CREATE TABLE failed!" << mysql_error(&mysql) << endl;
	}

	//ÿ������ʱ��ձ���������id��1
	sql = "truncate `t_binData`";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cout << "truncate table failed!" << mysql_error(&mysql) << endl;
	}

	//1 ����һ��stmt����
	MYSQL_STMT* stmt = mysql_stmt_init(&mysql);
	if ( !stmt )
	{
		cerr << "mysql_stmt_init failed!" << mysql_error(&mysql) << endl;
	}

	//2 Ԥ����sql���
	sql = "insert into `t_binData` (`name`, `data`, `size`) values (?,?,?)";
	re = mysql_stmt_prepare(stmt, sql.c_str(), sql.size());
	if (re != 0)
	{
		cerr << "mysql_stmt_prepare failed!" << mysql_stmt_error(stmt) << endl;
	}

	//3 ��ȡ���洢��ͼƬ����
	//Ϊ����ȡͼƬ���ļ���
	string filename = "mysql.jpg";  //ͼƬ��
	fstream in(filename, ios::in | ios::binary);  //ΪͼƬ�ļ����ļ�������ȡ�����Ƶķ�ʽ��
	if (!in.is_open())  //������ļ�ʧ��
	{
		cerr << "picture file open failed!" << endl;
	}
	//��ȡͼƬ�ļ��Ĵ�С
	in.seekg(0, ios::end);  //�ļ�ָ���ƶ�����β��
	int filesize = in.tellg();  //��ȡ�ļ���С
	in.seekg(0, ios::beg);  //�ļ�ָ���ƶ��ؿ�ͷ��
	//��ȡͼƬ���ֽ�������
	char* picData = new char[filesize];
	int readed = 0;
	while (!in.eof())  //ֻҪ��û�ж�����β���ͼ�����
	{
		in.read(picData + readed, filesize - readed);  //���Ѷ��ĵط���ʼ����ȡ����������ȥ�Ѷ����ֽ���
		if (in.gcount() <= 0)  //�������û�ж�ȡ������
		{
			break;
		}
		readed += in.gcount();
	}
	//�ر��ļ���
	in.close();

	//4 Ϊ�ֶΰ�����
	MYSQL_BIND bind[3] = { 0 };  //һ���������ֶΣ�ÿ���ֶ����ݰ�ʱ��Ҫ��д3�����ԣ����͡����ݡ���С
	bind[0].buffer_type = MYSQL_TYPE_STRING;  //name����Ϊstring
	bind[0].buffer = (char*)filename.c_str();  //name�����ݣ���ҪǿתΪchar*,Ϊ���ݿ�ĵ�ַ
	bind[0].buffer_length = filename.size();  //name�Ĵ�С��ռ���ֽ���

	bind[1].buffer_type = MYSQL_TYPE_BLOB;  //data����
	bind[1].buffer = picData;  //data������
	bind[1].buffer_length = filesize;  //data�Ĵ�С

	//int�͵�size����Ҫ���ݴ�С
	bind[2].buffer_type = MYSQL_TYPE_LONG;  //size����
	bind[2].buffer = &filesize;  //size������

	re = mysql_stmt_bind_param(stmt, bind);
	if (re != 0)
	{
		cerr << "mysql_stmt_bind_param failed!" << mysql_stmt_error(stmt) << endl;
	}

	//5 ִ��stmt sql
	re = mysql_stmt_execute(stmt);
	if (re != 0)
	{
		cerr << "mysql_stmt_execute failed!" << mysql_stmt_error(stmt) << endl;
	}
	delete picData;  //ɾ��ǰ�涯̬������ڴ棨��������ͷţ�

	//6 ����stmt�ṹ��
	re = mysql_stmt_close(stmt);
	if (re != 0)
	{
		cerr << "mysql_stmt_close failed!" << mysql_stmt_error(stmt) << endl;
	}

	//7 ��ѯ���������ݣ���������ļ�
	//��ѯ���е�����
	sql = "select * from `t_binData`";
	re = mysql_query(&mysql, sql.c_str());
	if (re != 0)
	{
		cerr << "select * from `t_binData` failed!" << mysql_error(&mysql) << endl;
	}

	//��ȡ��ѯ���õ��Ľ����
	MYSQL_RES* res = mysql_store_result(&mysql);
	if (!res)
	{
		cerr << "mysql_store_result failed!" << mysql_error(&mysql) << endl;
	}

	//ȡһ������
	MYSQL_ROW row = mysql_fetch_row(res);
	if (!row)
	{
		cerr << "mysql_fetch_row failed!" << mysql_error(&mysql) << endl;
	}
	cout << row[0] << " " << row[1] << " " << row[3] << endl;

	//��ȡÿ�����ݵĴ�С
	unsigned long *lens = mysql_fetch_lengths(res);  //����һ�����飬��¼����������ÿ���ֶδ������ݴ�С
	int colNum = mysql_num_fields(res);  //���ظ��е��ֶ�������Ҳ��������
	for (int i = 0; i < colNum; i++)
	{
		cout << "[" << lens[i] << "]" << " ";
	}
	cout << endl;

	//�����������ݱ����ͼƬ�ļ�
	filename = "out_";
	filename += row[1];
	fstream out(filename, ios::out | ios::binary);
	if (!out.is_open())
	{
		cerr << "open out file failed!" << endl;
	}
	out.write(row[2], lens[2]);
	out.close();

	mysql_close(&mysql);
	mysql_library_end();
	cout << "finish" << endl;
	getchar();
	return 0;
}