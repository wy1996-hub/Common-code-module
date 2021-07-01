#include <iostream>
#include <windows.h>
#include "XTread.h"
#include "XMutex.h"

using namespace std;

//�����ٽ�������
static char buf[1024] = { 0 };
//�����ٽ���������������룬�����˳���
//CRITICAL_SECTION section;
XMutex mutex;

class MyThread :public XTread
{
public:
	//��дXThread�Ĵ��麯��Main
	void Main()
	{
		cout << "In myThread!" << endl;
		for (;;)
		{
			int size = sizeof(buf);
			//�����ٽ�������
			//EnterCriticalSection(&section);
			mutex.Lock();
			for (int i = 0; i < size; i++)
			{
				buf[i] = ch;
				Sleep(1);
			}
			buf[size - 1] = '\0';
			cout << "[" << buf << "]" << endl;
			//�˳��ٽ�������
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
	//��ʼ���ٽ�������
	//InitializeCriticalSection(&section);

	//���������߳�
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

