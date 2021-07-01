#include "LXData.h"

#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

//���캯��,��������ָ�����ֱ�ӵ��ù��캯������һ��LXData����
LXData::LXData(const int* data)
{	
	//�ж�data�Ƿ�������
	if (!data)return;
	//Ϊ�ṹ���Ա��ֵ
	this->data = (const char*)data;
	this->size = sizeof(int);
	this->type = LX_TYPE_LONG;
}

//���캯���������ַ�������ֱ�ӵ��ù��캯������һ��LXData����
LXData::LXData(const char* data)
{
	//�ж�data�Ƿ�������
	if (!data)return;
	//Ϊ�ṹ���Ա��ֵ
	this->data = data;
	this->size = strlen(data);
	this->type = LX_TYPE_STRING;
}

//��ȡ�ļ�������д�뵽data�У��ļ���С����size ,���ڶ�������data�Ŀռ䣬��Ҫ��Drop�ͷ�
bool LXData::LoadFile(const char* filename)
{
	//���������ļ���Ϊ��
	if (!filename)
	{
		cerr << "LXData::LoadFile failed! filename is NULL!" << endl;
		return false;
	}
	//�����ļ������������ļ������а�
	fstream in(filename, ios::in | ios::binary);
	if (!in.is_open())
	{
		cerr << "LXData::LoadFile " << filename << " failed! file can not open!" << endl;
		return false;
	}
	//��ȡ�ļ���С
	in.seekg(0, ios::end);		//�ļ�ָ������ĩβ
	this->size = in.tellg();	//��ȡ�ļ���С
	in.seekg(0, ios::beg);		//�ļ�ָ���ƻؿ�ͷ
	if (this->size <= 0)
	{
		cerr << "LXData::LoadFile failed! file size <= 0!" << endl;
		return false;
	}
	//Ϊ�ļ���̬�����ڴ�
	this->data = new char[size];
	//��ȡ�ļ�ֱ���ļ�ָ��ָ��ĩβ
	int readed = 0;  //��ǵ�ǰ�Ѷ����٣����ڴ��ļ�������Ҫ�ֶ�ζ�ȡ
	while (!in.eof())
	{
		in.read((char*)data + readed, size - readed);
		if (in.gcount() > 0)
		{
			readed += in.gcount();
		}
		else
		{
			break;  //�������û�ж�������������
		}
	}
	//�ر��ļ�������
	in.close();
	this->type = LX_TYPE_BLOB;
	return true;
}

//�����ļ�
bool LXData::SaveFile(const char * filename)
{
	//�ж��ļ��Ƿ�������
	if (!this->data || this->size <= 0)
		return false;
	//�����ļ�����������ļ������а�
	fstream out(filename, ios::out | ios::binary);
	if (!out.is_open())
	{
		cerr << "LXData::SaveFile " << filename << " failed! file can not open!" << endl;
		return false;
	}
	//һ����д����������
	out.write(data, size);
	//�ر��ļ������
	out.close();
	//this->type = LX_TYPE_BLOB;
	return true;
}

//�ͷŶ�ȡ�ļ�ʱ�ڶ���������ڴ�
void LXData::Drop()
{
	if (this->data)
	{
		//�ͷ��ڴ棬����ָ���ÿ�
		delete this->data;
		this->data = NULL;
		//�����ļ��͵�LXData��С��0
		this->size = 0;
	}
}

//�ַ���ת�� UTF8=>GBK
std::string LXData::UTF8ToGBK()
{
	//�������ڴ洢ת�����ַ�����string����
	string re = "";
	//1 UFT8����=>Unicode����
	//����ת�����ִ��������ַ�����
	int len = MultiByteToWideChar(CP_UTF8,	//��ת���ִ����ַ��������ʽ
		0,									//0��ʾĬ�ϵ�ת����ʽ
		this->data,							//������ַ�������
		-1,									//������ַ�����С��-1��ʾ��'\0'���Զ������С��
		0,									//����ָ��������ݵ�ָ�룬��Ϊ�ò����ͳ���ַ�����������Ϊ0��ʾ�����
		0									//�������ʱ��Ӧ���ַ���������Ϊ�ò����ͳ���ַ�����������Ϊ0��ʾ�����
	);
	if (len <= 0)
		return re;
	//����洢�м�ת����Unicode�����wstring����
	wstring udata;
	//���ݼ�������ַ������������м�洢����wstring�Ĵ�С
	udata.resize(len);
	//��������UFT8����תΪUnicode����
	MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 Unicode����=>GBK����
	//����ת�����ִ��������ַ�����
	len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//ʧ��Ĭ������ַ�������Ϊ0��ζ���ڴ˲���\0�����ڴ˴��ض�
		0	//�Ƿ�ʹ��Ĭ����� 
	);
	if (len <= 0)
		return re;
	re.resize(len);
	//��������Unicode����תΪGBK����
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len, 0, 0);

	return re;
}

//�ַ���ת�� GBK=>UTF8
std::string LXData::GBKToUTF8()
{
	//�������ڴ洢ת�����ַ�����string����
	string re = "";
	//1 GBK����=>Unicode����
	//����ת���ִ��������ַ�����
	int len = MultiByteToWideChar(CP_ACP,	//��ת���ִ����ַ��������ʽ
		0,									//0��ʾĬ�ϵ�ת����ʽ
		data,								//������ַ�������
		-1,									//������ַ�����С��-1��ʾ��'\0'���Զ������С��
		0,									//����ָ��������ݵ�ָ�룬��Ϊ�ò����ͳ���ַ�����������Ϊ0��ʾ�����
		0									//�������ʱ��Ӧ���ַ���������Ϊ�ò����ͳ���ַ�����������Ϊ0��ʾ�����
	);
	if (len <= 0)
		return re;
	wstring udata;
	udata.resize(len);
	MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 Unicode����תUTF8����
	len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//ʧ��Ĭ������ַ�������Ϊ0��ζ���ڴ˲���\0�����ڴ˴��ض�
		0	//�Ƿ�ʹ��Ĭ����� 
	);
	if (len <= 0)
		return re;
	re.resize(len);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len, 0, 0);

	return re;
}