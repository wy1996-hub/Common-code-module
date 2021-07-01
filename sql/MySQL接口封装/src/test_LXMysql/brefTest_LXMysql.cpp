#include <iostream>
#include "LXMysql.h"

using namespace std;

int main(int argc, char *argv[])
{
	//定义一个自定义的LXMysql对象
	LXMysql my;

	bool ifOk = false;
	//1 初始化MySQL API
	my.Init();

	//2 连接前进行MySQL的option配置
	//设置连接超时等待时长为3秒
	my.SetConnectTimeout(3);
	//设置自动重连
	my.SetReconnect(true);

	//3 建立数据库连接,如果之前没有调用LXMysql::Init()，内部会自动调用
	my.Connect("127.0.0.1", "root", "qq489632250.", "wang");

	//4 执行sql语句创建表
	string sql = "";
	sql = "CREATE TABLE IF NOT EXISTS `t_video`\
			(`id` INT AUTO_INCREMENT,\
			`name` VARCHAR(1024) ,\
			`data` BLOB,\
			`size` INT,\
			PRIMARY KEY(`id`))";
	my.Query(sql.c_str());

	//创建表后清空数据，将自增id置1
	sql = "truncate `t_video`";
	my.Query(sql.c_str());

	//5 测试插入数据
	//插入非二进制数据
	XDATA normalData;
	normalData.insert(make_pair("name", LXData("getSqlTest")));	 //向map数据结构中存值的标准方式
	normalData.insert(make_pair("size", LXData("1024")));
	//测试sql语句生成接口，传入键值对map数据结构、目标插入表名
	cout << my.GetInsertSql(normalData, "t_video") << endl;
	my.Insert(normalData, "t_video");
	//插入一个二进制数据
	XDATA binData;
	LXData binaryFile;
	binaryFile.LoadFile("mysql.jpg");
	binData["name"] = "mysql_new.jpg";	//向map数据结构中存值的便捷方式
	binData["data"] = binaryFile;
	binData["size"] = &binaryFile.size;
	my.InsertBin(binData, "t_video");
	//二进制文件型的LXData数据在载入文件后存在堆空间申请问题，需要用Drop函数进行释放
	binaryFile.Drop();

	//6 测试利用事务插入数据
	XDATA data3;
	data3["name"] = "trans001";
	data3["size"] = "1024";
	//开启事务（设置手动提交事务）
	my.StartTransaction();
	my.Insert(data3, "t_video");
	//回滚,前面插入的trans001应该失败
	my.Rollback();
	data3["name"] = "trans002";
	my.Insert(data3, "t_video");
	data3["name"] = "trans003";
	my.Insert(data3, "t_video");
	//提交事务
	my.Commit();
	//关闭事务（还原自动提交事务）
	my.StopTransaction();

	//7 测试更新数据
	//Update接口修改非二进制数据 修改id=1的name
	XDATA udata;
	udata["name"] = "updateTest";
	cout << "my.Update rows = " << my.Update(udata, "t_video", "where id=1") << endl;
	//UpdateBin接口修改二进制数据 修改id=3 的data 图标
	XDATA udata2;
	LXData file2;
	file2.LoadFile("mysql2.jpg");
	udata2["data"] = file2;
	udata2["size"] = &file2.size;
	cout << "my.UpdateBin rows = " << my.UpdateBin(udata2, "t_video", "where id=2") << endl;
	file2.Drop();

	//8 结果集获取StoreResult
	//select语句产生结果集
	sql = "select * from `t_video`";
	my.Query(sql.c_str());
	//测试StoreResult获取结果集,获取到的结果集存储在成员属性my.result中（UseResult开发时已测试，测试方法一致）
	my.StoreResult();

	//9 从结果集中获取内容
	for (;;)
	{
		//从结果集中获取一行内容
		auto row = my.FetchRow();
		//判断返回的容器中是否有内容
		if (row.size() == 0) break;
		//将第二行的二进制图片存储下来
		row[2].SaveFile(row[1].data);
		//打印当前行中每个字段对应的内容
		for (int i = 0; i < row.size(); i++)
		{
			//判断该字段的内容是否为空
			if (row[i].data)
			{
				if (row[i].type == LX_TYPE_BLOB)  //二进制文件用cout打印存在问题
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
		cout << endl;  //打印完一行数据做一次换行	
	}

	//清理结果集
	my.FreeResult();

	//10 测试字符集转换问题 主要测试数据插入与读取时，UTF8与GBK格式之间的转化问题
	cout << "========================开始测试字符集========================" << endl;
	//创建一张UTF8编码格式、字符集排序方式为utf8_bin的表`t_utf8`
	sql = "CREATE TABLE IF NOT EXISTS `t_utf8` \
		(`id` INT AUTO_INCREMENT,	\
		`name` VARCHAR(1024) CHARACTER SET utf8 COLLATE utf8_bin,\
		PRIMARY KEY(`id`))";
	my.Query(sql.c_str());
	//每次运行前清空表`t_utf8`中的数据
	my.Query("TRUNCATE t_utf8");
	//指定客户端和服务器之间传递字符的编码规则为UTF8
	my.Query("set names utf8");
	{
		XDATA data;
		data["name"] = u8"测试的UTF中文";  //此时表格只接受UTF8格式数据，vs2015默认文件以GBK方式存储，需要添加u8前缀才能插入成功
		my.Insert(data, "t_utf8");
	}
	//创建一张GBK编码格式、字符集排序方式为gbk_bin的表`t_gbk`
	sql = "CREATE TABLE IF NOT EXISTS `t_gbk` \
		(`id` INT AUTO_INCREMENT,	\
		`name` VARCHAR(1024) CHARACTER SET gbk COLLATE gbk_bin,\
		PRIMARY KEY(`id`))";
	my.Query(sql.c_str());
	//清空数据
	my.Query("TRUNCATE t_gbk");
	//指定与mysql处理的字符集
	my.Query("set names gbk");
	{
		XDATA data;
		LXData tmp = u8"测试的GBK中文";
		string gbk = tmp.UTF8ToGBK();  //如果直接插入UTF8编码的数据进入GBK编码的表中会出现乱码
		data["name"] = gbk.c_str();
		my.Insert(data, "t_gbk");
	}
	//测试从GBK格式的表中获取数据
	cout << "==== Print names gbk string ==== " << endl;
	my.Query("set names gbk");
	my.Query("select * from t_gbk");
	my.StoreResult();
	for (;;)
	{
		//获取一行数据
		auto row = my.FetchRow();
		if (row.size() == 0)break;
		cout << "id:" << row[0].data << " name:" << row[1].data << endl;
	}
	my.FreeResult();
	//测试从UTF8格式的表中获取数据
	cout << "==== Print utf-8 string ==== " << endl;
	my.Query("set names utf8");
	my.Query("select * from t_utf8");
	my.StoreResult();
	for (;;)
	{
		//获取一行数据
		auto row = my.FetchRow();
		if (row.size() == 0)break;
		cout << "id:" << row[0].UTF8ToGBK() << " name:" << row[1].UTF8ToGBK() << endl;
	}
	my.FreeResult();

	//测试简易结果集获取接口
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

	//测试自动重连
	//for (;;)
	//{
	//	cout << my.Query(sql.c_str()) << flush;
	//}


	//清理MySQL占用的所有资源
	my.Close();

	cout << endl << "test_LXMysql run success!" << endl;
	getchar();
	return 0;
}