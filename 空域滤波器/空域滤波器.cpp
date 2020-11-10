#include "空域滤波器.h"

Mat g_srcImage, g_dstImage1, g_dstImage2, g_dstImage3, g_dstImage4, g_dstImage5;

int g_nBoxFilterValue = 6;		//方框滤波的内核值
int g_nMeanBlurValue = 10;		//均值滤波的内核值
int g_nGaussianBlurValue = 6;	//高斯滤波内核值
int g_nMedianBlurBlurValue = 10;	//中值滤波参数
int g_nBilateralFiterValue = 5;	//双边滤波参数值

int main()
{
	g_srcImage = imread("0.bmp");
	if (!g_srcImage.data)
	{
		printf("图片载入失败!\n");
		return -1;
	}

	g_dstImage1 = g_srcImage.clone();
	g_dstImage2 = g_srcImage.clone();
	g_dstImage3 = g_srcImage.clone();
	g_dstImage4 = g_srcImage.clone();
	g_dstImage5 = g_srcImage.clone();

	//显示原图
	namedWindow("原图");
	imshow("原图", g_srcImage);

	//方框滤波
	namedWindow("方框滤波");
	//创建滚动条
	createTrackbar("内核值:", "方框滤波", &g_nBoxFilterValue, 50, on_BoxFilter);
	on_BoxFilter(g_nBoxFilterValue, 0);
	imshow("方框滤波", g_dstImage1);

	//均值滤波
	namedWindow("均值滤波");
	createTrackbar("内核值:", "均值滤波", &g_nMeanBlurValue, 50, on_MeanBulr);
	on_MeanBulr(g_nMeanBlurValue, 0);
	imshow("均值滤波", g_dstImage2);

	//高斯
	namedWindow("高斯滤波");
	createTrackbar("内核值:", "高斯滤波", &g_nGaussianBlurValue, 50, on_GaussianBulr);
	on_GaussianBulr(g_nGaussianBlurValue, 0);
	imshow("高斯滤波", g_dstImage3);

	//中值
	namedWindow("中值滤波");
	createTrackbar("内核值:", "中值滤波", &g_nMedianBlurBlurValue, 50, on_MedianBlur);
	on_MedianBlur(g_nMedianBlurBlurValue, 0);
	imshow("中值滤波", g_dstImage4);

	//双边
	namedWindow("双边滤波");
	createTrackbar("内核值:", "双边滤波", &g_nBilateralFiterValue, 50, on_BilateralFiter);
	on_BilateralFiter(g_nBilateralFiterValue, 0);
	imshow("双边滤波", g_dstImage5);

	waitKey(0);
	return 0;
}

//回调函数

//方框
static void on_BoxFilter(int, void*)
{
	//方框滤波操作
	boxFilter(g_srcImage, g_dstImage1, -1, Size(g_nBoxFilterValue + 1, g_nBoxFilterValue + 1));
	//显示
	imshow("方框滤波", g_dstImage1);
}

//均值
static void on_MeanBulr(int, void*)
{
	blur(g_srcImage, g_dstImage2, Size(g_nMeanBlurValue + 1, g_nMeanBlurValue + 1));
	imshow("均值滤波", g_dstImage2);
}

//高斯
static void on_GaussianBulr(int, void*)
{
	GaussianBlur(g_srcImage, g_dstImage3, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);	//大于1的奇数
	imshow("高斯滤波", g_dstImage3);
}

//中值
static void on_MedianBlur(int, void*)
{
	medianBlur(g_srcImage, g_dstImage4, g_nMedianBlurBlurValue * 2 + 1);
	imshow("中值滤波", g_dstImage4);
}

//双边
static void on_BilateralFiter(int, void*)
{
	bilateralFilter(g_srcImage, g_dstImage5, g_nBilateralFiterValue, g_nBilateralFiterValue * 2, g_nBilateralFiterValue / 2);
	imshow("双边滤波", g_dstImage5);
}