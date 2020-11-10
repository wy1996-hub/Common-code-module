#include <opencv.hpp>
#include <iostream>
#include <io.h>  //getFilesͷ�ļ�

using namespace std;
using namespace cv;

void getFiles( string path, vector<string>& files );//��ָ���ļ����ж�ȡ����ͼƬ
void getData(Mat& trainingImages, vector<int>& trainingLabels, string& filePath,int label);//��ָ���ļ���ȡ��ͼƬ����ȡ��������������������������ǩ

void main()
{
	Mat trainingclasses;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;
	
	//��˳�������Ҫѵ�����ļ��е�ַ
	vector<string> targetpath;	
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/20200521-2/������Ż�/44.129.6.34/�γ�-����");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/20200521-2/������Ż�/44.129.6.34/�����-����");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/20200521-2/������Ż�/44.129.6.34/�ᳵ-����");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/20200521-2/������Ż�/44.129.6.34/С���ж���-����");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/20200521-2/������Ż�/44.129.6.34/�����ж���-����");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/20200521-2/������Ż�/44.129.6.34/�����ж���-����");

	//��ȡ��������������������������ǩ����ת������������ݸ�ʽ
	for (int i = 0;i<targetpath.size(); i++)
	{
		getData(trainingImages, trainingLabels, targetpath[i],i);
	}

	trainingImages.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(trainingclasses);
	//����SVMѵ��������
	CvSVMParams SVM_params;
	SVM_params.svm_type = CvSVM::C_SVC;
	SVM_params.kernel_type = CvSVM::LINEAR;
	SVM_params.C = 1;
	SVM_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
	//ѵ��
	CvSVM svm;
	svm.train(trainingData, trainingclasses, Mat(), Mat(), SVM_params);
	svm.save("6.34test.xml");
	cout<<"ѵ�����ˣ�����"<<endl;
	getchar();
	return;
}

void getFiles( string path, vector<string>& files )  
{  
	long   hFile   =   0;  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
	{  
		do  
		{  
			if((fileinfo.attrib &  _A_SUBDIR))  
			{  
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
					getFiles( p.assign(path).append("\\").append(fileinfo.name), files );  
			}  
			else  
			{  
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
			}  
		}while(_findnext(hFile, &fileinfo)  == 0);  

		_findclose(hFile);  
	}  
}
void getData(Mat& trainingImages, vector<int>& trainingLabels, string filePath, int label)
{
	vector<string> files;  
	getFiles(filePath.c_str(), files);  
	int number = files.size();
	for (int i = 0; i < number; i++)
	{
		Mat  srcImage = imread(files[i].c_str(), 0);
		resize(srcImage, srcImage, Size(128,128), (0,0), (0,0), CV_INTER_AREA);
		HOGDescriptor detector(Size(128,128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
		vector<float> descriptors;
		detector.compute(srcImage, descriptors);
		Mat p = Mat(descriptors).reshape(1, 1);
		trainingImages.push_back(p);
		trainingLabels.push_back(label);
	}
}