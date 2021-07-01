#include <iostream>
#include "LXMysql.h"

using namespace std;

int main(int argc, char *argv[])
{
	//����һ���Զ����LXMysql����
	LXMysql my;

	bool ifOk = false;
	//1 ��ʼ��MySQL API
	my.Init();

	//2 ����ǰ����MySQL��option����
	//�������ӳ�ʱ�ȴ�ʱ��Ϊ3��
	my.SetConnectTimeout(3);
	//�����Զ�����
	my.SetReconnect(true);

	//3 �������ݿ�����,���֮ǰû�е���LXMysql::Init()���ڲ����Զ�����
	my.Connect("127.0.0.1", "root", "qq489632250.", "wang");

	//4 ִ��sql��䴴����
	string sql = "";
	sql = "CREATE TABLE IF NOT EXISTS `t_video`\
			(`id` INT AUTO_INCREMENT,\
			`name` VARCHAR(1024) ,\
			`data` BLOB,\
			`size` INT,\
			PRIMARY KEY(`id`))";
	my.Query(sql.c_str());

	//�������������ݣ�������id��1
	sql = "truncate `t_video`";
	my.Query(sql.c_str());

	//5 ���Բ�������
	//����Ƕ���������
	XDATA normalData;
	normalData.insert(make_pair("name", LXData("getSqlTest")));	 //��map���ݽṹ�д�ֵ�ı�׼��ʽ
	normalData.insert(make_pair("size", LXData("1024")));
	//����sql������ɽӿڣ������ֵ��map���ݽṹ��Ŀ��������
	cout << my.GetInsertSql(normalData, "t_video") << endl;
	my.Insert(normalData, "t_video");
	//����һ������������
	XDATA binData;
	LXData binaryFile;
	binaryFile.LoadFile("mysql.jpg");
	binData["name"] = "mysql_new.jpg";	//��map���ݽṹ�д�ֵ�ı�ݷ�ʽ
	binData["data"] = binaryFile;
	binData["size"] = &binaryFile.size;
	my.InsertBin(binData, "t_video");
	//�������ļ��͵�LXData�����������ļ�����ڶѿռ��������⣬��Ҫ��Drop���������ͷ�
	binaryFile.Drop();

	//6 �������������������
	XDATA data3;
	data3["name"] = "trans001";
	data3["size"] = "1024";
	//�������������ֶ��ύ����
	my.StartTransaction();
	my.Insert(data3, "t_video");
	//�ع�,ǰ������trans001Ӧ��ʧ��
	my.Rollback();
	data3["name"] = "trans002";
	my.Insert(data3, "t_video");
	data3["name"] = "trans003";
	my.Insert(data3, "t_video");
	//�ύ����
	my.Commit();
	//�ر����񣨻�ԭ�Զ��ύ����
	my.StopTransaction();

	//7 ���Ը�������
	//Update�ӿ��޸ķǶ��������� �޸�id=1��name
	XDATA udata;
	udata["name"] = "updateTest";
	cout << "my.Update rows = " << my.Update(udata, "t_video", "where id=1") << endl;
	//UpdateBin�ӿ��޸Ķ��������� �޸�id=3 ��data ͼ��
	XDATA udata2;
	LXData file2;
	file2.LoadFile("mysql2.jpg");
	udata2["data"] = file2;
	udata2["size"] = &file2.size;
	cout << "my.UpdateBin rows = " << my.UpdateBin(udata2, "t_video", "where id=2") << endl;
	file2.Drop();

	//8 �������ȡStoreResult
	//select�����������
	sql = "select * from `t_video`";
	my.Query(sql.c_str());
	//����StoreResult��ȡ�����,��ȡ���Ľ�����洢�ڳ�Ա����my.result�У�UseResult����ʱ�Ѳ��ԣ����Է���һ�£�
	my.StoreResult();

	//9 �ӽ�����л�ȡ����
	for (;;)
	{
		//�ӽ�����л�ȡһ������
		auto row = my.FetchRow();
		//�жϷ��ص��������Ƿ�������
		if (row.size() == 0) break;
		//���ڶ��еĶ�����ͼƬ�洢����
		row[2].SaveFile(row[1].data);
		//��ӡ��ǰ����ÿ���ֶζ�Ӧ������
		for (int i = 0; i < row.size(); i++)
		{
			//�жϸ��ֶε������Ƿ�Ϊ��
			if (row[i].data)
			{
				if (row[i].type == LX_TYPE_BLOB)  //�������ļ���cout��ӡ��������
				{
					cout << "[BLOB]" << "\t";
				}
				else
				{
					cout << row[i].data << "\t";
				}
			}
			else
			{
				cout << "[NULL]" << "\t";
			}
		}
		cout << endl;  //��ӡ��һ��������һ�λ���	
	}

	//��������
	my.FreeResult();

	//10 �����ַ���ת������ ��Ҫ�������ݲ������ȡʱ��UTF8��GBK��ʽ֮���ת������
	cout << "========================��ʼ�����ַ���========================" << endl;
	//����һ��UTF8�����ʽ���ַ�������ʽΪutf8_bin�ı�`t_utf8`
	sql = "CREATE TABLE IF NOT EXISTS `t_utf8` \
		(`id` INT AUTO_INCREMENT,	\
		`name` VARCHAR(1024) CHARACTER SET utf8 COLLATE utf8_bin,\
		PRIMARY KEY(`id`))";
	my.Query(sql.c_str());
	//ÿ������ǰ��ձ�`t_utf8`�е�����
	my.Query("TRUNCATE t_utf8");
	//ָ���ͻ��˺ͷ�����֮�䴫���ַ��ı������ΪUTF8
	my.Query("set names utf8");
	{
		XDATA data;
		data["name"] = u8"���Ե�UTF����";  //��ʱ���ֻ����UTF8��ʽ���ݣ�vs2015Ĭ���ļ���GBK��ʽ�洢����Ҫ���u8ǰ׺���ܲ���ɹ�
		my.Insert(data, "t_utf8");
	}
	//����һ��GBK�����ʽ���ַ�������ʽΪgbk_bin�ı�`t_gbk`
	sql = "CREATE TABLE IF NOT EXISTS `t_gbk` \
		(`id` INT AUTO_INCREMENT,	\
		`name` VARCHAR(1024) CHARACTER SET gbk COLLATE gbk_bin,\
		PRIMARY KEY(`id`))";
	my.Query(sql.c_str());
	//�������
	my.Query("TRUNCATE t_gbk");
	//ָ����mysql������ַ���
	my.Query("set names gbk");
	{
		XDATA data;
		LXData tmp = u8"���Ե�GBK����";
		string gbk = tmp.UTF8ToGBK();  //���ֱ�Ӳ���UTF8��������ݽ���GBK����ı��л��������
		data["name"] = gbk.c_str();
		my.Insert(data, "t_gbk");
	}
	//���Դ�GBK��ʽ�ı��л�ȡ����
	cout << "==== Print names gbk string ==== " << endl;
	my.Query("set names gbk");
	my.Query("select * from t_gbk");
	my.StoreResult();
	for (;;)
	{
		//��ȡһ������
		auto row = my.FetchRow();
		if (row.size() == 0)break;
		cout << "id:" << row[0].data << " name:" << row[1].data << endl;
	}
	my.FreeResult();
	//���Դ�UTF8��ʽ�ı��л�ȡ����
	cout << "==== Print utf-8 string ==== " << endl;
	my.Query("set names utf8");
	my.Query("select * from t_utf8");
	my.StoreResult();
	for (;;)
	{
		//��ȡһ������
		auto row = my.FetchRow();
		if (row.size() == 0)break;
		cout << "id:" << row[0].UTF8ToGBK() << " name:" << row[1].UTF8ToGBK() << endl;
	}
	my.FreeResult();

	//���Լ��׽������ȡ�ӿ�
	XROWS rows = my.GetResult("select * from t_video");
	for (int i = 0; i < rows.size(); i++)
	{
		auto row = rows[i];
		for (int i = 0; i < row.size(); i++)
		{
			if (!row[i].data)
			{
				cout << "[NULL]" << "\t";
				continue;
			}
			switch (row[i].type)
			{
			case LX_TYPE_BLOB:
				cout << "[BLOB]" << "\t";
				break;
			case LX_TYPE_LONG:
			case LX_TYPE_STRING:
			default:
				cout << row[i].data << "\t";

			}
		}
		cout << endl;
	}

	//�����Զ�����
	//for (;;)
	//{
	//	cout << my.Query(sql.c_str()) << flush;
	//}


	//����MySQLռ�õ�������Դ
	my.Close();

	cout << endl << "test_LXMysql run success!" << endl;
	getchar();
	return 0;
}