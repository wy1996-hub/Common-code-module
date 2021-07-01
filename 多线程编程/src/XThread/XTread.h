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

	//�����߳�
	bool Start();

	//�߳�ִ�����ݣ����麯�����������û��̳к��Զ���ʵ��
	virtual void Main() = 0;

	//�ȴ��߳̽���������������
	void Wait();

	//�̹߳������أ��类�����߳������˻�������Դ�����ڹ����ڼ�һֱռ�ã�
	void Suspend();

	//�ָ̻߳�
	void Resume();

private:
	//�߳̾����
	unsigned int idThread = 0;
};

