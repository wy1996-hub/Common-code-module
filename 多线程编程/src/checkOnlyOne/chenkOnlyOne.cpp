#include <iostream>
#include <windows.h>

using namespace std;

int main(int argc, char *argv[])
{
	//���û��������Ƹó���ֻ������һ��

	cout << "in this process";

	//�����������,���������ԡ��Ƿ��ʼ������������
	HANDLE hMutex = CreateMutex(NULL, true, L"XONE");

	//���û����ϵ��ֹ����
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(0, L"XONE already running!", L"", 0);
		if (hMutex)
			CloseHandle(hMutex);  //�����ͷ���Դ
		return -1;
	}

	getchar();
	return 0;
}