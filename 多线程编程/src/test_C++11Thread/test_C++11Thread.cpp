#include <iostream>
#include <windows.h>
#include <thread> //�߳���
#include <mutex>  //������

using namespace std;

//��Ʊ��
static int ticks = 100;
//����һ���������
mutex mu;

class SaleTick
{
public:
	void Main()
	{
		cout << "In Sale Tick " << endl;
		for (;;)
		{
			mu.lock();
			if (ticks <= 0)
			{
				mu.unlock();
				break;
			}
			ticks--;
			mu.unlock();
			sales++;
			Sleep(1);
		}
	}
	//��ǰ�߳���Ʊ��
	int sales = 0;
};

int main(int argc, char *argv[])
{
	SaleTick st1;
	SaleTick st2;

	//C++11���߳��಻��ǿ����ȫ�ֺ�����Ϊ�߳���ں���������ʹ�����Ա����
	thread th1(&SaleTick::Main, &st1);  //�������Լ�����Ӷ�����������ǲ�����������
	Sleep(50);  //ʹ�����߳�����ʱ���������۲���Ʊ������
	thread th2(&SaleTick::Main, &st2);

	//�����߳������̷߳��루Ҳ������join���ȴ����߳����н�����
	//th1.detach();
	//th2.detach();
	th1.join();
	th2.join();
	cout << "st1: " << st1.sales << " st2: " << st2.sales << endl << endl;

	cout << "main thread finish" << endl;
	getchar();
	return 0;
}