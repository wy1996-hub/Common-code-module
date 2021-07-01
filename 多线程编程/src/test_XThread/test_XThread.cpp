#include <iostream>
#include <windows.h>
#include "XTread.h"

using namespace std;

class SusThread :public XTread  //测试线程挂起与恢复
{
public:
	//重写XThread的纯虚函数Main
	void Main()
	{
		for (;;)
		{
			cout << ch << endl;
			Sleep(500);
		}
	}
public:
	char ch;
};

int main01(int argc, char *argv[])
{
	//创建两个线程
	SusThread s1;
	s1.ch = 'A';
	SusThread s2;
	s2.ch = 'B';

	//启动两个线程，将同时进行打印操作
	s1.Start();
	s2.Start();

	//3秒后将s2挂起
	Sleep(3000);
	s2.Suspend();

	//3秒后将s2恢复
	Sleep(3000);
	s2.Resume();

	cout << "finish" << endl;
	getchar();
	return 0;
}