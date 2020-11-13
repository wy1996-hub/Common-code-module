#include "SVM.h"

void classify()
{
	string str;
	char * filePath = "I:/project/��������©��Ŀ/�������ѵ����/����/ѵ����/����/�����"; 
	vector<string> files;  
    getFiles(filePath, files);  
	int number = files.size();
    cout<<"�ļ���Ҫʶ���������:"<<number<<endl;
	Ptr<cv::ml::SVM> svm = Algorithm::load<ml::SVM>("1113test.xml");//��Ҫһ��δ����ʼ����ʵ��������ģ��
	cout << "svm loaded successly!" << endl;

	/*
	ʶ������У�����һ��ͼƬ��õ���cs�����ά����һ��ֻ��һ�еľ���
	֮����ͶӰ�õ���projectMat��һ��1*1����ֵΪ0�ľ���
	��ѵ��������ѵ�����������������ǽ���32ά�ģ�������û�취�õ���ѵ�������к�ѵ������ά��һ��������������
	����svm.predict�ͻ��б����Ӷ��ò���ʶ��Ľ����
	��ֻ��һ�еľ�����Э������������ֵ�ֽ���ȷʵ��ֻ��һ������������һ������ֵ��
	�ǲ��ܰ���PCA�����ٽ�ά�ģ��ǲ��Թ����о���Ҫ����һ�������ó����Խ������Ӧ����ô��PCA��ά�أ�
	---��Ҫ����Ͷ�ȡPCAģ��---
	*/
	/*FileStorage PCAclassify_fs("./ʵ���ļ�/4/0923_pca_linear_auto.xml", FileStorage::READ);
	PCA* pca = new PCA;
	pca->read(PCAclassify_fs.root());
	PCAclassify_fs.release();
	cout << "pca loaded successly!" << endl;*/

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

		//��PCA
		Mat p = Mat(descriptors).reshape(1, 1);
		p.convertTo(p, CV_32FC1);
		int response = (int)svm->predict(p);		
		
		//��PCA
		/*Mat trainingDatas = pca->project(p);
		trainingDatas.convertTo(trainingDatas, CV_32FC1);
		int response = (int)svm->predict(trainingDatas);*/
		
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
	cout<<"�ж��� numbers:"<<k3<<endl;
	cout<<"������ numbers:"<<k4<<endl;
	cout<<"������ numbers:"<<k5<<endl;
	cout <<"�ǻ����� numbers:"<< k6 << endl;

	//delete pca;
    return;
}

