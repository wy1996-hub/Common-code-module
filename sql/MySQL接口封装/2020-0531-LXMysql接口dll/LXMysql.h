#pragma once

#include <vector>
#include "LXData.h"

//��ͷ�ļ�������mysql���õ��Ľṹ�壬����ͷ�ļ���������࣬����������
struct MYSQL;
struct MYSQL_RES;

class LXAPI LXMysql
{
public:
	//��ʼ��MySQL API
	bool Init();

	//����MySQLռ�õ�������Դ
	void Close();

	//mysql�����趨 Connect֮ǰ����
	bool Options(LX_OPT opt, const void* arg);

	//���ݿ����ӣ��������̰߳�ȫ�� ����flag��־λΪ1����֧��һ��ִ�ж���sql���
	bool Connect(const char* host, const char* user, const char* pass, const char* db, unsigned short port = 3306, unsigned long flag = 0);

	//ִ��sql���  �������Ĭ��sqllen=0 ��ͨ��strlen��ȡ�ַ����ȣ��ڲ���mysql_real_queryִ��sql��䣩
	bool Query(const char* sql, unsigned long sqllen = 0);

	//�������ӳ�ʱʱ�䣨��/s��
	bool SetConnectTimeout(int sec);

	//�����Զ�������mysqlĬ��������flase
	bool SetReconnect(bool ifReconnect = true);

	//�������ȡ��ʽ1 һ���Դ洢���н����
	bool StoreResult();

	//�������ȡ��ʽ2 ��ʶ��ʼ��ȡ�������ͨ��Fetch�ӿ�ʵ�ʻ�ȡ�����
	bool UseResult();

	//�����������ͷŽ����ռ�õĿռ�
	void FreeResult();

	//�ӽ�����л�ȡһ������,���ݴ洢���Զ�����������LXData������������
	std::vector<LXData> FetchRow();

	//���ɲ���һ���Ƕ��������ݵ�sql���
	std::string GetInsertSql(XDATA kv, std::string table);

	//����Ƕ��������ݣ�ֻ������ִ���
	bool Insert(XDATA kv, std::string table);

	//�������������
	bool InsertBin(XDATA kv, std::string table);

	//���ɸ���һ�����ݵ�sql��䣨where����У��û�������ִ���Ҫ����where��
	std::string GetUpdateSql(XDATA kv, std::string table, std::string where);

	//���·Ƕ��������ݣ����ظ��µ�����������ʧ�ܷ���-1
	int Update(XDATA kv, std::string table, std::string where);

	//���¶��������ݣ����ظ��µ�����������ʧ�ܷ���-1
	int UpdateBin(XDATA kv, std::string table, std::string where);

	//����ӿ� �����ֶ��ύ����
	bool StartTransaction();

	//����ӿ� ��ԭ�Զ��ύ����
	bool StopTransaction();

	//����ӿ� �ύ�������
	bool Commit();

	//����ӿ� �ع�������������ֻ�д�������ΪInnodb�����ݿ��֧��
	bool Rollback();

	//���׽������ȡ�ӿ�,ֱ�ӷ���select�������Ľ������ÿһ�����ݣ�ÿ�ε���������һ�εĽ����
	XROWS GetResult(const char *sql);

protected:
	//mysql�����ģ���ָ�����ݿ⽨�����Ӻ󣬴洢���ڸ����ݿ��������Ϣ��
	MYSQL* mysql = 0;

	//��ȡ���Ľ����
	MYSQL_RES* result = 0;

};

