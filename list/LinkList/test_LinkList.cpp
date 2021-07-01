#include <iostream>
#include "LinkList.cpp"
#include <string>

using namespace std;

typedef struct Student
{
	string name;
	int age;
}Student;

void display()
{
	LinkList<Student> list;

	//����������������
	Student s1, s2, s3;
	s1.name = "xiao1";
	s1.age = 11;
	s2.name = "xiao2";
	s2.age = 22;
	s3.name = "xiao3";
	s3.age = 33;
	list.Insert(0, s1);
	list.Insert(0, s2);
	list.Insert(0, s3);
	for (int i = 0; i < list.getLength(); i++)
	{
		cout << list.Get(i).name << ":" << list.Get(i).age << endl;
	}
	cout << list.getLength() << endl;
	cout << endl;

	//ɾ������
	list.Delete(0);
	cout << "ɾ��ͷ�����ݺ�" << endl;
	for (int i = 0; i < list.getLength(); i++)
	{
		cout << list.Get(i).name << ":" << list.Get(i).age << endl;
	}
	cout << list.getLength() << endl;
	cout << endl;

	//��ȡ����ͷ������
	Student stu = list.Get(0);
	cout << "��ȡ��ǰ��ͷ���ڵ�" << endl;
	cout << stu.name << ":" << stu.age << endl << endl;

	//�������
	list.Clear();
	cout << "������ݺ�" << endl;
	for (int i = 0; i < list.getLength(); i++)
	{
		cout << list.Get(i).name << ":" << list.Get(i).age << endl;
	}
	cout << list.getLength() << endl;
	cout << endl;
}

int main(int argc, char *argv[])
{	
	for (int i = 0; i < 10000; i++)
	{
		display();
	}	

	cout << endl << "finish" << endl;
	getchar();
	return 0;
}