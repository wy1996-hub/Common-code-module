#include <iostream>
#include <opencv2\opencv.hpp>
#include "LXMysql.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	LXMysql my;
	my.Init();
	my.Close();

	cout << "finish" << endl;
	getchar();
	return 0;
}