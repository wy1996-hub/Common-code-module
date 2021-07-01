#include "XTread.h"

#include <windows.h>
#include <process.h>

static void ThreadMain(void* para)
{
	//获取当前线程类对象的指针
	XTread* th = (XTread*)para;
	//线程执行内容为该线程类的成员函数Main，纯虚函数，用户需要继承后自定义
	th->Main();
	//执行结束后释放线程资源
	_endthread();
}

XTread::XTread()
{
}


XTread::~XTread()
{
}


//开启线程
bool XTread::Start()
{
	//开启线程，入口函数为cpp内的一个静态函数，占空间大小由系统分配，参数传递当前线程类对象
	idThread = _beginthread(ThreadMain, 0, this);  //开启线程成功将返回线程句柄号
	if ((int)idThread <= 0)
	{
		return false;
	}

	return true;
}

//等待线程结束（阻塞函数）
void XTread::Wait()
{
	//调用windows等待资源的函数，等待时间（微秒）设置为INFINITE表示一直等待
	WaitForSingleObject((HANDLE)idThread, INFINITE);
}
