#include "SeqList.h"
#include <iostream>

using namespace std;

//含参构造函数
template<class T>
SeqList<T>::SeqList(const int capacity)
{
	//初始化表结构的基本参数
	this->capacity = capacity;
	this->length = 0;
	//根据用户设定容量大小和待存储的对象类型开辟内存，将首地址赋给pArray
	this->pArray = new T[capacity];
}

//虚析构函数
template<class T>
SeqList<T>::~SeqList()
{
	//判断表结构是否存在
	if (this->pArray == NULL)
		return;

	//释放存储对象区域的内存并将指针置空
	delete[] this->pArray;
	this->pArray = NULL;
	//将长度和容量清零
	this->capacity = 0;
	this->length = 0;
}

//清空表数据
template<class T>
void SeqList<T>::Clear()
{
	//判断表结构是否存在
	if (this->pArray == NULL)
		return;

	//将表长度置0
	this->length = 0;
}

//获取表的长度
template<class T>
int SeqList<T>::getLength()
{
	//判断表结构是否存在
	if (this->pArray == NULL)
	{
		cout << "getLength() failed! List is not exist!" << endl;
		return -1;
	}

	return this->length;
}

//获取表的容量
template<class T>
int SeqList<T>::getCapacity()
{
	//判断表结构是否存在
	if (this->pArray == NULL)
	{
		cout << "getCapacity() failed! List is not exist!" << endl;
		return -1;
	}

	return this->capacity;
}

//插入一个节点
template<class T>
bool SeqList<T>::Insert(int pos, T& node)
{
	//判断表结构是否存在,表是否已满，目标插入位置是否正常
	if (this->pArray == NULL || this->length == this->capacity || pos < 0 || pos > this->length)
	{
		cout << "Insert() failed!" << endl;
		return false;
	}
	//从插入位置至最后一个节点处，所有节点位置后移
	for (int i = this->length; i > pos; i--)
	{
		this->pArray[i] = this->pArray[i - 1];
	}
	//将pos处设为新节点
	this->pArray[pos] = node;
	//表长度加1
	this->length++;

	return true;
}

//删除一个节点
template<class T>
bool SeqList<T>::Delete(int pos)
{
	//判断表结构是否存在,是否有数据，目标删除位置是否正常
	if (this->pArray == NULL || this->length == 0 || pos < 0 || pos > length - 1)
	{
		cout << "Delete() failed!" << endl;
		return false;
	}
	//从删除位置的后一个节点至最后一个节点处，所有节点位置前移
	for (int i = pos; i < length - 1; i++)
	{
		this->pArray[i] = this->pArray[i + 1];
	}
	//表长度减1
	this->length--;

	return true;
}

//获取一个节点
template<class T>
T SeqList<T>::Get(int pos)
{
	//根据模板类型创建一个节点对象
	T node;
	//判断表结构是否存在,是否有数据，目标获取位置是否正常
	if (this->pArray == NULL || this->length == 0 || pos < 0 || pos >= length)
	{
		cout << "Get() failed!" << endl;
		return node;
	}
	//直接根据位置索引	
	node = pArray[pos];

	return node;
}
