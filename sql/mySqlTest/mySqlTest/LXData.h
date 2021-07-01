#pragma once

#ifdef LXMYSQL_EXPORTS	
#define LXAPI __declspec(dllexport)//动态链接库项目调用
#else					
#define LXAPI __declspec(dllimport)//执行程序调用
#endif

#include <map>
#include <string>

//自定义枚举类型，供mysql_option操作调用
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

//自定义枚举类型，供MYSQL_BIND选择绑定类型
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
	//构造函数,传入整型指针可以直接调用构造函数生成一个LXData对象
	LXData(const int* data);

	//构造函数，传入字符串可以直接调用构造函数生成一个LXData对象
	LXData(const char* data = 0);

	//读取文件，内容写入到data中，文件大小存入size ,会在堆中申请data的空间，需要用Drop释放
	bool LoadFile(const char* filename);

	//保存文件
	bool SaveFile(const char* filename);

	//释放读取文件时在堆中申请的内存
	void Drop();

	//成员变量
	const char* data = 0;
	int size = 0;
	FIELD_TYPE type;
};

//别名为XDATA的map数据结构，方便生成插入和更新用的sql语句
typedef std::map<std::string, LXData> XDATA;

