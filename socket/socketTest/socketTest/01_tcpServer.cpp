#include <iostream>
#include <windows.h>
#include <thread>

using namespace std;

class TcpThread
{
public:
	void Main()
	{
		//用于存储客户端发来的数据
		char buf[1024] = { 0 };
		//持续与客户端进行数据交互
		for (;;)
		{
			char buf[1024] = { 0 };
			int recvlen = recv(idClient, buf, sizeof(buf) - 1, 0);  //从client文件流中一次读取最多sizeof(buf)-1个数据，留一个写\0
			if (recvlen <= 0 || strstr(buf, "quit") != NULL)  //如果接收的数据为空或者接收到"quit"，则不再接收，退出
			{
				char re[] = "quit success!\n";
				send(idClient, re, strlen(re) + 1, 0);
				cout << "idClient " << idClient << " quit success!" << endl << endl;
				break;
			}
			buf[recvlen] = '\0';  //接收到的字串不会自动在末尾补0，除非事先约定好
			int sendlen = send(idClient, "ok\n", 4, 0);
			cout << "idClient " << idClient << " recv:" << buf << endl;
		}
		//释放套接字占用的资源
		closesocket(idClient);
		//释放该线程对象占用的资源
		delete this;
	}
	int idClient = 0;
};

int main(int argc, char *argv[])
{
	//初始化动态链接库，需要添加依赖项ws2_32.lib
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	//1.创建并初始化一个用于监听端口的socket套接字，AF_INET标识了协议簇用ipv4地址（32位的）与端口号（16位的）的组合，SOCK_STREAM标识了socket是TCP类型
	int sock = socket(AF_INET, SOCK_STREAM, 0);  //建立一个socket意味着为一个socket数据结构分配存储空间
	if (sock == -1)
	{
		cout << "创建通信socket失败！" << endl << endl;
		getchar(); return -1;
	}
	else
	{
		cout << "创建通信socket成功！" << endl << endl;
	}

	//2.将创建的socket套接字文件绑定到指定的IP地址和端口上
	unsigned short port = 8081;  //配置用于监听数据的端口号（给0时，系统随机选择一个未被使用的端口号）
	if (argc > 1) //若客户端传入了端口号，则用客户端传入的端口号进行监听
	{
		port = atoi(argv[1]);
	}
	sockaddr_in saddr;					//声明一个存储服务端自身IP地址、端口号的结构体，后面用于bind调用，与socket文件绑定
	saddr.sin_family = AF_INET;			//标识协议簇用ipv4地址（32位的）与端口号（16位的）的组合
	saddr.sin_port = htons(port);		//配置用于监听的端口号,注意用htons将端口号的字节序改为大端字节序Big Endian（网络字节序格式）
	saddr.sin_addr.s_addr = htonl(0);	//配置接收访问的IP地址，0标识任意IP都接收访问，INADDR_ANY也是表示任意IP可以接收访问
	int re = 0;
	re = ::bind(sock, (sockaddr*)&saddr, sizeof(saddr));  //绑定操作，绑定成功返回值为0，若不为0则绑定失败
	/*在C++11中，std命名空间下也有一个bind函数，此处要想使用socket的bind函数，在bind前加::标识此bind函数是全局下的，而非某个命名空间下的*/
	if (re != 0)
	{
		printf("bind port %d failed!\n", port);
		getchar(); return -2;
	}
	else
	{
		cout << "bind port " << port << " success!" << endl << endl;
	}

	//3.开始监听socket文件绑定的端口信息
	re = listen(sock, 10);  //指定了内核为此套接字排队的最大连接个数为10,队列中超过10个请求存在仍未处理则溢出
	if (re != 0)
	{
		cout << "listen port failed!" << endl << endl;
		getchar(); return -3;
	}
	else
	{
		cout << "listen port success!" << endl << endl;
	}

	//4.不断从监听的端口中获取连接信息，每来一个连接信息创建一个专用于该客户端的数据交互socket套接字
	for (;;)
	{
		//持续监听是否有新的客户端连入
		sockaddr_in caddr;			//声明一个存储接收连接客户端IP地址、端口号的结构体
		int clen = sizeof(caddr);
		int client = accept(sock, (sockaddr*)&caddr, &clen);  //创建一个专用于该客户端的数据交互socket套接字，客户端IP、端口等信息存在caddr中
		if (client == -1)
		{
			cout << "创建数据交互socket失败！" << endl << endl;
			getchar(); return -1;
		}
		else
		{
			cout << "创建数据交互socket成功！" << endl;
			char *ip = inet_ntoa(caddr.sin_addr);			//从caddr中取出客户端的IP
			unsigned short cport = ntohs(caddr.sin_port);	//从caddr中取出客户端的端口号（转为小型机字节序，小端字节序Little Endian）
			printf("该客户端IP为：%s, 端口号为：%d\n", ip, cport);
			cout << endl;
		}
		//为新连入的客户端创建线程，独立实现数据交互
		TcpThread *clientThread = new TcpThread();
		clientThread->idClient = client;  //将该客户端句柄号传给该对象
		thread newTread(&TcpThread::Main, clientThread);
		newTread.detach();
	}

	

	//关闭服务端监听socket
	closesocket(sock);


	cout << "=================================" << endl << "socketTest run success!" << endl;
	getchar(); return 0;
}