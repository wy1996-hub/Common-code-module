#include <iostream>
#include <windows.h>
#include "XTread.h"

using namespace std;

class SusThread :public XTread  //�����̹߳�����ָ�
{
public:
	//��дXThread�Ĵ��麯��Main
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
	//���������߳�
	SusThread s1;
	s1.ch = 'A';
	SusThread s2;
	s2.ch = 'B';

	//���������̣߳���ͬʱ���д�ӡ����
	s1.Start();
	s2.Start();

	//3���s2����
	Sleep(3000);
	s2.Suspend();

	//3���s2�ָ�
	Sleep(3000);
	s2.Resume();

	cout << "finish" << endl;
	getchar();
	return 0;
}