#pragma once
class XTread
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

private:
	//线程句柄号
	unsigned int idThread = 0;
};

