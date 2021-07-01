#pragma once
#include <QString>
#include <QSqlDatabase>
#include <map>
#include <string>
#include <vector>

// ����Ԫ�������ʱ����Ľṹ��
struct DETECT_INFO_INSERT;

// ��ѯԪ�������ʱ�����Ľṹ��
struct DETECT_INFO_REQUIRE;


// AOI��Ŀ���ݿ���
class JS_DATABASE
{
public:
	// ���캯��
	JS_DATABASE(QString connection_name, QString host_IP, QString user_name, QString passward, QString database_name);

	// ��������
	~JS_DATABASE();

public:
	// �������ݿ�����
	bool connect();

	// ����������
	bool insert_detect_result(DETECT_INFO_INSERT& result_info);

	// ��ѯ����ʱ����£�ĳ����ĳ��Ԫ���������Ϣ
	bool inquire_detect_result(QString timestamp, QString board, QString ele_index, DETECT_INFO_REQUIRE& result_info);

	// ���ָ�������ݱ�
	bool truncate_table(QString table_name);

public:
	// ��ȡʱ������� ʾ����2020/08/29 16:59:28
	static QString get_timestamp_now();

private:
	// ������Ӷ�Ӧ����������map��ʼ��
	void defect_index_map_init();

	// ������Ӷ�Ӧ���ֵ������map��ʼ��
	void value_num_map_init();


public:
	// ���ݿ����
	QSqlDatabase db;
	// ���ݿ��������ƣ��Զ��壬�������֣�
	QString connection_name;
	// Ŀ�����ݿ���������IP
	QString host_IP;
	// ��¼Ŀ�����ݿ�ʱ���û���
	QString user_name;
	// ��¼����
	QString passward;
	// Ŀ�����ݿ�����
	QString database_name;

private:
	// ������Ӷ�Ӧ����������map
	std::map<int, QString> defect_index_map;
	// ������Ӷ�Ӧ���ֵ������map
	std::map<int, int> value_num_map;

private:
	// ��ʶ���ݿ������Ƿ���Ч
	bool is_driver_valid = false;
	// ��ʶ���ݿ��Ƿ��Ѿ�����
	bool is_connected = false;

};

// ����Ԫ�������ʱ����Ľṹ��
struct DETECT_INFO_INSERT
{
	QString board_name;		// boom�����ƣ�ÿ����Ψһ��ʶ��
	QString ele_name;		// Ԫ����
	QString ele_index;		// Ԫ�����루ÿ����ϵ�ÿ��Ԫ����Ψһ��ʶ��
	QString timestamp;		// ʱ��������ָ�ʽ: 2020/08/29 16:59:28 || 2020-08-29 16:59:28 || 20200829165928��
							// ���Ե���JS_DATABASE�ľ�̬��Ա����get_timestamp_now��ȡ��ʽΪ2020/08/29 16:59:28��ʱ���
	bool is_ng;				// �Ƿ�NG��ʶλ������һ��������Ӽ��ΪNG���Ԫ��NG��ʶΪtrue��
	QString ng_type;		// Ԫ�������ȱ�����ͱ�ʶ�ִ������ַ�'0'��'1'��ɣ�������ȱ������һ�£��ݶ�Ϊ24
	QString ng_triger;		// Ԫ�������NG��Ӧȱ�ݵı�ʶ�ִ������ַ�'0'��'1'��ɣ�������ȱ������һ�£��ݶ�Ϊ24
	QString pic_path;		// ���ͼ�洢��·����·���ָ���Ϊ4����б��'\\\\'��ʾ����"D:\\\\test\\\\pic\\\\test.bmp"��
	int loc_x;				// ���ͼ������ͼ�е����Ͻǵ�x����
	int loc_y;				// ���ͼ������ͼ�е����Ͻǵ�y����
	int width;				// ���ͼ�Ŀ��
	int height;				// ���ͼ�ĸ߶�
	std::vector<QString> vec_values;  // Ԫ�������Ӽ������ֵ�����ֵ�����˳��Ӧ����ng_type�д���������1λ��Ӧ�ļ���������ֵ��
};

// ��ѯԪ�������ʱ�����Ľṹ��
struct DETECT_INFO_REQUIRE
{
	QString ele_name;		// Ԫ����
	bool b_isNG;			// ���ͼ��NG�ж����
	QString ng_triger;		// Ԫ�����NG�ı�ʶ�ִ�
	std::string pic_path;	// ���ͼ�洢��·��
	int loc_x;				// ���ͼ������ͼ�е����Ͻǵ�x����
	int loc_y;				// ���ͼ������ͼ�е����Ͻǵ�y����
	int width;				// ���ͼ�Ŀ��
	int height;				// ���ͼ�ĸ߶�
	std::map<int, QString> detect_value_map;  // ���м�������������
};
