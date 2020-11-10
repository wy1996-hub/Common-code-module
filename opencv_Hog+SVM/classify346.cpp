#include <iostream>
#include<opencv.hpp>
#include <io.h>

using namespace std;
using namespace cv;
using namespace cv::ml;

/************************************************************************/
/* ��������������*/
/************************************************************************/
static int k0=0,k1=0,k2=0,k3=0,k4=0,k5=0,k6=0,k7=0,k8=0;
int number;
void getFiles( string path, vector<string>& files ); 

/************************************************************************/
/*����ʵ��*/
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
	char * filePath = "I:/project/��������©��Ŀ/�������ѵ����/20200521-2/������Ż�/44.129.6.34/�����ж���-����"; 
	vector<string> files;  
    getFiles(filePath, files);  
    number = files.size();  
    cout<<"�ļ���Ҫʶ���������:"<<number<<endl;
	Ptr<cv::ml::SVM> svm = SVM::create();
    svm->clear();
	//xml�ļ���ַ
	string modelpath = "6.34test.xml";
    FileStorage svm_fs(modelpath,FileStorage::READ);
    if(svm_fs.isOpened())
    {
		svm->load(modelpath.c_str());
		cout << svm->getSupportVectors() << endl;
    }	
    for (int i = 0;i < number;i++)  
    {
		//�����ݶ�ֱ��ͼ����
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
			//sprintf(resultpath, "D:\\�о���ѧϰ�����ļ���\\������Ŀ�ļ���\\���а�����Ŀ\\test\\0509\\0\\%d.jpg", k0);
			//imwrite(resultpath, src);
			
        }
		else if (response == 1)
        {
			char resultpath[130];
			/*sprintf(resultpath, "D:/�о���ѧϰ�����ļ���/������Ŀ�ļ���/��������©��Ŀ/ѵ����/�������ѵ����/20200521-2/����2/������3/���������/�����/%d.jpg", k1);
			imwrite(resultpath, src);*/
			k1++;
		}
		else if (response ==2)
		{
			char resultpath[130];
			/*sprintf(resultpath, "D:/�о���ѧϰ�����ļ���/������Ŀ�ļ���/��������©��Ŀ/ѵ����/�������ѵ����/20200521-2/����2/������3/���������/�ᳵ/%d.jpg", k2);
			imwrite(resultpath, src);*/
			k2++;
		}
		else if (response ==3)
		{
			char resultpath[130];
			/*sprintf(resultpath, "D:/�о���ѧϰ�����ļ���/������Ŀ�ļ���/��������©��Ŀ/ѵ����/�������ѵ����/20200521-2/����2/������3/���������/�ж���/С%d.jpg", k3);
			imwrite(resultpath, src);*/	
			k3++;
		}
		else if (response ==4)
		{
			char resultpath[130];	
			/*sprintf(resultpath, "D:/�о���ѧϰ�����ļ���/������Ŀ�ļ���/��������©��Ŀ/ѵ����/�������ѵ����/20200521-2/����2/������3/���������/�ж���/��%d.jpg", k4);
			imwrite(resultpath, src);	*/
			k4++;
		}
		else if (response ==5)
		{
			char resultpath[130];
			/*sprintf(resultpath, "D:/�о���ѧϰ�����ļ���/������Ŀ�ļ���/��������©��Ŀ/ѵ����/�������ѵ����/20200521-2/����2/������3/���������/�ж���/��%d.jpg", k5);
			imwrite(resultpath, src);	*/
			k5++;
		}
		else if (response ==6)
		{	
			char resultpath[130];
			k6++;
			//sprintf(resultpath, "D:\\�о���ѧϰ�����ļ���\\������Ŀ�ļ���\\���а�����Ŀ\\test\\ͷ���ֻ�-�����ֻ�\\%d.jpg", k5);
			//imwrite(resultpath, src);			
		}
    }
	cout<<"�γ� numbers:"<<k0<<endl;
	cout<<"����� numbers:"<<k1<<endl;
	cout<<"�ᳵ numbers:"<<k2<<endl;
	cout<<"С�ж��� numbers:"<<k3<<endl;
	cout<<"���ж��� numbers:"<<k4<<endl;
	cout<<"���ж��� numbers:"<<k5<<endl;
    return;
}

void main(){
	classify();	
	system("pause");
	return;
}