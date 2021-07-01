#include <iostream>
#include <windows.h>
#include "XTread.h"
#include "XMutex.h"

using namespace std;

//用于临界区测试
static char buf[1024] = { 0 };
//创建临界区变量（尽晚进入，尽早退出）
//CRITICAL_SECTION section;
XMutex mutex;

class MyThread :public XTread
{
public:
	//重写XThread的纯虚函数Main
	void Main()
	{
		cout << "In myThread!" << endl;
		for (;;)
		{
			int size = sizeof(buf);
			//进入临界区变量
			//EnterCriticalSection(&section);
			mutex.Lock();
			for (int i = 0; i < size; i++)
			{
				buf[i] = ch;
				Sleep(1);
			}
			buf[size - 1] = '\0';
			cout << "[" << buf << "]" << endl;
			//退出临界区变量
			//LeaveCriticalSection(&section);
			mutex.Unlock();
			Sleep(1);
		}
	}
public:
	char ch;
};

int main(int argc, char *argv[])
{
	//初始化临界区变量
	//InitializeCriticalSection(&section);

	//创建两个线程
	MyThread s1;
	s1.ch = 'A';
	MyThread s2;
	s2.ch = 'B';

	s1.Start();
	s2.Start();


	cout << "finish" << endl;
	getchar();
	return 0;
}

