#include <iostream>
#include <windows.h>
#include <thread> //线程类
#include <mutex>  //互斥类

using namespace std;

//总票数
static int ticks = 100;
//创建一个互斥变量
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
	//当前线程售票数
	int sales = 0;
};

int main(int argc, char *argv[])
{
	SaleTick st1;
	SaleTick st2;

	//C++11的线程类不再强制用全局函数作为线程入口函数，可以使用类成员函数
	thread th1(&SaleTick::Main, &st1);  //参数可以继续添加多个参数，但是不建议这样做
	Sleep(50);  //使两个线程启动时间错开，方便观察售票数差异
	thread th2(&SaleTick::Main, &st2);

	//将子线程与主线程分离（也可以用join，等待子线程运行结束）
	//th1.detach();
	//th2.detach();
	th1.join();
	th2.join();
	cout << "st1: " << st1.sales << " st2: " << st2.sales << endl << endl;

	cout << "main thread finish" << endl;
	getchar();
	return 0;
}