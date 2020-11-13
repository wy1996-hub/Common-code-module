#include "SVM.h"

void train()
{
	Mat trainingclasses;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;

	//��˳�������Ҫѵ�����ļ��е�ַ
	vector<string> targetpath;
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/С�γ�");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/�����");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/�ᳵ");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/�ж���");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/������");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/������");
	targetpath.emplace_back("I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/�ǻ�����");

	//��ȡ��������������������������ǩ����ת������������ݸ�ʽ
	for (int i = 0; i < targetpath.size(); i++)
	{
		getData(trainingImages, trainingLabels, targetpath[i], i);
	}

	//PCA��ά
	int PCANum = 1000;
	trainingImages.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(trainingclasses);
	PCA* pca = new PCA(trainingData, Mat(), CV_PCA_DATA_AS_ROW, PCANum);
	Mat trainingDatas = pca->project(trainingData);
	FileStorage PCAtrain_fs("0919_pca_linear.xml", FileStorage::WRITE);
	pca->write(PCAtrain_fs);
	PCAtrain_fs.release();
	delete pca;

	//����SVMѵ��������
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);
	//svm->setKernel(SVM::RBF);
	//svm->setKernel(SVM::POLY);
	svm->setC(0.1);
	//svm->setGamma(0.001);
	//svm->setCoef0(0.0);
	//svm->setDegree(3);
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 1000, FLT_EPSILON));
	Ptr<TrainData> traindata = ml::TrainData::create(trainingDatas, ROW_SAMPLE, trainingclasses);
	//ѵ��
	svm->train(traindata);
	//svm->trainAuto(traindata, 3);
	svm->save("0919_linear_pca.xml");
	//double res_c = svm->getC();
	//double res_gamma = svm->getGamma();
	//double res_coef = svm->getCoef0();
	//double res_degree = svm->getDegree();
	//cout << "C:" << res_c << endl;
	//cout << "Gamma:" << res_gamma << endl;
	//cout << "Coef0:" << res_coef << endl;
	//cout << "Degree:" << res_degree << endl;
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
		Mat p = Mat(descriptors).reshape(1, 1);//shape:(1,8100)
		trainingImages.push_back(p);
		trainingLabels.push_back(label);
	}
}