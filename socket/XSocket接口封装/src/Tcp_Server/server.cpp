#include <iostream>
#include <thread>

#include "XTcp.h"


using namespace std;

//线程入口类
class TcpThread
{
public:
	void Main()
	{
		char buf[1024] = { 0 };
		for (;;)
		{
			int recvlen = client.Recv(buf, sizeof(buf) - 1);
			if (recvlen <= 0)
			{
				cout << "quit success!\n";
				break;
			}				
			buf[recvlen] = '\0';
			if (strstr(buf, "quit") != NULL)
			{
				char re[] = "quit success!\n";
				cout << re << endl;
				client.Send(re, strlen(re) + 1);
				break;
			}
			int sendlen = client.Send("ok\n", 4);
			cout << "recv from " << client.sock << " :" << buf << endl;
		}
		client.Close();
		delete this;
	}
	XTcp client;
};

int main(int argc, char *argv[])
{
	cout << "===============TCP Server===============" << endl << endl;

	//配置服务端监听的端口号
	unsigned short port = 8081;
	if (argc > 1)
	{
		port = atoi(argv[1]);
	}

	//创建TCP服务端对象
	XTcp server;

	//为服务端创建通信socket
	server.CreateSocket();
	if (server.sock != -1)
	{
		cout << "Tcp server create success!" << endl << endl;
	}

	//为服务端绑定目标监听端口号，同时开启监听
	server.Bind(port);

	//持续从监听的端口中获取连接信息
	for (;;)
	{
		//每来一个连接信息创建一个专用于该客户端的数据交互socket套接字
		XTcp client = server.Accept();
		cout << "accept client " << client.sock << endl;
		//创建一个自定义线程类对象
		TcpThread *clientThread = new TcpThread();
		clientThread->client = client;
		//利用C++11的线程类创建线程
		thread newThread(&TcpThread::Main, clientThread);
		//将子线程分离
		newThread.detach();
	}

	//关闭服务端监听套接字
	server.Close();
	cout << "Tcp_Server run success!" << endl;
	getchar();
	return 0;
}