#include <iostream>
#include <windows.h>
#include <thread>

using namespace std;

class TcpThread
{
public:
	void Main()
	{
		//���ڴ洢�ͻ��˷���������
		char buf[1024] = { 0 };
		//������ͻ��˽������ݽ���
		for (;;)
		{
			char buf[1024] = { 0 };
			int recvlen = recv(idClient, buf, sizeof(buf) - 1, 0);  //��client�ļ�����һ�ζ�ȡ���sizeof(buf)-1�����ݣ���һ��д\0
			if (recvlen <= 0 || strstr(buf, "quit") != NULL)  //������յ�����Ϊ�ջ��߽��յ�"quit"�����ٽ��գ��˳�
			{
				char re[] = "quit success!\n";
				send(idClient, re, strlen(re) + 1, 0);
				cout << "idClient " << idClient << " quit success!" << endl << endl;
				break;
			}
			buf[recvlen] = '\0';  //���յ����ִ������Զ���ĩβ��0����������Լ����
			int sendlen = send(idClient, "ok\n", 4, 0);
			cout << "idClient " << idClient << " recv:" << buf << endl;
		}
		//�ͷ��׽���ռ�õ���Դ
		closesocket(idClient);
		//�ͷŸ��̶߳���ռ�õ���Դ
		delete this;
	}
	int idClient = 0;
};

int main(int argc, char *argv[])
{
	//��ʼ����̬���ӿ⣬��Ҫ���������ws2_32.lib
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	//1.��������ʼ��һ�����ڼ����˿ڵ�socket�׽��֣�AF_INET��ʶ��Э�����ipv4��ַ��32λ�ģ���˿ںţ�16λ�ģ�����ϣ�SOCK_STREAM��ʶ��socket��TCP����
	int sock = socket(AF_INET, SOCK_STREAM, 0);  //����һ��socket��ζ��Ϊһ��socket���ݽṹ����洢�ռ�
	if (sock == -1)
	{
		cout << "����ͨ��socketʧ�ܣ�" << endl << endl;
		getchar(); return -1;
	}
	else
	{
		cout << "����ͨ��socket�ɹ���" << endl << endl;
	}

	//2.��������socket�׽����ļ��󶨵�ָ����IP��ַ�Ͷ˿���
	unsigned short port = 8081;  //�������ڼ������ݵĶ˿ںţ���0ʱ��ϵͳ���ѡ��һ��δ��ʹ�õĶ˿ںţ�
	if (argc > 1) //���ͻ��˴����˶˿ںţ����ÿͻ��˴���Ķ˿ںŽ��м���
	{
		port = atoi(argv[1]);
	}
	sockaddr_in saddr;					//����һ���洢���������IP��ַ���˿ںŵĽṹ�壬��������bind���ã���socket�ļ���
	saddr.sin_family = AF_INET;			//��ʶЭ�����ipv4��ַ��32λ�ģ���˿ںţ�16λ�ģ������
	saddr.sin_port = htons(port);		//�������ڼ����Ķ˿ں�,ע����htons���˿ںŵ��ֽ����Ϊ����ֽ���Big Endian�������ֽ����ʽ��
	saddr.sin_addr.s_addr = htonl(0);	//���ý��շ��ʵ�IP��ַ��0��ʶ����IP�����շ��ʣ�INADDR_ANYҲ�Ǳ�ʾ����IP���Խ��շ���
	int re = 0;
	re = ::bind(sock, (sockaddr*)&saddr, sizeof(saddr));  //�󶨲������󶨳ɹ�����ֵΪ0������Ϊ0���ʧ��
	/*��C++11�У�std�����ռ���Ҳ��һ��bind�������˴�Ҫ��ʹ��socket��bind��������bindǰ��::��ʶ��bind������ȫ���µģ�����ĳ�������ռ��µ�*/
	if (re != 0)
	{
		printf("bind port %d failed!\n", port);
		getchar(); return -2;
	}
	else
	{
		cout << "bind port " << port << " success!" << endl << endl;
	}

	//3.��ʼ����socket�ļ��󶨵Ķ˿���Ϣ
	re = listen(sock, 10);  //ָ�����ں�Ϊ���׽����Ŷӵ�������Ӹ���Ϊ10,�����г���10�����������δ���������
	if (re != 0)
	{
		cout << "listen port failed!" << endl << endl;
		getchar(); return -3;
	}
	else
	{
		cout << "listen port success!" << endl << endl;
	}

	//4.���ϴӼ����Ķ˿��л�ȡ������Ϣ��ÿ��һ��������Ϣ����һ��ר���ڸÿͻ��˵����ݽ���socket�׽���
	for (;;)
	{
		//���������Ƿ����µĿͻ�������
		sockaddr_in caddr;			//����һ���洢�������ӿͻ���IP��ַ���˿ںŵĽṹ��
		int clen = sizeof(caddr);
		int client = accept(sock, (sockaddr*)&caddr, &clen);  //����һ��ר���ڸÿͻ��˵����ݽ���socket�׽��֣��ͻ���IP���˿ڵ���Ϣ����caddr��
		if (client == -1)
		{
			cout << "�������ݽ���socketʧ�ܣ�" << endl << endl;
			getchar(); return -1;
		}
		else
		{
			cout << "�������ݽ���socket�ɹ���" << endl;
			char *ip = inet_ntoa(caddr.sin_addr);			//��caddr��ȡ���ͻ��˵�IP
			unsigned short cport = ntohs(caddr.sin_port);	//��caddr��ȡ���ͻ��˵Ķ˿ںţ�תΪС�ͻ��ֽ���С���ֽ���Little Endian��
			printf("�ÿͻ���IPΪ��%s, �˿ں�Ϊ��%d\n", ip, cport);
			cout << endl;
		}
		//Ϊ������Ŀͻ��˴����̣߳�����ʵ�����ݽ���
		TcpThread *clientThread = new TcpThread();
		clientThread->idClient = client;  //���ÿͻ��˾���Ŵ����ö���
		thread newTread(&TcpThread::Main, clientThread);
		newTread.detach();
	}

	

	//�رշ���˼���socket
	closesocket(sock);


	cout << "=================================" << endl << "socketTest run success!" << endl;
	getchar(); return 0;
}