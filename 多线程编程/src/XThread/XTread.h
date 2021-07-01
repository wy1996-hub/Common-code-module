#pragma once

#ifdef XTHREAD_EXPORTS
#define XTHREAD_API __declspec(dllexport)
#else
#define XTHREAD_API __declspec(dllimport)
#endif


class XTHREAD_API XTread
{
public:
	XTread();
	virtual ~XTread();

public:

	//开启线程
	bool Start();

	//线程执行内容，纯虚函数，必须由用户继承后自定义实现
	virtual void Main() = 0;

	//等待线程结束（阻塞函数）
	void Wait();

	//线程挂起（慎重，如被挂起线程申请了互斥量资源，则在挂起期间一直占用）
	void Suspend();

	//线程恢复
	void Resume();

private:
	//线程句柄号
	unsigned int idThread = 0;
};

