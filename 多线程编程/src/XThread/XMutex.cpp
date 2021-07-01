#include "XMutex.h"
#include <windows.h>


XMutex::XMutex()
{
	//�����ٽ�������
	this->section = new CRITICAL_SECTION();
	//��ʼ���ٽ�������
	InitializeCriticalSection((CRITICAL_SECTION*)this->section);
}


XMutex::~XMutex()
{
}

//�����ٽ���������������Դ���������������
void XMutex::Lock()
{
	EnterCriticalSection((CRITICAL_SECTION*)this->section);
}

//�뿪�ٽ���������������Դ�������������뿪
void XMutex::Unlock()
{
	LeaveCriticalSection((CRITICAL_SECTION*)this->section);
}
