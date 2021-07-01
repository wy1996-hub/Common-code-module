#pragma once

#ifdef LXMYSQL_EXPORTS	
#define LXAPI __declspec(dllexport)//��̬���ӿ���Ŀ����
#else					
#define LXAPI __declspec(dllimport)//ִ�г������
#endif

#include <map>
#include <string>

//�Զ���ö�����ͣ���mysql_option��������
enum LX_OPT {
	LX_OPT_CONNECT_TIMEOUT,
	LX_OPT_COMPRESS,
	LX_OPT_NAMED_PIPE,
	LX_INIT_COMMAND,
	LX_READ_DEFAULT_FILE,
	LX_READ_DEFAULT_GROUP,
	LX_SET_CHARSET_DIR,
	LX_SET_CHARSET_NAME,
	LX_OPT_LOCAL_INFILE,
	LX_OPT_PROTOCOL,
	LX_SHARED_MEMORY_BASE_NAME,
	LX_OPT_READ_TIMEOUT,
	LX_OPT_WRITE_TIMEOUT,
	LX_OPT_USE_RESULT,
	LX_REPORT_DATA_TRUNCATION,
	LX_OPT_RECONNECT,
	LX_PLUGIN_DIR,
	LX_DEFAULT_AUTH,
	LX_OPT_BIND,
	LX_OPT_SSL_KEY,
	LX_OPT_SSL_CERT,
	LX_OPT_SSL_CA,
	LX_OPT_SSL_CAPATH,
	LX_OPT_SSL_CIPHER,
	LX_OPT_SSL_CRL,
	LX_OPT_SSL_CRLPATH,
	LX_OPT_CONNECT_ATTR_RESET,
	LX_OPT_CONNECT_ATTR_ADD,
	LX_OPT_CONNECT_ATTR_DELETE,
	LX_SERVER_PUBLIC_KEY,
	LX_ENABLE_CLEARTEXT_PLUGIN,
	LX_OPT_CAN_HANDLE_EXPIRED_PASSWORDS,
	LX_OPT_MAX_ALLOWED_PACKET,
	LX_OPT_NET_BUFFER_LENGTH,
	LX_OPT_TLS_VERSION,
	LX_OPT_SSL_MODE,
	LX_OPT_GET_SERVER_PUBLIC_KEY,
	LX_OPT_RETRY_COUNT,
	LX_OPT_OPTIONAL_RESULTSET_METADATA,
	LX_OPT_SSL_FIPS_MODE
};

//�Զ���ö�����ͣ���MYSQL_BINDѡ�������
enum FIELD_TYPE {
	LX_TYPE_DECIMAL,
	LX_TYPE_TINY,
	LX_TYPE_SHORT,
	LX_TYPE_LONG,
	LX_TYPE_FLOAT,
	LX_TYPE_DOUBLE,
	LX_TYPE_NULL,
	LX_TYPE_TIMESTAMP,
	LX_TYPE_LONGLONG,
	LX_TYPE_INT24,
	LX_TYPE_DATE,
	LX_TYPE_TIME,
	LX_TYPE_DATETIME,
	LX_TYPE_YEAR,
	LX_TYPE_NEWDATE, /**< Internal to MySQL. Not used in protocol */
	LX_TYPE_VARCHAR,
	LX_TYPE_BIT,
	LX_TYPE_TIMESTAMP2,
	LX_TYPE_DATETIME2, /**< Internal to MySQL. Not used in protocol */
	LX_TYPE_TIME2,     /**< Internal to MySQL. Not used in protocol */
	LX_TYPE_JSON = 245,
	LX_TYPE_NEWDECIMAL = 246,
	LX_TYPE_ENUM = 247,
	LX_TYPE_SET = 248,
	LX_TYPE_TINY_BLOB = 249,
	LX_TYPE_MEDIUM_BLOB = 250,
	LX_TYPE_LONG_BLOB = 251,
	LX_TYPE_BLOB = 252,
	LX_TYPE_VAR_STRING = 253,
	LX_TYPE_STRING = 254,
	LX_TYPE_GEOMETRY = 255
};

struct LXAPI LXData
{
	//���캯��,��������ָ�����ֱ�ӵ��ù��캯������һ��LXData����
	LXData(const int* data);

	//���캯���������ַ�������ֱ�ӵ��ù��캯������һ��LXData����
	LXData(const char* data = 0);

	//��ȡ�ļ�������д�뵽data�У��ļ���С����size ,���ڶ�������data�Ŀռ䣬��Ҫ��Drop�ͷ�
	bool LoadFile(const char* filename);

	//�����ļ�
	bool SaveFile(const char* filename);

	//�ͷŶ�ȡ�ļ�ʱ�ڶ���������ڴ�
	void Drop();

	//��Ա����
	const char* data = 0;
	int size = 0;
	FIELD_TYPE type;
};

//����ΪXDATA��map���ݽṹ���������ɲ���͸����õ�sql���
typedef std::map<std::string, LXData> XDATA;

