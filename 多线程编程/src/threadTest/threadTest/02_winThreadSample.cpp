#include <iostream>
#include <windows.h>
#include "XTread.h"

using namespace std;

static char* arr = NULL;

class MyThread :public XTread
{
public:
	//重写父类的Main函数
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
	//准备测试数据
	int size = 1000 * 1000 * 1000 * 2;
	arr = new char[size];

	//单线程处理时间测试
	long long start = GetTickCount();
	for (int i = 0; i < size; i++)
	{
		arr[i] = i;
	}
	long long end = GetTickCount();
	cout << "single thread cost " << end - start << endl << endl;

	//多线程处理时间测试
	int thredNum = 12;  //选择启用的线程数量
	MyThread* myThread = new MyThread[thredNum];
	start = GetTickCount();
	for (int i = 0; i < thredNum; i++)  //指定各线程处理的开始与结束位置
	{
		myThread[i].begin = i*(size / thredNum);
		myThread[i].end = myThread[i].begin + (size / thredNum);
		if (myThread[i].end >= size)
		{
			myThread[i].end = size - 1;
		}
		//启动线程
		myThread[i].Start();
	}
	for (int i = 0; i < thredNum; i++)  //等待各线程退出
	{
		myThread[i].Wait();
	}
	end = GetTickCount();
	cout << "multi thread cost " << end - start << endl << endl;

	cout << "finish" << endl;
	getchar();
	return 0;
}