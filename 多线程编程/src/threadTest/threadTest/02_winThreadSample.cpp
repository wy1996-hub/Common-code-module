#include <iostream>
#include <windows.h>
#include "XTread.h"

using namespace std;

static char* arr = NULL;

class MyThread :public XTread
{
public:
	//��д�����Main����
	void Main()
	{
		for (int i = begin; i < end; i++)
		{
			arr[i] = i;
		}
	}
public:
	int begin = 0;
	int end = 0;
};

int main(int argc, char *argv[])
{
	//׼����������
	int size = 1000 * 1000 * 1000 * 2;
	arr = new char[size];

	//���̴߳���ʱ�����
	long long start = GetTickCount();
	for (int i = 0; i < size; i++)
	{
		arr[i] = i;
	}
	long long end = GetTickCount();
	cout << "single thread cost " << end - start << endl << endl;

	//���̴߳���ʱ�����
	int thredNum = 12;  //ѡ�����õ��߳�����
	MyThread* myThread = new MyThread[thredNum];
	start = GetTickCount();
	for (int i = 0; i < thredNum; i++)  //ָ�����̴߳���Ŀ�ʼ�����λ��
	{
		myThread[i].begin = i*(size / thredNum);
		myThread[i].end = myThread[i].begin + (size / thredNum);
		if (myThread[i].end >= size)
		{
			myThread[i].end = size - 1;
		}
		//�����߳�
		myThread[i].Start();
	}
	for (int i = 0; i < thredNum; i++)  //�ȴ����߳��˳�
	{
		myThread[i].Wait();
	}
	end = GetTickCount();
	cout << "multi thread cost " << end - start << endl << endl;

	cout << "finish" << endl;
	getchar();
	return 0;
}