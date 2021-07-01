#pragma once

#include <vector>
#include "LXData.h"

//在头文件中声明mysql中用到的结构体，避免头文件依赖项过多，起到屏蔽作用
struct MYSQL;
struct MYSQL_RES;

class LXAPI LXMysql
{
public:
	//初始化MySQL API
	bool Init();

	//清理MySQL占用的所有资源
	void Close();

	//mysql参数设定 Connect之前调用
	bool Options(LX_OPT opt, const void* arg);

	//数据库连接（不考虑线程安全） 设置flag标志位为1可以支持一次执行多条sql语句
	bool Connect(const char* host, const char* user, const char* pass, const char* db, unsigned short port = 3306, unsigned long flag = 0);

	//执行sql语句  如果采用默认sqllen=0 则通过strlen获取字符长度（内部用mysql_real_query执行sql语句）
	bool Query(const char* sql, unsigned long sqllen = 0);

	//设置连接超时时间（秒/s）
	bool SetConnectTimeout(int sec);

	//设置自动重连，mysql默认配置是flase
	bool SetReconnect(bool ifReconnect = true);

	//结果集获取方式1 一次性存储所有结果集
	bool StoreResult();

	//结果集获取方式2 标识开始获取结果集，通过Fetch接口实际获取结果集
	bool UseResult();

	//清理结果集，释放结果集占用的空间
	void FreeResult();

	//从结果集中获取一行数据,数据存储在自定义数据类型LXData的容器数组中
	std::vector<LXData> FetchRow();

	//生成插入一条非二进制数据的sql语句
	std::string GetInsertSql(XDATA kv, std::string table);

	//插入非二进制数据（只插入简单字串）
	bool Insert(XDATA kv, std::string table);

	//插入二进制数据
	bool InsertBin(XDATA kv, std::string table);

	//生成更新一行数据的sql语句（where语句中，用户传入的字串中要包含where）
	std::string GetUpdateSql(XDATA kv, std::string table, std::string where);

	//更新非二进制数据，返回更新的行数，更新失败返回-1
	int Update(XDATA kv, std::string table, std::string where);

	//更新二进制数据，返回更新的行数，更新失败返回-1
	int UpdateBin(XDATA kv, std::string table, std::string where);

	//事务接口 设置手动提交事务
	bool StartTransaction();

	//事务接口 还原自动提交事务
	bool StopTransaction();

	//事务接口 提交事务操作
	bool Commit();

	//事务接口 回滚操作（撤销）只有存贮引擎为Innodb的数据库才支持
	bool Rollback();

	//简易结果集获取接口,直接返回select语句产生的结果集中每一行数据，每次调用清理上一次的结果集
	XROWS GetResult(const char *sql);

protected:
	//mysql上下文（与指定数据库建立连接后，存储关于该数据库的所有信息）
	MYSQL* mysql = 0;

	//获取到的结果集
	MYSQL_RES* result = 0;

};

