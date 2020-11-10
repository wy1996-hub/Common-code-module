#include "�����Ե���.h"

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

	namedWindow("ԭͼ", WINDOW_NORMAL);
	imshow("ԭͼ", src);

	//x����
	Sobel(src, grad_x, CV_8U, 1, 0, 3, 1, 0, BORDER_DEFAULT);

	namedWindow("x����", WINDOW_NORMAL);
	imshow("x����", grad_x);

	//y����
	Sobel(src, grad_y, CV_8U, 0, 1, 3, 1, 0, BORDER_DEFAULT);
	namedWindow("y����", WINDOW_NORMAL);
	imshow("y����", grad_y);

	//�ϲ���
	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dst);
	namedWindow("x+y", WINDOW_NORMAL);
	imshow("x+y", dst);

	waitKey(0);
}

void LaplacianDT(Mat& src)
{
	Mat src_gray, dst;

	namedWindow("ԭͼ", WINDOW_NORMAL);
	imshow("ԭͼ", src);

	//�ø�˹�˲�����
	Mat g_src;
	GaussianBlur(src, g_src, Size(3, 3), 0, 0);

	//�Ҷ�ͼ��
	cvtColor(g_src, src_gray, COLOR_RGB2GRAY);

	//��ʼ��Ե���(Laplacian)
	Laplacian(src_gray, dst, CV_8U, 9, 1, 0, BORDER_DEFAULT);

	namedWindow("Ч��ͼ", WINDOW_NORMAL);
	imshow("Ч��ͼ", dst);

	waitKey(0);
}

void ScharrDT(Mat& src)
{
	namedWindow("ԭͼ", WINDOW_NORMAL);
	imshow("ԭͼ", src);

	//x����
	Mat grad_x;
	Scharr(src, grad_x, CV_8U, 1, 0, 1, 0, BORDER_DEFAULT);
	namedWindow("x����", WINDOW_NORMAL);
	imshow("x����", grad_x);

	//y����
	Mat grad_y;
	Scharr(src, grad_y, CV_8U, 0, 1, 1, 0, BORDER_DEFAULT);
	namedWindow("y����", WINDOW_NORMAL);
	imshow("y����", grad_y);

	//x+y����
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

	namedWindow("ԭͼ", WINDOW_NORMAL);
	imshow("ԭͼ", src);

	//����һ3���µİ汾
	Canny(src, out, 150, 100);
	namedWindow("����һ��Ч��ͼ", WINDOW_NORMAL);
	imshow("����һ��Ч��ͼ", out);

	//������3���ϵİ汾
	Mat dst, edge, gray;

	//����һ����src1һ���ľ���
	dst.create(src1.size(), src1.type());

	//��ԭͼ��תΪ�Ҷ�
	cvtColor(src1, gray, COLOR_RGB2GRAY);

	//�˲�(����)
	blur(gray, edge, Size(3, 3));

	//canny
	Mat out2;
	Canny(edge, out2, 15, 10);


	dst = Scalar::all(0);

	src1.copyTo(dst, out2);

	namedWindow("��������Ч��ͼ", WINDOW_NORMAL);
	imshow("��������Ч��ͼ", out2);

	waitKey(0);
}