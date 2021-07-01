#pragma once

//链表中的节点，包含指向下一个节点的指针
template<class T>
struct LinkListNode
{
	//记录下一个节点指针域的位置（与节点头部重合）
	LinkListNode* next;
	//节点中存放的模板对象
	T node;
};

template<class T>
class LinkList
{
public:
	//构造函数
	LinkList();
	//虚析构函数（清理表节点与头部节点）
	virtual ~LinkList();

public:
	//清空表数据
	void Clear();
	//获取表的长度
	int getLength();
	//插入一个节点
	bool Insert(int pos, T& node);
	//删除一个节点
	bool Delete(int pos);
	//获取一个节点
	T Get(int pos);

public:
	//链表的头部节点
	LinkListNode<T>* listHead;
	//链表的长度
	int length;

};

