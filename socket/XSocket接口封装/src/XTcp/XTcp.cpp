#include "XTcp.h"

#include <iostream>
#include <windows.h>

using namespace std;

XTcp::XTcp()
{
	//socket相关动态链接库只需要在对象生成时初始化一次即可
	static bool first = true;
	if (first)
	{
		first = false;
		WSADATA ws;							//声明并初始化socket相关动态链接库
		WSAStartup(MAKEWORD(2, 2), &ws);	//将动态链接库的引用计数+1
	}
}


XTcp::~XTcp()
{
}


//创建一个Tcp协议的socket套接字文件
int XTcp::CreateSocket()
{
	//创建并初始化一个socket套接字
	sock = socket(AF_INET, SOCK_STREAM, 0);		//AF_INET标识协议簇用ipv4地址（32位的）与端口号（16位的）的组合，SOCK_STREAM标识了socket是TCP类型
	if (sock == -1)								//创建失败返回-1
	{
		cout << "CreateSocket() failed!" << endl << endl;
		return -1;
	}

	//创建成功返回socket套接字句柄ID
	return sock;
}

//为服务端socket套接字绑定一个监听端口号，同时开启监听
bool XTcp::Bind(unsigned short port)
{
	//绑定前判断当前服务端socket的句柄号是否正常
	if (sock <= 0)
		CreateSocket();
	//为当前服务端socket文件绑定端口
	sockaddr_in  saddr;					//声明一个存储服务端自身IP地址、端口号的结构体，后面用于bind调用，与socket文件绑定
	saddr.sin_family = AF_INET;			//标识协议簇用ipv4地址（32位的）与端口号（16位的）的组合
	saddr.sin_port = htons(port);		//配置用于监听的端口号,注意用htons将端口号的字节序改为大端字节序Big Endian（网络字节序格式）
	saddr.sin_addr.s_addr = htonl(0);	//配置接收访问的IP地址，0标识任意IP都接收访问，INADDR_ANY也是表示任意IP可以接收访问
	/*在C++11中，std命名空间下也有一个bind函数，此处要想使用socket的bind函数，在bind前加::标识此bind函数是全局下的，而非某个命名空间下的*/
	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)  //绑定操作，绑定成功返回值为0，若不为0则绑定失败
	{
		cout << "Bind port " << port << " failed!" << endl << endl;
		return false;
	}
	//打印服务端绑定监听的IP和端口号信息
	char *ip = inet_ntoa(saddr.sin_addr);
	cout << "The IP of server is picked from local. Please inquire your network card device!" << endl;
	cout << "Tcp server bind port " << port << " success!" << endl;
	//开始监听绑定端口的连接信息
	if (listen(sock, 1000) != 0)		//指定了内核为此套接字排队的最大连接个数为1000,队列中超过1000个请求存在仍未处理则溢出
	{
		cout << "Listen port " << port << " failed!" << endl << endl;
		return false;
	}
	cout << "Tcp server is listening..." << endl << endl;

	return true;
}

//接受一个客户端连接，同时创建一个线程与之进行数据交互
XTcp XTcp::Accept()
{
	//为客户端创建一个XTcp对象
	XTcp tcp;
	sockaddr_in caddr;  //声明一个存储接收连接客户端IP地址、端口号的结构体
	int len = sizeof(caddr);
	int client = accept(sock, (sockaddr*)&caddr, &len);  //创建一个专用于该客户端的数据交互socket套接字，客户端IP、端口信息存在caddr中
	if (client == -1)
	{
		cout << "Accept client failed!" << endl;
		return tcp;
	}
	//转存连入的客户端IP、端口、socket句柄号信息
	char *ipClient = inet_ntoa(caddr.sin_addr);
	strcpy(tcp.ip, ipClient);
	tcp.port = ntohs(caddr.sin_port);
	tcp.sock = client;
	cout << "Tcp server acceptclient! " << tcp.ip << ":" << tcp.port << " success!" << endl;

	return tcp;
}

//关闭并清理socket
void XTcp::Close()
{
	//判断当前socket是否占用了资源
	if (sock <= 0) 
		return;
	//关闭并清理
	closesocket(sock);
	//将socket句柄号清零
	sock = 0;
}

//接收数据
int XTcp::Recv(char *buf, int bufsize)
{
	//将接收到的数据写入缓存buf中
	return recv(sock, buf, bufsize, 0);
}

//发送数据（只读）,确保发完给定字节数
int XTcp::Send(const char *buf, int sendsize)
{
	//存储累计已经发出的字节数
	int s = 0;
	//持续发送，直到将目标发出的字节数全部发完
	while (s != sendsize)
	{
		int len = send(sock, buf + s, sendsize - s, 0);
		if (len <= 0)break;
		s += len;
	}

	//返回发出的字节数
	return s;
}

//客户端申请连接服务端
bool XTcp::Connect(const char *ipServer, unsigned short portServer, int timeoutms)
{
	//判断该客户端的socket句柄号是否有效
	if (sock <= 0) 
		CreateSocket();
	//配置目标连接服务端的IP地址和端口号
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(portServer);
	saddr.sin_addr.s_addr = inet_addr(ipServer);
	//设置连接为非阻塞连接，连接申请超时时间默认为1秒，可修改
	SetBlock(false);
	//使用select轮循函数实现超时时间的设定
	fd_set set;  //文件句柄数组，每一个数组元素都能与一打开的文件句柄(socket、文件、管道、设备等)
	if (connect(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)  //如果连接失败
	{
		FD_ZERO(&set);					//先清空文件句柄数组
		FD_SET(sock, &set);				//将sock加入数组（sock属于网络句柄）
		timeval tm;						//设置超时时长，若配置0标识不阻塞
		tm.tv_sec = 0;					//设置秒
		tm.tv_usec = timeoutms * 1000;	//设置微秒（传入的毫秒要*1000）
		//监听文件序列中是否有可写(参数：1最大文件句柄号+1，2可读的，3可写的，4错误处理，5超时设置)
		if (select(sock + 1, 0, &set, 0, &tm) <= 0)  //判断在规定时间内，是否有可以写入的文件的文件，出错返回-1，超时返回0
		{
			cout << "Connect server timeout or error!" << endl;
			printf("Connect server %s:%d failed! Error is :%s\n", ipServer, portServer, strerror(errno));
			return false;
		}
	}
	//将连接还原为阻塞连接
	SetBlock(true);

	return true;
}

//设置阻塞模式
bool XTcp::SetBlock(bool isblock)
{
	//判断该客户端的socket句柄号是否有效
	if (sock <= 0) 
		return false;
	//若转入false，u1=0，为非阻塞连接
	unsigned long ul = 0;
	//若传入true，u1=1，保持阻塞连接（accept默认是阻塞）
	if (!isblock) 
		ul = 1;
	//设置阻塞模式
	ioctlsocket(sock, FIONBIO, &ul);

	return true;
}
