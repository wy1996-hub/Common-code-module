#include <iostream>
#include <windows.h>
#include <process.h>

using namespace std;

//�߳���ں�������ʽ�̶���ͨ��������޷���ֵ��
void threadMain1(void* para)
{
	int* p = (int*)para;

	for (int i = 0; i < 20; i++)
	{
		cout << "in thread1 para = " << *p << endl;
		Sleep(500);
	}
	cout << "thread1 finish!" << endl;

	//�����߳���Դ
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

	//�����߳���Դ
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