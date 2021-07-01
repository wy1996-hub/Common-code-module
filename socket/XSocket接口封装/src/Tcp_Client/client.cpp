#include <iostream>
#include "XTcp.h"

#include <windows.h>

using namespace std;

int main(int argc, char *argv[])
{
	cout << "===============TCP Client===============" << endl << endl;

	//����TCP�ͻ��˶���
	XTcp client;

	//Ϊ�ͻ��˴������ݽ���socket
	client.CreateSocket();
	if (client.sock != -1)
	{
		cout << "Tcp client create success!" << endl << endl;
	}

	//unsigned short port = ntohs(client.port);
	//cout << port << endl << endl;

	//�������������ӣ����ӳ�ʱ����Ϊ1000����
	if (client.Connect("192.168.8.121", 8081, 2000))
	{
		for (int i = 0; i < 10; i++)
		{
			char re[] = "Hello server!";
			client.Send(re, strlen(re) + 1);
			Sleep(1000);
		}
	}

	//����ͻ��˵�socket��Դ
	client.Close();

	cout << "Tcp_Client run success!" << endl;
	getchar();
	return 0;
}