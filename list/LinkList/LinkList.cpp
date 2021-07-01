#include "LinkList.h"
#include <iostream>

using namespace std;

//���캯��
template<class T>
LinkList<T>::LinkList()
{
	//��ʼ����ṹ�Ļ�������
	this->length = 0;
	//����ͷ���ڵ���󣬳�ʼ����ͷ���ڵ�ָ�����ÿ�
	this->listHead = new LinkListNode<T>;
	this->listHead->next = NULL;
}

//�����������������ڵ���ͷ���ڵ㣩
template<class T>
LinkList<T>::~LinkList()
{
	//������и��ڵ������
	Clear();
	//����ͷ���ڵ�
	delete this->listHead;
	this->listHead = NULL;
	this->length = 0;
}

//��ձ�����
template<class T>
void LinkList<T>::Clear()
{
	//�жϱ����Ƿ�������
	if (this->length <= 0)
		return;

	//ͷ���ڵ㱣��
	LinkListNode<T>* curNode = this->listHead->next;
	//������������нڵ������
	for (int i = 0; i < this->length; i++)
	{
		//���浱ǰ�ڵ�
		LinkListNode<T>* tmpNode = curNode;
		//ͷ���ڵ����
		curNode = curNode->next;
		//����ǰ�ڵ���ڴ�
		delete tmpNode;
	}
	//����������
	this->length = 0;
	//ͷ���ڵ�ָ���ÿ�
	this->listHead->next = NULL;
}

//��ȡ��ĳ���
template<class T>
int LinkList<T>::getLength()
{
	return this->length;
}

//����һ���ڵ�
template<class T>
bool LinkList<T>::Insert(int pos, T& node)
{
	//�ж�Ŀ�����λ���Ƿ�����
	if (pos < 0 || pos > this->length)
	{
		cout << "Insert() failed!" << endl;
		return false;
	}
	//������������ɽڵ�,��ʼ��ָ����Ϊ��
	LinkListNode<T>* newNode = new LinkListNode<T>;
	newNode->node = node;
	newNode->next = NULL;
	//�ҵ�Ŀ�����λ�õ�ǰһ���ڵ�
	LinkListNode<T>* curNode = listHead;
	for (int i = 0; i < pos; i++)
	{
		curNode = listHead->next;
	}
	//��curNode��ָ�������ݸ���newNode
	newNode->next = curNode->next;
	//��curNode��ָ����ָ��newNode
	curNode->next = newNode;
	//�����ȼ�1
	this->length++;

	return true;
}

//ɾ��һ���ڵ�
template<class T>
bool LinkList<T>::Delete(int pos)
{
	//�ж�Ŀ��ɾ��λ���Ƿ�����
	if (pos < 0 || pos > (this->length - 1))
	{
		cout << "Delete() failed!" << endl;
		return false;
	}
	//�ҵ�Ŀ��ɾ��λ�õ�ǰһ���ڵ�
	LinkListNode<T>* curNode = listHead;
	for (int i = 0; i < pos; i++)
	{
		curNode = listHead->next;
	}
	//�����ɾ���ڵ�
	LinkListNode<T>* delNode = curNode->next;
	//����ɾ���ڵ��ǰһ���ڵ�ָ���һ���ڵ�
	curNode->next = delNode->next;
	//�����ɾ���ڵ���ڴ�
	delete delNode;
	//�����ȼ�1
	this->length--;

	return true;
}

//��ȡһ���ڵ�����
template<class T>
T LinkList<T>::Get(int pos)
{
	//����Ŀ��ڵ����ݻ������
	T getNodeT;
	//�ж�Ŀ���ȡλ���Ƿ�����
	if (pos < 0 || pos >(this->length - 1))
	{
		cout << "Get() failed!" << endl;
		return getNodeT;
	}
	//�ҵ�Ŀ��ڵ�
	LinkListNode<T>* curNode = listHead;
	for (int i = 0; i <= pos; i++)
	{
		curNode = curNode->next;
	}
	//����Ŀ��ڵ�����
	getNodeT = curNode->node;

	return getNodeT;
}