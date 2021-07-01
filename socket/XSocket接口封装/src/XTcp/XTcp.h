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
	//����һ��TcpЭ���socket�׽����ļ�
	int CreateSocket();

	//Ϊ�����socket�׽��ְ�һ�������˿ںţ�ͬʱ��������
	bool Bind(unsigned short port);

	//����һ���ͻ������ӣ�ͬʱ����һ���߳���֮�������ݽ���(���ݷ��ض����sockֵ�жϲ����Ƿ�ɹ�)
	XTcp Accept();

	//�رղ�����socket
	void Close();

	//��������
	int Recv(char *buf, int bufsize);

	//�������ݣ�ֻ����,ȷ����������ֽ���
	int Send(const char *buf, int sendsize);

	//�ͻ����������ӷ����,Ĭ���������ӳ�ʱΪ1��
	bool Connect(const char *ip, unsigned short port, int timeoutms = 1000);

	//��������ģʽ
	bool SetBlock(bool isblock);

public:
	//�洢socket�׽��־��ID
	int sock = 0;
	//�洢�˿ں�
	unsigned short port = 0;
	//�洢IP��ַ
	char ip[16];
};

