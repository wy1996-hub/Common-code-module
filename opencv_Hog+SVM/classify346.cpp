#include <iostream>
#include<opencv.hpp>
#include <io.h>

using namespace std;
using namespace cv;
using namespace cv::ml;

/************************************************************************/
/* 函数及变量声明*/
/************************************************************************/
static int k0=0,k1=0,k2=0,k3=0,k4=0,k5=0,k6=0,k7=0,k8=0;
int number;
void getFiles( string path, vector<string>& files ); 

/************************************************************************/
/*函数实现*/
/************************************************************************/
void getFiles( string path, vector<string>& files )  
{  
	intptr_t hFile   =   0;  
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

void classify()
{
	string str;
	char * filePath = "I:/project/垃圾车地漏项目/卡口相机训练库/20200521-2/针对性优化/44.129.6.34/中型有斗车-白天"; 
	vector<string> files;  
    getFiles(filePath, files);  
    number = files.size();  
    cout<<"文件夹要识别的数量是:"<<number<<endl;
	Ptr<cv::ml::SVM> svm = SVM::create();
    svm->clear();
	//xml文件地址
	string modelpath = "6.34test.xml";
    FileStorage svm_fs(modelpath,FileStorage::READ);
    if(svm_fs.isOpened())
    {
		svm->load(modelpath.c_str());
		cout << svm->getSupportVectors() << endl;
    }	
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
		Mat p = Mat(descriptors).reshape(1, 1);

        p.convertTo(p, CV_32FC1);
		int response = (int)svm->predict(p);
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
	cout<<"小有斗车 numbers:"<<k3<<endl;
	cout<<"中有斗车 numbers:"<<k4<<endl;
	cout<<"大有斗车 numbers:"<<k5<<endl;
    return;
}

void main(){
	classify();	
	system("pause");
	return;
}