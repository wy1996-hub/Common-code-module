#include "LXMysql.h"

#include <iostream>
#include <mysql.h>

using namespace std;

//��ʼ��MySQL API
bool LXMysql::Init() 
{
	//��ʼ��ǰ��ִ��һ��������Դ��������ֹ����ʱ�������Դ�ظ�����
	Close();
	//�´���һ��mysql����
	mysql = mysql_init(0);
	//mysql_init(mysql);
	if (!mysql)
	{
		cerr << "LXMysql::Init() failed!" << endl;
		return false;
	}

	//cout << "LXMysql::Init() success!" << endl;
	return true;
}

//����MySQLռ�õ�������Դ
void LXMysql::Close()
{
	//������һ���Ѵ��ڵĽ����
	FreeResult();
	//��mysqlָ�벻Ϊ�գ���ִ������
	if (mysql)
	{
		mysql_close(mysql);		//�ر�����mysql���������mysql��ռ�õĿռ�
		mysql = NULL;			//��ֹ�ٴιر�ʱ����
	}
	//cout << "LXMysql::Close() success!" << endl;
}

//mysql�����趨 Connect֮ǰ����
bool LXMysql::Options(LX_OPT opt, const void* arg)
{
	//���ȼ��һ��mysql�Ƿ�Ϊ��
	if (!mysql)
	{
		cerr << "LXMysql::Options failed! mysql is NULL!" << endl;
		return false;
	}
	//����mysql_options���в����趨��LX_OPTΪ�Զ���ö�ٽṹ����Ҫ��MySQL�ڲ������mysql_optionö�ٽṹ����һ��
	int re = mysql_options(mysql, (mysql_option)opt, arg);  //LX_OPTö�ٽṹ��ҪǿתΪmysql_optionö�ٽṹ
	if (re != 0)
	{
		cerr << "LXMysql::Options() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	return true;
}

//���ݿ����ӣ��������̰߳�ȫ�� ����flag��־λ����֧��һ��ִ�ж���sql���
bool LXMysql::Connect(const char* host, const char* user, const char* pass, const char* db, unsigned short port, unsigned long flag)
{
	//��ʵ������ǰ���mysql�Ƿ�Ϊ�գ���Ϊ����ִ��һ�γ�ʼ��������ȷ������MYSQL������ܽ������Ӳ���
	if (!mysql)
	{
		if (!Init())
		{
			cerr << "LXMysql::Connect() failed! MYSQL struct can not init!" << endl;
			return false;
		}
	}
	//������ݿ����Ӳ���
	if (!mysql_real_connect(mysql, host, user, pass, db, port, 0, flag))
	{
		cerr << "LXMysql::Connect() failed! The reason is: " << endl << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::Connect() '" << host << "' success!" << endl;
	return true;
}

//ִ��sql���  �������Ĭ��sqllen=0 ��ͨ��strlen��ȡ�ַ����ȣ��ڲ���mysql_real_queryִ��sql��䣩
bool LXMysql::Query(const char* sql, unsigned long sqllen)
{
	//���ȼ��һ��mysql�Ƿ�Ϊ��
	if (!mysql)
	{
		cerr << "LXMysql::Query failed! mysql is NULL!" << endl;
		return false;
	}
	//�жϴ����sql����Ƿ�Ϊ��
	if (!sql)
	{
		cerr << "LXMysql::Query failed! sql is NULL��" << endl;
		return false;
	}
	//�ж��û��Ƿ�ѡ����sql���ĳ���
	if (sqllen <= 0)  //Ĭ�ϲ�����sql����ʱ��sqllenΪ0���ڲ�ͨ��strlen������ȡ
		sqllen = (unsigned long)strlen(sql);
	//�ٴ��ж�sql���ĳ����Ƿ�����
	if (sqllen <= 0)  //sql����ֵ��<=0����Ϊ�쳣״̬
	{
		cerr << "LXMysql::Query failed! sql is empty or wrong format!" << endl;
		return false;
	}
	//ִ�и�sql���
	int re = mysql_real_query(mysql, sql, sqllen);
	if (re != 0)
	{
		cerr << "LXMysql::Query() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::Query() success!" << endl;
	return true;
}

//�������ӳ�ʱʱ�䣨��/s��
bool LXMysql::SetConnectTimeout(int sec)
{
	//cout << "LXMysql::SetConnectTimeout() success!" << endl;
	return Options(LX_OPT_CONNECT_TIMEOUT, &sec);
}

//�����Զ�������mysqlĬ��������flase
bool LXMysql::SetReconnect(bool ifReconnect)
{
	//cout << "LXMysql::SetReconnect() success!" << endl;
	return Options(LX_OPT_RECONNECT, &ifReconnect);
}

//�������ȡ��ʽ1 һ���Դ洢���н����
bool  LXMysql::StoreResult()
{
	//���ȼ��һ��mysql�Ƿ�Ϊ��
	if (!mysql)
	{
		cerr << "LXMysql::StoreResult failed! mysql is NULL!" << endl;
		return false;
	}
	//��ȡǰ������һ�ν����
	FreeResult();
	//��ȡ�����
	this->result = mysql_store_result(mysql);
	//�жϻ�ȡ���������Ƿ��������
	if (!this->result)
	{
		cerr << "LXMysql::StoreResult() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::StoreResult() success!" << endl;
	return true;
}

//�������ȡ��ʽ2 ��ʶ��ʼ��ȡ�������ͨ��Fetch�ӿ�ʵ�ʻ�ȡ�����
bool LXMysql::UseResult()
{
	//���ȼ��һ��mysql�Ƿ�Ϊ��
	if (!mysql)
	{
		cerr << "LXMysql::UseResult failed! mysql is NULL!" << endl;
		return false;
	}
	//��ȡǰ������һ�ν����
	FreeResult();
	//��ʶ��ʼ��ȡ�����
	result = mysql_use_result(mysql);
	if (!result)
	{
		cerr << "LXMysql::UseResult() failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}

	//cout << "LXMysql::UseResult() success!" << endl;
	return true;
}

//�����������ͷŽ����ռ�õĿռ�
void LXMysql::FreeResult()
{
	//�жϽ����������ʱִ���������
	if (result)
	{
		mysql_free_result(result);
		result = NULL;
	}
}

//�ӽ�����л�ȡһ������,���ݴ洢���Զ�����������LXData������������
std::vector<LXData> LXMysql::FetchRow()
{
	//����洢����������ÿ���ֶ����ݵ�LXData����
	std::vector<LXData> resultRow;
	//�жϵ�ǰ����Ľ�������Ƿ�Ϊ��
	if (!result)
	{
		return resultRow;  //�������Ϊ�գ��򷵻�һ���յ�std::vector<LXData>����
	}
	//�ӽ������ȡ��һ������
	MYSQL_ROW row = mysql_fetch_row(result);
	if (!row)
	{
		return resultRow;  //��������е�ǰ�������ݣ��򷵻�һ���յ�std::vector<LXData>����
	}
	//����ý�������ֶ�����
	int colNum = mysql_num_fields(result);
	//��ȡ������и�������ÿ���ֶ����ݵĴ�С
	unsigned long* lens = mysql_fetch_lengths(result);
	//������ȡ����һ��������ÿ���ֶε����ݣ�����std::vector<LXData>������
	for (int i = 0; i < colNum; i++)
	{
		LXData tempData;
		tempData.data = row[i];
		tempData.size = lens[i];
		//����mysql��API��ȡ��ǰ�н�����и��е���Ϣ
		auto field = mysql_fetch_field_direct(result, i);
		//��mysql��ö�ٱ�ʶǿתΪ�Զ����FIELD_TYPEö������
		tempData.type = (FIELD_TYPE)field->type;
		resultRow.push_back(tempData);
	}

	return resultRow;
}

//����insert sql���
std::string LXMysql::GetInsertSql(XDATA kv, std::string table)
{
	string sql = "";
	//�жϽӿڲ����Ƿ�Ϊ��
	if (kv.empty() || table.empty()) 
		return sql;
	//���sql��� ʾ����insert into `t_video` (`name`,`size`) values ('name1','1024')
	sql = "insert into `" + table + "` (";
	string keys = "";
	string vals = "";
	//���õ���������kv��ϸ��ֶ������Ӧ�ļ�ֵ
	for (XDATA::iterator ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		//����ֶ���
		keys += "`";
		keys += ptr->first;
		keys += "`,";
		//����ֶ�����Ӧ��ֵ
		vals += "'";
		vals += ptr->second.data;
		vals += "',";
	}
	//ȥ������Ķ���
	keys[keys.size() - 1] = ' ';
	vals[vals.size() - 1] = ' ';
	//�������sql���
	sql += keys;
	sql += ") values (";
	sql += vals;
	sql += ")";

	return sql;
}

//����Ƕ��������ݣ�ֻ������ִ���
bool LXMysql::Insert(XDATA kv, std::string table)
{
	//���ȼ��һ��mysql�Ƿ�Ϊ��
	if (!mysql)
	{
		cerr << "LXMysql::Insert failed! mysql is NULL!" << endl;
		return false;
	}
	//�����ڲ��ӿ�GetInsertSql����sql���
	string sql = GetInsertSql(kv, table);
	if (sql.empty())
	{
		cerr << "LXMysql::Insert failed! sql generated is empty!" << endl;
		return false;
	}
	//ִ��sql���
	if (!Query(sql.c_str()))
	{
		cerr << "LXMysql::Insert failed! sql execute wrong!" << endl;
		return false;
	}
	//���ݸ����Ӱ��������ж��Ƿ���insert���ִ�гɹ���������������sql�ɹ�ִ��������ţ�
	int affctedRows = mysql_affected_rows(mysql);
	if (affctedRows <= 0)
	{
		cerr << "LXMysql::Insert failed! mysql_affected_rows <= 0!" << endl;
		return false;
	}

	return true;
}

//�������������
bool LXMysql::InsertBin(XDATA kv, std::string table)
{
	//��������mysql_stmt_prepare�õ�sql���
	string sql = "";
	//�жϽӿڲ����Ƿ�Ϊ�գ�mysql�Ƿ�Ϊ��
	if (kv.empty() || table.empty() || !mysql)
		return false;
	//���mysql_stmt_prepare�õ�sql��� ʾ����insert into `t_video` (`name`,`size`) values (?,?)
	sql = "insert into `" + table + "` (";
	string keys = "";
	string vals = "";
	//�������ڰ��ֶε�MYSQL_BIND�ṹ�����飨���԰󶨵��ֶ��������޳����趨Ϊ256����
	MYSQL_BIND bind[256] = { 0 };
	int i = 0;
	//���õ���������kv��ϸ��ֶ������Ӧ�ļ�ֵ
	for (XDATA::iterator ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		//����ֶ���
		keys += "`";
		keys += ptr->first;
		keys += "`,";
		//����ֶ�����Ӧ��?���������Ӧ������
		vals += "?,";
		bind[i].buffer = (char*)ptr->second.data;
		bind[i].buffer_length = ptr->second.size;
		bind[i].buffer_type = (enum_field_types)ptr->second.type;
		i++;
	}
	//ȥ������Ķ���
	keys[keys.size() - 1] = ' ';
	vals[vals.size() - 1] = ' ';
	//�������sql���
	sql += keys;
	sql += ") values (";
	sql += vals;
	sql += ")";

	//����һ��stmt����
	MYSQL_STMT* stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		cerr << "LXMysql::InsertBin() mysql_stmt_init failed! The reason is: " << mysql_error(mysql) << endl;
		return false;
	}
	//Ԥ����sql���
	if (mysql_stmt_prepare(stmt, sql.c_str(), sql.length()) != 0)
	{
		//����stmt������ڴ���Դ
		mysql_stmt_close(stmt);
		cerr << "LXMysql::InsertBin() mysql_stmt_prepare failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return false;
	}
	//���ֶ�
	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		//����stmt������ڴ���Դ
		mysql_stmt_close(stmt);
		cerr << "LXMysql::InsertBin() mysql_stmt_bind_param failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return false;
	}
	//ִ��stmt
	if (mysql_stmt_execute(stmt) != 0)
	{
		//����stmt������ڴ���Դ
		mysql_stmt_close(stmt);
		cerr << "LXMysql::InsertBin() mysql_stmt_execute failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return false;
	}
	//����stmt������ڴ���Դ
	mysql_stmt_close(stmt);

	return true;
}

//���ɸ���һ�����ݵ�sql��䣨where����У��û�������ִ���Ҫ����where��
std::string LXMysql::GetUpdateSql(XDATA kv, std::string table, std::string where)
{
	//���sql��� ʾ����update `t_video` set `name`='update001',`size`='1000' where `id`='10'
	string sql = "";
	//�жϴ���ļ�ֵ��������Ƿ�Ϊ��
	if (kv.empty() || table.empty())
		return "";
	//ƴ��update����ǰ����
	sql = "update `";
	sql += table;
	sql += "` set ";
	//������ֵ��map����Ϻ���Ҫ���µ��ֶ��Լ���Ӧ����
	for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		sql += "`";
		sql += ptr->first;
		sql += "`='";
		sql += ptr->second.data;
		sql += "',";
	}
	//ȥ������Ķ���
	sql[sql.size() - 1] = ' ';
	//ƴ���û������where��䣬���ڶ�λ��Ҫ������������
	sql += " ";
	sql += where;

	return sql;
}

//���·Ƕ��������ݣ����ظ��µ�����������ʧ�ܷ���-1
int LXMysql::Update(XDATA kv, std::string table, std::string where)
{
	//���ȼ��һ��mysql�Ƿ�Ϊ��
	if (!mysql)
	{
		cerr << "LXMysql::Update failed! mysql is NULL!" << endl;
		return -1;
	}
	//�����ڲ��ӿ����ɸ������ݵ�sql���
	string sql = GetUpdateSql(kv, table, where);
	//�ж��Ƿ�ɹ�����sql���
	if (sql.empty())
	{
		cerr << "LXMysql::Update failed! GetUpdateSql failed!" << endl;
		return -1;
	}
	//ִ�и������ݵ�sql���
	if (!Query(sql.c_str()))
	{
		cerr << "LXMysql::Update failed! UpdateSql Query failed!" << endl;
		return -1;
	}

	//���ظ��µ�����
	return mysql_affected_rows(mysql);
}

//���¶��������ݣ����ظ��µ�����������ʧ�ܷ���-1
int LXMysql::UpdateBin(XDATA kv, std::string table, std::string where)
{
	//���ȼ��һ��mysql�Ƿ�Ϊ��
	if (!mysql)
	{
		cerr << "LXMysql::UpdateBin failed! mysql is NULL!" << endl;
		return -1;
	}
	//�жϴ���ļ�ֵ��������Ƿ�Ϊ��
	if (kv.empty() || table.empty())
	{
		cerr << "LXMysql::UpdateBin failed! kv or table name is empty!" << endl;
		return -1;
	}
	//���mysql_stmt_prepare�õ�sql��� ʾ����update `t_video` set `name`=?,`size`=? where `id` = 1
	string sql = "";
	sql = "update `";
	sql += table;
	sql += "` set ";
	//�������ڰ��ֶε�MYSQL_BIND�ṹ�����飨���԰󶨵��ֶ��������޳����趨Ϊ256����
	MYSQL_BIND bind[256] = { 0 };
	int i = 0;
	//���õ���������kv��ϸ��ֶ������Ӧ�ļ�ֵ
	for (auto ptr = kv.begin(); ptr != kv.end(); ptr++)
	{
		sql += "`";
		sql += ptr->first;
		sql += "`=?,";
		bind[i].buffer = (char*)ptr->second.data;
		bind[i].buffer_length = ptr->second.size;
		bind[i].buffer_type = (enum_field_types)ptr->second.type;
		i++;
	}
	//ȥ������Ķ���
	sql[sql.size() - 1] = ' ';
	sql += " ";
	sql += where;

	//����һ��stmt����
	MYSQL_STMT *stmt = mysql_stmt_init(mysql);
	if (!stmt)
	{
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_init failed! The reason is: " << mysql_error(mysql) << endl;
		return -1;
	}
	//Ԥ����sql���
	if (mysql_stmt_prepare(stmt, sql.c_str(), sql.length()) != 0)
	{
		mysql_stmt_close(stmt);
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_prepare failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return -1;
	}
	//Ϊ�ֶΰ�����
	if (mysql_stmt_bind_param(stmt, bind) != 0)
	{
		mysql_stmt_close(stmt);
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_bind_param failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return -1;
	}
	//ִ��stmt
	if (mysql_stmt_execute(stmt) != 0)
	{
		mysql_stmt_close(stmt);
		cerr << "LXMysql::UpdateBin failed! mysql_stmt_execute failed! The reason is: " << mysql_stmt_error(stmt) << endl;
		return -1;
	}
	//����stmt������ڴ���Դ
	mysql_stmt_close(stmt);

	//���ظ��µ�����
	return mysql_stmt_affected_rows(stmt);
}

//����ӿ� �����ֶ��ύ����
bool LXMysql::StartTransaction()
{
	//��־�������㣬�ع������᳷����sql������������ݿ�sql�������
	Query("START TRANSACTION");
	//Query("BEGIN");  //ͬ��Ҳ�Ǳ�־����������

	return Query("set autocommit=0");
}

//����ӿ� ��ԭ�Զ��ύ����
bool LXMysql::StopTransaction()
{
	return Query("set autocommit=1");
}

//����ӿ� �ύ�������
bool LXMysql::Commit()
{
	return Query("commit");
}

//����ӿ� �ع�������������ֻ�д�������ΪInnodb�����ݿ��֧��
bool LXMysql::Rollback() 
{
	return Query("rollback");
}

//���׽������ȡ�ӿ�,ֱ�ӷ���select�������Ľ������ÿһ�����ݣ�ÿ�ε���������һ�εĽ����
XROWS LXMysql::GetResult(const char *sql)
{
	//ÿ�ε���ǰ������һ�εĽ����
	FreeResult();
	//�������ڴ洢����ֵ��XROWS���󣬽��������ÿһ�����ݴ���������
	XROWS rows;
	//ִ��sql���
	if (!Query(sql))
		return rows;
	//�洢�����Ľ������this->result��
	if (!StoreResult())
		return rows;
	//��������е��������ݷ���XROWS������
	for (;;)
	{
		//��һ����ÿһ���ֶ����ݴ���vector<LXData>������
		auto row = FetchRow();
		//�����������ֶ�Ϊ�գ�˵���Ѿ�������������
		if (row.empty())break;
		//��һ�����ݹ��ɵ���������Ƕ������vector<std::vector<LXData>>��
		rows.push_back(row);
	}

	return rows;
}