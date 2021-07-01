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
	//�����ٽ���������������Դ���������������
	void Lock();

	//�뿪�ٽ���������������Դ�������������뿪
	void Unlock();

private:
	//�ٽ�������
	void* section;  //�����void*����ͷ�ļ�
};

