#include <opencv.hpp>
#include <iostream>
#include <io.h>  //getFiles头文件

using namespace std;
using namespace cv;
using namespace cv::ml;

void getFiles(string path, vector<string>& files);//从指定文件夹中读取所有图片
void getData(Mat& trainingImages, vector<int>& trainingLabels, string& filePath, int label);//从指定文件读取夹图片，提取特征，生成特征向量与特征标签

void main()
{
	Mat trainingclasses;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;

	//按顺序添加需要训练的文件夹地址
	vector<string> targetpath;
	targetpath.emplace_back("I:/桌面/test/轿车");
	targetpath.emplace_back("I:/桌面/test/厢车");
	targetpath.emplace_back("I:/桌面/test/有斗车");
	targetpath.emplace_back("I:/project/垃圾车地漏项目/卡口相机训练库/20200521-2/针对性优化/44.129.6.34/小型有斗车-白天");
	targetpath.emplace_back("I:/project/垃圾车地漏项目/卡口相机训练库/20200521-2/针对性优化/44.129.6.34/中型有斗车-白天");
	targetpath.emplace_back("I:/project/垃圾车地漏项目/卡口相机训练库/20200521-2/针对性优化/44.129.6.34/大型有斗车-白天");

	//提取特征，生成特征向量与特征标签，并转化成所需的数据格式
	for (int i = 0; i < targetpath.size(); i++)
	{
		getData(trainingImages, trainingLabels, targetpath[i], i);
	}

	trainingImages.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(trainingclasses);
	//配置SVM训练器参数
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setC(1);
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 1000, 0.01));
	//训练
	svm->train(trainingData, ROW_SAMPLE, trainingclasses);
	svm->save("0910test.xml");
	cout << "训练好了！！！" << endl;

	getchar();
	return;
}

void getFiles(string path, vector<string>& files)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void getData(Mat& trainingImages, vector<int>& trainingLabels, string& filePath, int label)
{
	vector<string> files;
	getFiles(filePath.c_str(), files);
	int number = files.size();
	for (int i = 0; i < number; i++)
	{
		Mat  srcImage = imread(files[i].c_str(), 0);
		resize(srcImage, srcImage, Size(128, 128), (0, 0), (0, 0), CV_INTER_AREA);
		HOGDescriptor detector(Size(128, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
		vector<float> descriptors;
		detector.compute(srcImage, descriptors);
		Mat p = Mat(descriptors).reshape(1, 1);
		trainingImages.push_back(p);
		trainingLabels.push_back(label);
	}
}