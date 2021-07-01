#include "SeqList.h"
#include <iostream>

using namespace std;

//���ι��캯��
template<class T>
SeqList<T>::SeqList(const int capacity)
{
	//��ʼ����ṹ�Ļ�������
	this->capacity = capacity;
	this->length = 0;
	//�����û��趨������С�ʹ��洢�Ķ������Ϳ����ڴ棬���׵�ַ����pArray
	this->pArray = new T[capacity];
}

//����������
template<class T>
SeqList<T>::~SeqList()
{
	//�жϱ�ṹ�Ƿ����
	if (this->pArray == NULL)
		return;

	//�ͷŴ洢����������ڴ沢��ָ���ÿ�
	delete[] this->pArray;
	this->pArray = NULL;
	//�����Ⱥ���������
	this->capacity = 0;
	this->length = 0;
}

//��ձ�����
template<class T>
void SeqList<T>::Clear()
{
	//�жϱ�ṹ�Ƿ����
	if (this->pArray == NULL)
		return;

	//��������0
	this->length = 0;
}

//��ȡ��ĳ���
template<class T>
int SeqList<T>::getLength()
{
	//�жϱ�ṹ�Ƿ����
	if (this->pArray == NULL)
	{
		cout << "getLength() failed! List is not exist!" << endl;
		return -1;
	}

	return this->length;
}

//��ȡ�������
template<class T>
int SeqList<T>::getCapacity()
{
	//�жϱ�ṹ�Ƿ����
	if (this->pArray == NULL)
	{
		cout << "getCapacity() failed! List is not exist!" << endl;
		return -1;
	}

	return this->capacity;
}

//����һ���ڵ�
template<class T>
bool SeqList<T>::Insert(int pos, T& node)
{
	//�жϱ�ṹ�Ƿ����,���Ƿ�������Ŀ�����λ���Ƿ�����
	if (this->pArray == NULL || this->length == this->capacity || pos < 0 || pos > this->length)
	{
		cout << "Insert() failed!" << endl;
		return false;
	}
	//�Ӳ���λ�������һ���ڵ㴦�����нڵ�λ�ú���
	for (int i = this->length; i > pos; i--)
	{
		this->pArray[i] = this->pArray[i - 1];
	}
	//��pos����Ϊ�½ڵ�
	this->pArray[pos] = node;
	//���ȼ�1
	this->length++;

	return true;
}

//ɾ��һ���ڵ�
template<class T>
bool SeqList<T>::Delete(int pos)
{
	//�жϱ�ṹ�Ƿ����,�Ƿ������ݣ�Ŀ��ɾ��λ���Ƿ�����
	if (this->pArray == NULL || this->length == 0 || pos < 0 || pos > length - 1)
	{
		cout << "Delete() failed!" << endl;
		return false;
	}
	//��ɾ��λ�õĺ�һ���ڵ������һ���ڵ㴦�����нڵ�λ��ǰ��
	for (int i = pos; i < length - 1; i++)
	{
		this->pArray[i] = this->pArray[i + 1];
	}
	//���ȼ�1
	this->length--;

	return true;
}

//��ȡһ���ڵ�
template<class T>
T SeqList<T>::Get(int pos)
{
	//����ģ�����ʹ���һ���ڵ����
	T node;
	//�жϱ�ṹ�Ƿ����,�Ƿ������ݣ�Ŀ���ȡλ���Ƿ�����
	if (this->pArray == NULL || this->length == 0 || pos < 0 || pos >= length)
	{
		cout << "Get() failed!" << endl;
		return node;
	}
	//ֱ�Ӹ���λ������	
	node = pArray[pos];

	return node;
}
