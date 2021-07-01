#include "XTread.h"

#include <windows.h>
#include <process.h>

static void ThreadMain(void* para)
{
	//��ȡ��ǰ�߳�������ָ��
	XTread* th = (XTread*)para;
	//�߳�ִ������Ϊ���߳���ĳ�Ա����Main�����麯�����û���Ҫ�̳к��Զ���
	th->Main();
	//ִ�н������ͷ��߳���Դ
	_endthread();
}

XTread::XTread()
{
}


XTread::~XTread()
{
}


//�����߳�
bool XTread::Start()
{
	//�����̣߳���ں���Ϊcpp�ڵ�һ����̬������ռ�ռ��С��ϵͳ���䣬�������ݵ�ǰ�߳������
	idThread = _beginthread(ThreadMain, 0, this);  //�����̳߳ɹ��������߳̾����
	if ((int)idThread <= 0)
	{
		return false;
	}

	return true;
}

//�ȴ��߳̽���������������
void XTread::Wait()
{
	//����windows�ȴ���Դ�ĺ������ȴ�ʱ�䣨΢�룩����ΪINFINITE��ʾһֱ�ȴ�
	WaitForSingleObject((HANDLE)idThread, INFINITE);
}

//�̹߳������أ��类�����߳������˻�������Դ�����ڹ����ڼ�һֱռ�ã�
void XTread::Suspend()
{
	//�ж��߳̾���Ƿ���Ч
	if (idThread == 0)
		return;
	SuspendThread((HANDLE)idThread);
}

//�ָ̻߳�
void XTread::Resume()
{
	//�ж��߳̾���Ƿ���Ч
	if (idThread == 0)
		return;
	ResumeThread((HANDLE)idThread);
}
