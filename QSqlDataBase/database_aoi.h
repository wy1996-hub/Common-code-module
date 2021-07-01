#pragma once
#include <QString>
#include <QSqlDatabase>
#include <map>
#include <string>
#include <vector>

// 插入元件检测结果时传入的结构体
struct DETECT_INFO_INSERT;

// 查询元件检测结果时传出的结构体
struct DETECT_INFO_REQUIRE;


// AOI项目数据库类
class JS_DATABASE
{
public:
	// 构造函数
	JS_DATABASE(QString connection_name, QString host_IP, QString user_name, QString passward, QString database_name);

	// 析构函数
	~JS_DATABASE();

public:
	// 建立数据库连接
	bool connect();

	// 插入结果数据
	bool insert_detect_result(DETECT_INFO_INSERT& result_info);

	// 查询具体时间戳下，某块板的某个元件检测结果信息
	bool inquire_detect_result(QString timestamp, QString board, QString ele_index, DETECT_INFO_REQUIRE& result_info);

	// 清空指定的数据表
	bool truncate_table(QString table_name);

public:
	// 获取时间戳函数 示例：2020/08/29 16:59:28
	static QString get_timestamp_now();

private:
	// 检测算子对应表名的索引map初始化
	void defect_index_map_init();

	// 检测算子对应输出值数量的map初始化
	void value_num_map_init();


public:
	// 数据库对象
	QSqlDatabase db;
	// 数据库连接名称（自定义，用于区分）
	QString connection_name;
	// 目标数据库所属主机IP
	QString host_IP;
	// 登录目标数据库时的用户名
	QString user_name;
	// 登录密码
	QString passward;
	// 目标数据库名称
	QString database_name;

private:
	// 检测算子对应表名的索引map
	std::map<int, QString> defect_index_map;
	// 检测算子对应输出值数量的map
	std::map<int, int> value_num_map;

private:
	// 标识数据库驱动是否有效
	bool is_driver_valid = false;
	// 标识数据库是否已经连接
	bool is_connected = false;

};

// 插入元件检测结果时传入的结构体
struct DETECT_INFO_INSERT
{
	QString board_name;		// boom板名称（每块板的唯一标识）
	QString ele_name;		// 元件名
	QString ele_index;		// 元件编码（每块板上的每个元件的唯一标识）
	QString timestamp;		// 时间戳（三种格式: 2020/08/29 16:59:28 || 2020-08-29 16:59:28 || 20200829165928）
							// 可以调用JS_DATABASE的静态成员函数get_timestamp_now获取格式为2020/08/29 16:59:28的时间戳
	bool is_ng;				// 是否NG标识位（任意一个检测算子检出为NG则该元件NG标识为true）
	QString ng_type;		// 元件需检查的缺陷类型标识字串，由字符'0'，'1'组成，长度与缺陷数量一致，暂定为24
	QString ng_triger;		// 元件检出有NG对应缺陷的标识字串，由字符'0'，'1'组成，长度与缺陷数量一致，暂定为24
	QString pic_path;		// 结果图存储的路径（路径分隔符为4个倒斜杠'\\\\'，示例："D:\\\\test\\\\pic\\\\test.bmp"）
	int loc_x;				// 结果图在整板图中的左上角点x坐标
	int loc_y;				// 结果图在整板图中的左上角点y坐标
	int width;				// 结果图的宽度
	int height;				// 结果图的高度
	std::vector<QString> vec_values;  // 元件经算子检测的输出值（输出值的添加顺序应该是ng_type中从左往右置1位对应的检测算子输出值）
};

// 查询元件检测结果时传出的结构体
struct DETECT_INFO_REQUIRE
{
	QString ele_name;		// 元件名
	bool b_isNG;			// 结果图的NG判定结果
	QString ng_triger;		// 元件检出NG的标识字串
	std::string pic_path;	// 结果图存储的路径
	int loc_x;				// 结果图在整板图中的左上角点x坐标
	int loc_y;				// 结果图在整板图中的左上角点y坐标
	int width;				// 结果图的宽度
	int height;				// 结果图的高度
	std::map<int, QString> detect_value_map;  // 进行检测的算子输出结果
};
