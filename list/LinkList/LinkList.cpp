#include "LinkList.h"
#include <iostream>

using namespace std;

//构造函数
template<class T>
LinkList<T>::LinkList()
{
	//初始化表结构的基本参数
	this->length = 0;
	//创建头部节点对象，初始化将头部节点指针域置空
	this->listHead = new LinkListNode<T>;
	this->listHead->next = NULL;
}

//虚析构函数（清理表节点与头部节点）
template<class T>
LinkList<T>::~LinkList()
{
	//清理表中各节点的数据
	Clear();
	//清理头部节点
	delete this->listHead;
	this->listHead = NULL;
	this->length = 0;
}

//清空表数据
template<class T>
void LinkList<T>::Clear()
{
	//判断表中是否有数据
	if (this->length <= 0)
		return;

	//头部节点保留
	LinkListNode<T>* curNode = this->listHead->next;
	//清空链表中所有节点的内容
	for (int i = 0; i < this->length; i++)
	{
		//缓存当前节点
		LinkListNode<T>* tmpNode = curNode;
		//头部节点后移
		curNode = curNode->next;
		//清理当前节点的内存
		delete tmpNode;
	}
	//链表长度清零
	this->length = 0;
	//头部节点指针置空
	this->listHead->next = NULL;
}

//获取表的长度
template<class T>
int LinkList<T>::getLength()
{
	return this->length;
}

//插入一个节点
template<class T>
bool LinkList<T>::Insert(int pos, T& node)
{
	//判断目标插入位置是否正常
	if (pos < 0 || pos > this->length)
	{
		cout << "Insert() failed!" << endl;
		return false;
	}
	//将插入对象打包成节点,初始化指针域为空
	LinkListNode<T>* newNode = new LinkListNode<T>;
	newNode->node = node;
	newNode->next = NULL;
	//找到目标插入位置的前一个节点
	LinkListNode<T>* curNode = listHead;
	for (int i = 0; i < pos; i++)
	{
		curNode = listHead->next;
	}
	//将curNode的指针域内容赋给newNode
	newNode->next = curNode->next;
	//将curNode的指针域指向newNode
	curNode->next = newNode;
	//链表长度加1
	this->length++;

	return true;
}

//删除一个节点
template<class T>
bool LinkList<T>::Delete(int pos)
{
	//判断目标删除位置是否正常
	if (pos < 0 || pos > (this->length - 1))
	{
		cout << "Delete() failed!" << endl;
		return false;
	}
	//找到目标删除位置的前一个节点
	LinkListNode<T>* curNode = listHead;
	for (int i = 0; i < pos; i++)
	{
		curNode = listHead->next;
	}
	//缓存待删除节点
	LinkListNode<T>* delNode = curNode->next;
	//将待删除节点的前一个节点指向后一个节点
	curNode->next = delNode->next;
	//清理待删除节点的内存
	delete delNode;
	//链表长度减1
	this->length--;

	return true;
}

//获取一个节点内容
template<class T>
T LinkList<T>::Get(int pos)
{
	//创建目标节点内容缓存变量
	T getNodeT;
	//判断目标获取位置是否正常
	if (pos < 0 || pos >(this->length - 1))
	{
		cout << "Get() failed!" << endl;
		return getNodeT;
	}
	//找到目标节点
	LinkListNode<T>* curNode = listHead;
	for (int i = 0; i <= pos; i++)
	{
		curNode = curNode->next;
	}
	//缓存目标节点内容
	getNodeT = curNode->node;

	return getNodeT;
}