#pragma once

#ifdef XTCP_EXPORTS
#define XTCP_API __declspec(dllexport)
#else
#define XTCP_API __declspec(dllimport)
#endif

class XTCP_API XTcp
{
public:
	XTcp();
	virtual ~XTcp();

public:
	//创建一个Tcp协议的socket套接字文件
	int CreateSocket();

	//为服务端socket套接字绑定一个监听端口号，同时开启监听
	bool Bind(unsigned short port);

	//接受一个客户端连接，同时创建一个线程与之进行数据交互(根据返回对象的sock值判断操作是否成功)
	XTcp Accept();

	//关闭并清理socket
	void Close();

	//接收数据
	int Recv(char *buf, int bufsize);

	//发送数据（只读）,确保发完给定字节数
	int Send(const char *buf, int sendsize);

	//客户端申请连接服务端,默认设置连接超时为1秒
	bool Connect(const char *ip, unsigned short port, int timeoutms = 1000);

	//设置阻塞模式
	bool SetBlock(bool isblock);

public:
	//存储socket套接字句柄ID
	int sock = 0;
	//存储端口号
	unsigned short port = 0;
	//存储IP地址
	char ip[16];
};

