#pragma once


template<class T>
class SeqList
{
public:
	//含参构造函数
	SeqList(const int capacity);
	//虚析构函数
	virtual ~SeqList();

public:
	//清空表数据
	void Clear();
	//获取表的长度
	int getLength();
	//获取表的容量
	int getCapacity();
	//插入一个节点
	bool Insert(int pos, T& node);
	//删除一个节点
	bool Delete(int pos);
	//获取一个节点
	T Get(int pos);
	//存储所有表节点对象的数组指针
	T* pArray;
private:
	//表容量
	int capacity;
	//当前表的长度
	int length;
	

};

