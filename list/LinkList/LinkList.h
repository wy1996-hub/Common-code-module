#pragma once

//�����еĽڵ㣬����ָ����һ���ڵ��ָ��
template<class T>
struct LinkListNode
{
	//��¼��һ���ڵ�ָ�����λ�ã���ڵ�ͷ���غϣ�
	LinkListNode* next;
	//�ڵ��д�ŵ�ģ�����
	T node;
};

template<class T>
class LinkList
{
public:
	//���캯��
	LinkList();
	//�����������������ڵ���ͷ���ڵ㣩
	virtual ~LinkList();

public:
	//��ձ�����
	void Clear();
	//��ȡ��ĳ���
	int getLength();
	//����һ���ڵ�
	bool Insert(int pos, T& node);
	//ɾ��һ���ڵ�
	bool Delete(int pos);
	//��ȡһ���ڵ�
	T Get(int pos);

public:
	//�����ͷ���ڵ�
	LinkListNode<T>* listHead;
	//����ĳ���
	int length;

};

