#include "LXData.h"

#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

//构造函数,传入整型指针可以直接调用构造函数生成一个LXData对象
LXData::LXData(const int* data)
{	
	//判断data是否有数据
	if (!data)return;
	//为结构体成员赋值
	this->data = (const char*)data;
	this->size = sizeof(int);
	this->type = LX_TYPE_LONG;
}

//构造函数，传入字符串可以直接调用构造函数生成一个LXData对象
LXData::LXData(const char* data)
{
	//判断data是否有数据
	if (!data)return;
	//为结构体成员赋值
	this->data = data;
	this->size = strlen(data);
	this->type = LX_TYPE_STRING;
}

//读取文件，内容写入到data中，文件大小存入size ,会在堆中申请data的空间，需要用Drop释放
bool LXData::LoadFile(const char* filename)
{
	//如果传入的文件名为空
	if (!filename)
	{
		cerr << "LXData::LoadFile failed! filename is NULL!" << endl;
		return false;
	}
	//创建文件输入流，与文件名进行绑定
	fstream in(filename, ios::in | ios::binary);
	if (!in.is_open())
	{
		cerr << "LXData::LoadFile " << filename << " failed! file can not open!" << endl;
		return false;
	}
	//获取文件大小
	in.seekg(0, ios::end);		//文件指针移至末尾
	this->size = in.tellg();	//获取文件大小
	in.seekg(0, ios::beg);		//文件指针移回开头
	if (this->size <= 0)
	{
		cerr << "LXData::LoadFile failed! file size <= 0!" << endl;
		return false;
	}
	//为文件动态申请内存
	this->data = new char[size];
	//读取文件直到文件指针指向末尾
	int readed = 0;  //标记当前已读多少，对于大文件可能需要分多次读取
	while (!in.eof())
	{
		in.read((char*)data + readed, size - readed);
		if (in.gcount() > 0)
		{
			readed += in.gcount();
		}
		else
		{
			break;  //如果本次没有读到数据则跳出
		}
	}
	//关闭文件输入流
	in.close();
	this->type = LX_TYPE_BLOB;
	return true;
}

//保存文件
bool LXData::SaveFile(const char * filename)
{
	//判断文件是否有内容
	if (!this->data || this->size <= 0)
		return false;
	//创建文件输出流，与文件名进行绑定
	fstream out(filename, ios::out | ios::binary);
	if (!out.is_open())
	{
		cerr << "LXData::SaveFile " << filename << " failed! file can not open!" << endl;
		return false;
	}
	//一次性写完所有内容
	out.write(data, size);
	//关闭文件输出流
	out.close();
	//this->type = LX_TYPE_BLOB;
	return true;
}

//释放读取文件时在堆中申请的内存
void LXData::Drop()
{
	if (this->data)
	{
		//释放内存，并将指针置空
		delete this->data;
		this->data = NULL;
		//将该文件型的LXData大小置0
		this->size = 0;
	}
}

//字符集转化 UTF8=>GBK
std::string LXData::UTF8ToGBK()
{
	//定义用于存储转换后字符串的string对象
	string re = "";
	//1 UFT8编码=>Unicode编码
	//计算转换后字串包含的字符个数
	int len = MultiByteToWideChar(CP_UTF8,	//待转换字串的字符集编码格式
		0,									//0表示默认的转换方式
		this->data,							//输入的字符串内容
		-1,									//输入的字符串大小（-1表示找'\0'，自动计算大小）
		0,									//配置指向输出内容的指针，因为该步骤仅统计字符个数，配置为0表示不输出
		0									//配置输出时对应的字符个数，因为该步骤仅统计字符个数，配置为0表示不输出
	);
	if (len <= 0)
		return re;
	//定义存储中间转换的Unicode编码的wstring对象
	wstring udata;
	//根据计算出的字符个数，调整中间存储对象wstring的大小
	udata.resize(len);
	//将数据由UFT8编码转为Unicode编码
	MultiByteToWideChar(CP_UTF8, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 Unicode编码=>GBK编码
	//计算转换后字串包含的字符个数
	len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//失败默认替代字符，配置为0意味着在此插入\0，即在此处截断
		0	//是否使用默认替代 
	);
	if (len <= 0)
		return re;
	re.resize(len);
	//将数据由Unicode编码转为GBK编码
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len, 0, 0);

	return re;
}

//字符集转化 GBK=>UTF8
std::string LXData::GBKToUTF8()
{
	//定义用于存储转换后字符串的string对象
	string re = "";
	//1 GBK编码=>Unicode编码
	//计算转换字串包含的字符个数
	int len = MultiByteToWideChar(CP_ACP,	//待转换字串的字符集编码格式
		0,									//0表示默认的转换方式
		data,								//输入的字符串内容
		-1,									//输入的字符串大小（-1表示找'\0'，自动计算大小）
		0,									//配置指向输出内容的指针，因为该步骤仅统计字符个数，配置为0表示不输出
		0									//配置输出时对应的字符个数，因为该步骤仅统计字符个数，配置为0表示不输出
	);
	if (len <= 0)
		return re;
	wstring udata;
	udata.resize(len);
	MultiByteToWideChar(CP_ACP, 0, data, -1, (wchar_t*)udata.data(), len);

	//2 Unicode编码转UTF8编码
	len = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, 0, 0,
		0,	//失败默认替代字符，配置为0意味着在此插入\0，即在此处截断
		0	//是否使用默认替代 
	);
	if (len <= 0)
		return re;
	re.resize(len);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)udata.data(), -1, (char*)re.data(), len, 0, 0);

	return re;
}