#include "�����˲���.h"

Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4, g_dstImage5;

int g_nBoxFilterValue = 6;		//�����˲����ں�ֵ
int g_nMeanBlurValue = 10;		//��ֵ�˲����ں�ֵ
int g_nGaussianBlurValue = 6;	//��˹�˲��ں�ֵ
int g_nMedianBlurBlurValue = 10;	//��ֵ�˲�����
int g_nBilateralFiterValue = 5;	//˫���˲�����ֵ

int main()
{
	g_srcImage = imread("0.bmp");
	if (!g_srcImage.data)
	{
		printf("ͼƬ����ʧ��!\n");
		return -1;
	}

	g_dstImage1 = g_srcImage.clone();
	g_dstImage2 = g_srcImage.clone();
	g_dstImage3 = g_srcImage.clone();
	g_dstImage4 = g_srcImage.clone();
	g_dstImage5 = g_srcImage.clone();

	//��ʾԭͼ
	namedWindow("ԭͼ");
	imshow("ԭͼ", g_srcImage);

	//�����˲�
	namedWindow("�����˲�");
	//����������
	createTrackbar("�ں�ֵ:", "�����˲�", &g_nBoxFilterValue, 50, on_BoxFilter);
	on_BoxFilter(g_nBoxFilterValue, 0);
	imshow("�����˲�", g_dstImage1);

	//��ֵ�˲�
	namedWindow("��ֵ�˲�");
	createTrackbar("�ں�ֵ:", "��ֵ�˲�", &g_nMeanBlurValue, 50, on_MeanBulr);
	on_MeanBulr(g_nMeanBlurValue, 0);
	imshow("��ֵ�˲�", g_dstImage2);

	//��˹
	namedWindow("��˹�˲�");
	createTrackbar("�ں�ֵ:", "��˹�˲�", &g_nGaussianBlurValue, 50, on_GaussianBulr);
	on_GaussianBulr(g_nGaussianBlurValue, 0);
	imshow("��˹�˲�", g_dstImage3);

	//��ֵ
	namedWindow("��ֵ�˲�");
	createTrackbar("�ں�ֵ:", "��ֵ�˲�", &g_nMedianBlurBlurValue, 50, on_MedianBlur);
	on_MedianBlur(g_nMedianBlurBlurValue, 0);
	imshow("��ֵ�˲�", g_dstImage4);

	//˫��
	namedWindow("˫���˲�");
	createTrackbar("�ں�ֵ:", "˫���˲�", &g_nBilateralFiterValue, 50, on_BilateralFiter);
	on_BilateralFiter(g_nBilateralFiterValue, 0);
	imshow("˫���˲�", g_dstImage5);

	waitKey(0);
	return 0;
}

//�ص�����

//����
static void on_BoxFilter(int, void*)
{
	//�����˲�����
	boxFilter(g_srcImage, g_dstImage1, -1, Size(g_nBoxFilterValue + 1, g_nBoxFilterValue + 1));
	//��ʾ
	imshow("�����˲�", g_dstImage1);
}

//��ֵ
static void on_MeanBulr(int, void*)
{
	blur(g_srcImage, g_dstImage2, Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1));
	imshow("��ֵ�˲�", g_dstImage2);
}

//��˹
static void on_GaussianBulr(int, void*)
{
	GaussianBlur(g_srcImage, g_dstImage3, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);	//����1������
	imshow("��˹�˲�", g_dstImage3);
}

//��ֵ
static void on_MedianBlur(int, void*)
{
	medianBlur(g_srcImage, g_dstImage4, g_nMedianBlurBlurValue * 2 + 1);
	imshow("��ֵ�˲�", g_dstImage4);
}

//˫��
static void on_BilateralFiter(int, void*)
{
	bilateralFilter(g_srcImage, g_dstImage5, g_nBilateralFiterValue, g_nBilateralFiterValue * 2, g_nBilateralFiterValue / 2);
	imshow("˫���˲�", g_dstImage5);
}