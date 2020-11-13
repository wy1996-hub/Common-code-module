#include "SVM.h"

void classify()
{
	string str;
	char * filePath = "I:/project/垃圾车地漏项目/卡口相机训练库/二期/训练库/测试/面包车"; 
	vector<string> files;  
    getFiles(filePath, files);  
	int number = files.size();
    cout<<"文件夹要识别的数量是:"<<number<<endl;
	Ptr<cv::ml::SVM> svm = Algorithm::load<ml::SVM>("1113test.xml");//需要一个未经初始化的实例来载入模型
	cout << "svm loaded successly!" << endl;

	/*
	识别过程中，输入一张图片后得到的cs矩阵的维数是一个只有一行的矩阵，
	之后做投影得到的projectMat是一个1*1并且值为0的矩阵。
	而训练过程中训练样本的特征向量是降到32维的，这样就没办法得到和训练过程中和训练样本维数一样的特征向量，
	这样svm.predict就会有报错，从而得不到识别的结果。
	而只有一行的矩阵求协方差矩阵的特征值分解结果确实是只有一个特征向量和一个特征值，
	是不能按照PCA理论再降维的，那测试过程中就是要输入一个样本得出测试结果，那应该怎么用PCA降维呢？
	---需要保存和读取PCA模型---
	*/
	/*FileStorage PCAclassify_fs("./实验文件/4/0923_pca_linear_auto.xml", FileStorage::READ);
	PCA* pca = new PCA;
	pca->read(PCAclassify_fs.root());
	PCAclassify_fs.release();
	cout << "pca loaded successly!" << endl;*/

    for (int i = 0;i < number;i++)  
    {
		//方向梯度直方图测试
		Mat  src = imread(files[i].c_str());
		Mat srcImage;
		cvtColor(src, srcImage, CV_BGR2GRAY);
		resize(srcImage, srcImage, Size(128,128), (0,0), (0,0), CV_INTER_AREA);
		HOGDescriptor detector(Size(128,128), Size(16, 16), Size(8, 8), Size(8, 8), 9);
		vector<float> descriptors;
		detector.compute(srcImage, descriptors);

		//无PCA
		Mat p = Mat(descriptors).reshape(1, 1);
		p.convertTo(p, CV_32FC1);
		int response = (int)svm->predict(p);		
		
		//有PCA
		/*Mat trainingDatas = pca->project(p);
		trainingDatas.convertTo(trainingDatas, CV_32FC1);
		int response = (int)svm->predict(trainingDatas);*/
		
		if (response == 0)
        {	
			char resultpath[130];
			k0++;
			//sprintf(resultpath, "D:\\研究生学习工作文件夹\\工作项目文件夹\\银行安防项目\\test\\0509\\0\\%d.jpg", k0);
			//imwrite(resultpath, src);
			
        }
		else if (response == 1)
        {
			char resultpath[130];
			/*sprintf(resultpath, "D:/研究生学习工作文件夹/工作项目文件夹/垃圾车地漏项目/训练集/卡口相机训练库/20200521-2/白天2/分类器3/面包车测试/面包车/%d.jpg", k1);
			imwrite(resultpath, src);*/
			k1++;
		}
		else if (response ==2)
		{
			char resultpath[130];
			/*sprintf(resultpath, "D:/研究生学习工作文件夹/工作项目文件夹/垃圾车地漏项目/训练集/卡口相机训练库/20200521-2/白天2/分类器3/面包车测试/厢车/%d.jpg", k2);
			imwrite(resultpath, src);*/
			k2++;
		}
		else if (response ==3)
		{
			char resultpath[130];
			/*sprintf(resultpath, "D:/研究生学习工作文件夹/工作项目文件夹/垃圾车地漏项目/训练集/卡口相机训练库/20200521-2/白天2/分类器3/面包车测试/有斗车/小%d.jpg", k3);
			imwrite(resultpath, src);*/	
			k3++;
		}
		else if (response ==4)
		{
			char resultpath[130];	
			/*sprintf(resultpath, "D:/研究生学习工作文件夹/工作项目文件夹/垃圾车地漏项目/训练集/卡口相机训练库/20200521-2/白天2/分类器3/面包车测试/有斗车/中%d.jpg", k4);
			imwrite(resultpath, src);	*/
			k4++;
		}
		else if (response ==5)
		{
			char resultpath[130];
			/*sprintf(resultpath, "D:/研究生学习工作文件夹/工作项目文件夹/垃圾车地漏项目/训练集/卡口相机训练库/20200521-2/白天2/分类器3/面包车测试/有斗车/大%d.jpg", k5);
			imwrite(resultpath, src);	*/
			k5++;
		}
		else if (response ==6)
		{	
			char resultpath[130];
			k6++;
			//sprintf(resultpath, "D:\\研究生学习工作文件夹\\工作项目文件夹\\银行安防项目\\test\\头无手机-正面手机\\%d.jpg", k5);
			//imwrite(resultpath, src);			
		}
    }
	cout<<"轿车 numbers:"<<k0<<endl;
	cout<<"面包车 numbers:"<<k1<<endl;
	cout<<"厢车 numbers:"<<k2<<endl;
	cout<<"有斗车 numbers:"<<k3<<endl;
	cout<<"垃圾车 numbers:"<<k4<<endl;
	cout<<"渣土车 numbers:"<<k5<<endl;
	cout <<"非机动车 numbers:"<< k6 << endl;

	//delete pca;
    return;
}

