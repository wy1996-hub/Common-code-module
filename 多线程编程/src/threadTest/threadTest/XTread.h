#pragma once
class XTread
{
public:
	XTread();
	virtual ~XTread();

public:

	//�����߳�
	bool Start();

	//�߳�ִ�����ݣ����麯�����������û��̳к��Զ���ʵ��
	virtual void Main() = 0;

	//�ȴ��߳̽���������������
	void Wait();

private:
	//�߳̾����
	unsigned int idThread = 0;
};

