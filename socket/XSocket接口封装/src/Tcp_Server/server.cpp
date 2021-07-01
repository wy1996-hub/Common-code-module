#include <iostream>
#include <thread>

#include "XTcp.h"


using namespace std;

//�߳������
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

	//���÷���˼����Ķ˿ں�
	unsigned short port = 8081;
	if (argc > 1)
	{
		port = atoi(argv[1]);
	}

	//����TCP����˶���
	XTcp server;

	//Ϊ����˴���ͨ��socket
	server.CreateSocket();
	if (server.sock != -1)
	{
		cout << "Tcp server create success!" << endl << endl;
	}

	//Ϊ����˰�Ŀ������˿ںţ�ͬʱ��������
	server.Bind(port);

	//�����Ӽ����Ķ˿��л�ȡ������Ϣ
	for (;;)
	{
		//ÿ��һ��������Ϣ����һ��ר���ڸÿͻ��˵����ݽ���socket�׽���
		XTcp client = server.Accept();
		cout << "accept client " << client.sock << endl;
		//����һ���Զ����߳������
		TcpThread *clientThread = new TcpThread();
		clientThread->client = client;
		//����C++11���߳��ഴ���߳�
		thread newThread(&TcpThread::Main, clientThread);
		//�����̷߳���
		newThread.detach();
	}

	//�رշ���˼����׽���
	server.Close();
	cout << "Tcp_Server run success!" << endl;
	getchar();
	return 0;
}