#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

//线程入口函数（格式固定，通配参数，无返回值）
void threadMain1(void* para)
{
	int* p = (int*)para;

	for (int i = 0; i < 20; i++)
	{
		cout << "in thread1 para = " << *p << endl;
		Sleep(500);
	}
	cout << "thread1 finish!" << endl;

	//清理线程资源
	_endthread();
}

void threadMain2(void* para)
{
	int* p = (int*)para;

	for (int i = 0; i < 20; i++)
	{
		cout << "in thread2 para = " << *p << endl;
		Sleep(500);
	}
	cout << "thread2 finish!" << endl;

	//清理线程资源
	_endthread();
}

int main01(int argc, char *argv[])
{

	int i = 111;
	int j = 222;
	_beginthread(threadMain1, 0, &i);
	_beginthread(threadMain2, 0, &j);
	
	Sleep(2000);
	i = 333;
	j = 444;

	cout << "Main thread is finish!" << endl << endl;
	getchar();
	return 0;
}