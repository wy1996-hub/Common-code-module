#include "XTcp.h"

#include <iostream>
#include <windows.h>

using namespace std;

XTcp::XTcp()
{
	//socket��ض�̬���ӿ�ֻ��Ҫ�ڶ�������ʱ��ʼ��һ�μ���
	static bool first = true;
	if (first)
	{
		first = false;
		WSADATA ws;							//��������ʼ��socket��ض�̬���ӿ�
		WSAStartup(MAKEWORD(2, 2), &ws);	//����̬���ӿ�����ü���+1
	}
}


XTcp::~XTcp()
{
}


//����һ��TcpЭ���socket�׽����ļ�
int XTcp::CreateSocket()
{
	//��������ʼ��һ��socket�׽���
	sock = socket(AF_INET, SOCK_STREAM, 0);		//AF_INET��ʶЭ�����ipv4��ַ��32λ�ģ���˿ںţ�16λ�ģ�����ϣ�SOCK_STREAM��ʶ��socket��TCP����
	if (sock == -1)								//����ʧ�ܷ���-1
	{
		cout << "CreateSocket() failed!" << endl << endl;
		return -1;
	}

	//�����ɹ�����socket�׽��־��ID
	return sock;
}

//Ϊ�����socket�׽��ְ�һ�������˿ںţ�ͬʱ��������
bool XTcp::Bind(unsigned short port)
{
	//��ǰ�жϵ�ǰ�����socket�ľ�����Ƿ�����
	if (sock <= 0)
		CreateSocket();
	//Ϊ��ǰ�����socket�ļ��󶨶˿�
	sockaddr_in  saddr;					//����һ���洢���������IP��ַ���˿ںŵĽṹ�壬��������bind���ã���socket�ļ���
	saddr.sin_family = AF_INET;			//��ʶЭ�����ipv4��ַ��32λ�ģ���˿ںţ�16λ�ģ������
	saddr.sin_port = htons(port);		//�������ڼ����Ķ˿ں�,ע����htons���˿ںŵ��ֽ����Ϊ����ֽ���Big Endian�������ֽ����ʽ��
	saddr.sin_addr.s_addr = htonl(0);	//���ý��շ��ʵ�IP��ַ��0��ʶ����IP�����շ��ʣ�INADDR_ANYҲ�Ǳ�ʾ����IP���Խ��շ���
	/*��C++11�У�std�����ռ���Ҳ��һ��bind�������˴�Ҫ��ʹ��socket��bind��������bindǰ��::��ʶ��bind������ȫ���µģ�����ĳ�������ռ��µ�*/
	if (::bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)  //�󶨲������󶨳ɹ�����ֵΪ0������Ϊ0���ʧ��
	{
		cout << "Bind port " << port << " failed!" << endl << endl;
		return false;
	}
	//��ӡ����˰󶨼�����IP�Ͷ˿ں���Ϣ
	char *ip = inet_ntoa(saddr.sin_addr);
	cout << "The IP of server is picked from local. Please inquire your network card device!" << endl;
	cout << "Tcp server bind port " << port << " success!" << endl;
	//��ʼ�����󶨶˿ڵ�������Ϣ
	if (listen(sock, 1000) != 0)		//ָ�����ں�Ϊ���׽����Ŷӵ�������Ӹ���Ϊ1000,�����г���1000�����������δ���������
	{
		cout << "Listen port " << port << " failed!" << endl << endl;
		return false;
	}
	cout << "Tcp server is listening..." << endl << endl;

	return true;
}

