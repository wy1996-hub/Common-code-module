#pragma once

#ifdef XTHREAD_EXPORTS
#define XTHREAD_API __declspec(dllexport)
#else
#define XTHREAD_API __declspec(dllimport)
#endif

class XTHREAD_API XMutex
{
public:
	XMutex();
	virtual ~XMutex();

public:
	//进入临界区变量（锁定资源），尽可能晚进入
	void Lock();

	//离开临界区变量（解锁资源），尽可能早离开
	void Unlock();

private:
	//临界区变量
	void* section;  //定义成void*屏蔽头文件
};

