#include <iostream>
#include "SeqList.cpp"
#include <string>

using namespace std;

typedef struct Student
{
	string name = "";
	int age = 0;
}Student;

void display()
{
	//创建一个容量为10的顺序存储线性表
	SeqList<Student> list(10);

	//插入三个测试数据
	Student s1, s2, s3;
	s1.name = "xiao1";
	s1.age = 11;
	s2.name = "xiao2";
	s2.age = 22;
	s3.name = "xiao3";
	s3.age = 33;
	list.Insert(list.getLength(), s1);
	list.Insert(list.getLength(), s2);
	list.Insert(list.getLength(), s3);
	cout << "插入三个数据后" << endl;
	for (int i = 0; i < list.getLength(); i++)
	{
		cout << list.Get(i).name << ":" << list.Get(i).age << endl;
	}
	cout << list.getLength() << endl;
	cout << endl;

	//删除数据
	list.Delete(0);
	cout << "删除头部数据后" << endl;
	for (int i = 0; i < list.getLength(); i++)
	{
		cout << list.Get(i).name << ":" << list.Get(i).age << endl;
	}
	cout << list.getLength() << endl;
	cout << endl;

	//获取现在头部数据
	Student stu = list.Get(0);
	cout << "获取当前的头部节点" << endl;
	cout << stu.name << ":" << stu.age << endl << endl;

	//清空数据
	list.Clear();
	cout << "清空数据后" << endl;
	for (int i = 0; i < list.getLength(); i++)
	{
		cout << list.Get(i).name << ":" << list.Get(i).age << endl;
	}
	cout << list.getLength() << endl;
	cout << endl;
}

int main(int argc, char *argv[])
{
	for (int i = 0; i < 1000000; i++)
	{
		display();
	}
	

	cout << endl << "finish" << endl;
	getchar();
	return 0;
}