//����һ���ͻ������ӣ�ͬʱ����һ���߳���֮�������ݽ���
XTcp XTcp::Accept()
{
	//Ϊ�ͻ��˴���һ��XTcp����
	XTcp tcp;
	sockaddr_in caddr;  //����һ���洢�������ӿͻ���IP��ַ���˿ںŵĽṹ��
	int len = sizeof(caddr);
	int client = accept(sock, (sockaddr*)&caddr, &len);  //����һ��ר���ڸÿͻ��˵����ݽ���socket�׽��֣��ͻ���IP���˿���Ϣ����caddr��
	if (client == -1)
	{
		cout << "Accept client failed!" << endl;
		return tcp;
	}
	//ת������Ŀͻ���IP���˿ڡ�socket�������Ϣ
	char *ipClient = inet_ntoa(caddr.sin_addr);
	strcpy(tcp.ip, ipClient);
	tcp.port = ntohs(caddr.sin_port);
	tcp.sock = client;
	cout << "Tcp server acceptclient! " << tcp.ip << ":" << tcp.port << " success!" << endl;

	return tcp;
}

//�رղ�����socket
void XTcp::Close()
{
	//�жϵ�ǰsocket�Ƿ�ռ������Դ
	if (sock <= 0) 
		return;
	//�رղ�����
	closesocket(sock);
	//��socket���������
	sock = 0;
}

//��������
int XTcp::Recv(char *buf, int bufsize)
{
	//�����յ�������д�뻺��buf��
	return recv(sock, buf, bufsize, 0);
}

//�������ݣ�ֻ����,ȷ����������ֽ���
int XTcp::Send(const char *buf, int sendsize)
{
	//�洢�ۼ��Ѿ��������ֽ���
	int s = 0;
	//�������ͣ�ֱ����Ŀ�귢�����ֽ���ȫ������
	while (s != sendsize)
	{
		int len = send(sock, buf + s, sendsize - s, 0);
		if (len <= 0)break;
		s += len;
	}

	//���ط������ֽ���
	return s;
}

//�ͻ����������ӷ����
bool XTcp::Connect(const char *ipServer, unsigned short portServer, int timeoutms)
{
	//�жϸÿͻ��˵�socket������Ƿ���Ч
	if (sock <= 0) 
		CreateSocket();
	//����Ŀ�����ӷ���˵�IP��ַ�Ͷ˿ں�
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(portServer);
	saddr.sin_addr.s_addr = inet_addr(ipServer);
	//��������Ϊ���������ӣ��������볬ʱʱ��Ĭ��Ϊ1�룬���޸�
	SetBlock(false);
	//ʹ��select��ѭ����ʵ�ֳ�ʱʱ����趨
	fd_set set;  //�ļ�������飬ÿһ������Ԫ�ض�����һ�򿪵��ļ����(socket���ļ����ܵ����豸��)
	if (connect(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0)  //�������ʧ��
	{
		FD_ZERO(&set);					//������ļ��������
		FD_SET(sock, &set);				//��sock�������飨sock������������
		timeval tm;						//���ó�ʱʱ����������0��ʶ������
		tm.tv_sec = 0;					//������
		tm.tv_usec = timeoutms * 1000;	//����΢�루����ĺ���Ҫ*1000��
		//�����ļ��������Ƿ��п�д(������1����ļ������+1��2�ɶ��ģ�3��д�ģ�4������5��ʱ����)
		if (select(sock + 1, 0, &set, 0, &tm) <= 0)  //�ж��ڹ涨ʱ���ڣ��Ƿ��п���д����ļ����ļ���������-1����ʱ����0
		{
			cout << "Connect server timeout or error!" << endl;
			printf("Connect server %s:%d failed! Error is :%s\n", ipServer, portServer, strerror(errno));
			return false;
		}
	}
	//�����ӻ�ԭΪ��������
	SetBlock(true);

	return true;
}

//��������ģʽ
bool XTcp::SetBlock(bool isblock)
{
	//�жϸÿͻ��˵�socket������Ƿ���Ч
	if (sock <= 0) 
		return false;
	//��ת��false��u1=0��Ϊ����������
	unsigned long ul = 0;
	//������true��u1=1�������������ӣ�acceptĬ����������
	if (!isblock) 
		ul = 1;
	//��������ģʽ
	ioctlsocket(sock, FIONBIO, &ul);

	return true;
}
