#include "空域边缘检测.h"

void main() {
	Mat src = imread("dst4.bmp");

	SobelDT(src);
	LaplacianDT(src);
	ScharrDT(src);
	CannyDT(src);

	destroyAllWindows();
	return;
}

void SobelDT(Mat& src)
{
	Mat grad_x, grad_y, dst;

	namedWindow("原图", WINDOW_NORMAL);
	imshow("原图", src);

	//x方向
	Sobel(src, grad_x, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);

	namedWindow("x方向", WINDOW_NORMAL);
	imshow("x方向", grad_x);

	//y方向
	Sobel(src, grad_y, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	namedWindow("y方向", WINDOW_NORMAL);
	imshow("y方向", grad_y);

	//合并的
	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dst);
	namedWindow("x+y", WINDOW_NORMAL);
	imshow("x+y", dst);

	waitKey(0);
}

void LaplacianDT(Mat& src)
{
	Mat src_gray, dst;

	namedWindow("原图", WINDOW_NORMAL);
	imshow("原图", src);

	//用高斯滤波降噪
	Mat g_src;
	GaussianBlur(src, g_src, Size(3, 3), 0, 0);

	//灰度图像
	cvtColor(g_src, src_gray, COLOR_RGB2GRAY);

	//开始边缘检测(Laplacian)
	Laplacian(src_gray, dst, CV_8U, 9, 1, 0, BORDER_DEFAULT);

	namedWindow("效果图", WINDOW_NORMAL);
	imshow("效果图", dst);

	waitKey(0);
}

void ScharrDT(Mat& src)
{
	namedWindow("原图", WINDOW_NORMAL);
	imshow("原图", src);

	//x方向
	Mat grad_x;
	Scharr(src, grad_x, CV_8U, 1, 0, 1, 0, BORDER_DEFAULT);
	namedWindow("x方向", WINDOW_NORMAL);
	imshow("x方向", grad_x);

	//y方向
	Mat grad_y;
	Scharr(src, grad_y, CV_8U, 0, 1, 1, 0, BORDER_DEFAULT);
	namedWindow("y方向", WINDOW_NORMAL);
	imshow("y方向", grad_y);

	//x+y方向
	Mat dst;
	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dst);
	namedWindow("x+y", WINDOW_NORMAL);
	imshow("x+y", dst);

	waitKey(0);
}

void CannyDT(Mat& src)
{
	Mat src1 = src;
	Mat out;

	namedWindow("原图", WINDOW_NORMAL);
	imshow("原图", src);

	//方法一3以下的版本
	Canny(src, out, 150, 100);
	namedWindow("方法一的效果图", WINDOW_NORMAL);
	imshow("方法一的效果图", out);

	//方法二3以上的版本
	Mat dst, edge, gray;

	//创建一个与src1一样的矩阵
	dst.create(src1.size(), src1.type());

	//将原图像转为灰度
	cvtColor(src1, gray, COLOR_RGB2GRAY);

	//滤波(降噪)
	blur(gray, edge, Size(3, 3));

	//canny
	Mat out2;
	Canny(edge, out2, 15, 10);


	dst = Scalar::all(0);

	src1.copyTo(dst, out2);

	namedWindow("方法二的效果图", WINDOW_NORMAL);
	imshow("方法二的效果图", out2);

	waitKey(0);
}