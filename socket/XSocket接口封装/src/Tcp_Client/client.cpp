#include <iostream>
#include "XTcp.h"

#include <windows.h>

using namespace std;

int main(int argc, char *argv[])
{
	cout << "===============TCP Client===============" << endl << endl;

	//创建TCP客户端对象
	XTcp client;

	//为客户端创建数据交互socket
	client.CreateSocket();
	if (client.sock != -1)
	{
		cout << "Tcp client create success!" << endl << endl;
	}

	//unsigned short port = ntohs(client.port);
	//cout << port << endl << endl;

	//与服务端申请连接，连接超时设置为1000毫秒
	if (client.Connect("192.168.8.121", 8081, 2000))
	{
		for (int i = 0; i < 10; i++)
		{
			char re[] = "Hello server!";
			client.Send(re, strlen(re) + 1);
			Sleep(1000);
		}
	}

	//清理客户端的socket资源
	client.Close();

	cout << "Tcp_Client run success!" << endl;
	getchar();
	return 0;
}