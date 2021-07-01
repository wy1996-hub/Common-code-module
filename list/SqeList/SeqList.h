#pragma once


template<class T>
class SeqList
{
public:
	//���ι��캯��
	SeqList(const int capacity);
	//����������
	virtual ~SeqList();

public:
	//��ձ�����
	void Clear();
	//��ȡ��ĳ���
	int getLength();
	//��ȡ�������
	int getCapacity();
	//����һ���ڵ�
	bool Insert(int pos, T& node);
	//ɾ��һ���ڵ�
	bool Delete(int pos);
	//��ȡһ���ڵ�
	T Get(int pos);
	//�洢���б�ڵ���������ָ��
	T* pArray;
private:
	//������
	int capacity;
	//��ǰ��ĳ���
	int length;
	

};

