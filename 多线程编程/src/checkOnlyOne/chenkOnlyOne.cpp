#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char *argv[])
{
	//利用互斥量控制该程序只能运行一个

	cout << "in this process";

	//创建互斥变量,参数：属性、是否初始化、互斥量名
	HANDLE hMutex = CreateMutex(NULL, true, L"XONE");

	//利用互斥关系阻止启动
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(0, L"XONE already running!", L"", 0);
		if (hMutex)
			CloseHandle(hMutex);  //主动释放资源
		return -1;
	}

	getchar();
	return 0;
}