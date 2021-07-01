#include "XMutex.h"
#include <windows.h>


XMutex::XMutex()
{
	//创建临界区变量
	this->section = new CRITICAL_SECTION();
	//初始化临界区变量
	InitializeCriticalSection((CRITICAL_SECTION*)this->section);
}


XMutex::~XMutex()
{
}

//进入临界区变量（锁定资源），尽可能晚进入
void XMutex::Lock()
{
	EnterCriticalSection((CRITICAL_SECTION*)this->section);
}

//离开临界区变量（解锁资源），尽可能早离开
void XMutex::Unlock()
{
	LeaveCriticalSection((CRITICAL_SECTION*)this->section);
}
