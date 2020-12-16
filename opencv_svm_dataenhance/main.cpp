#include <opencv.hpp>
#include <iostream>
#include <io.h>  //getFilesͷ�ļ�
#include <time.h>

using namespace std;
using namespace cv;
using namespace cv::ml;

void getFiles(string path, vector<string>& files);//��ָ���ļ����ж�ȡ����ͼƬ
void getData(Mat& trainingImages, vector<int>& trainingLabels, string& filePath, int label);//��ָ���ļ���ȡ��ͼƬ����ȡ��������������������������ǩ
vector<int> createRandomNums(int N, int maxVal);//�����������
float createRandomProbability();//�������Probability
vector<Mat> enhance(Mat& srcImage);//ͼ������

void main()
{
	Mat trainingclasses;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;

	//��˳�������Ҫѵ�����ļ��е�ַ
	vector<string> targetpath;
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/0912��/�ǻ�����");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/0912��/�����");

	//��ȡ��������������������������ǩ����ת������������ݸ�ʽ
	for (int i = 0; i < targetpath.size(); i++)
	{
		getData(trainingImages, trainingLabels, targetpath[i], i);
	}

	trainingImages.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(trainingclasses);
	//����SVMѵ��������
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	svm->setC(1);
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 1000, 0.01));
	//ѵ��
	svm->train(trainingData, ROW_SAMPLE, trainingclasses);
	svm->save("1113test_enhance.xml");
	cout << "ѵ�����ˣ�����" << endl;

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

vector<int> createRandomNums(int N, int maxVal)
{
	vector<int> nums;
	int RandomNumber;
	srand((unsigned)time(NULL));
	for (int i = 0; i < N; i++)
	{
		RandomNumber = rand() % maxVal + 1;
		nums.emplace_back(RandomNumber);
	}
	return nums;
}

float createRandomProbability()
{
	const int maxVal = 100;
	float RandomNumber;
	srand((unsigned)time(NULL));
	RandomNumber = rand() % maxVal / (float)(maxVal * 5);
	return RandomNumber;
}

vector<Mat> enhance(Mat& srcImage)
{
	vector<Mat> out;
	out.emplace_back(srcImage);

	Mat imgFlip;
	flip(srcImage, imgFlip, 1);
	out.emplace_back(imgFlip);
	//imwrite("test/1.jpg", imgFlip);

	vector<int>& pos = createRandomNums(4, 40);
	Mat shift = srcImage(Rect(pos[0], pos[1], srcImage.cols - pos[0] - pos[2], srcImage.rows - pos[1] - pos[2]));
	resize(shift, shift, Size(128, 128), (0, 0), (0, 0), CV_INTER_AREA);
	out.emplace_back(shift);
	//imwrite("test/2.jpg", shift);

	Mat bright = srcImage.clone();
	float alpha = createRandomProbability();
	vector<int> beta = createRandomNums(1, 25);
	for (int row = 0; row < srcImage.rows; row++)
	{
		uchar* psrcImage = srcImage.ptr<uchar>(row);
		uchar* pbright = bright.ptr<uchar>(row);
		for (int col = 0; col < srcImage.cols; col++)
		{
			pbright[col] = saturate_cast<uchar>((int)(psrcImage[col] * (1 + alpha - 0.3)) + beta[0]);
		}
	}
	out.emplace_back(bright);
	//imwrite("test/3.jpg", bright);

	return out;
}

void getData(Mat& trainingImages, vector<int>& trainingLabels, string& filePath, int label)
{
	vector<string> files;
	getFiles(filePath.c_str(), files);
	int number = files.size();
	for (int i = 0; i < number; i++)
	{
		Mat srcImage = imread(files[i].c_str(), 0);
		resize(srcImage, srcImage, Size(128, 128), (0, 0), (0, 0), CV_INTER_AREA);
		if (filePath == "I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/0912��/�ǻ�����")
		{
			vector<Mat> enhanceImg = enhance(srcImage);
			HOGDescriptor detector(Size(128, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
			vector<float> descriptors;
			for (size_t i = 0; i < enhanceImg.size(); i++)
			{
				detector.compute(enhanceImg[i], descriptors);
				Mat p = Mat(descriptors).reshape(1, 1);
				trainingImages.push_back(p);
				trainingLabels.push_back(label);
				descriptors.clear();
			}
		}
		else
		{
			HOGDescriptor detector(Size(128, 128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
			vector<float> descriptors;
			detector.compute(srcImage, descriptors);
			Mat p = Mat(descriptors).reshape(1, 1);
			trainingImages.push_back(p);
			trainingLabels.push_back(label);
		}	
	}
}