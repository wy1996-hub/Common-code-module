#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//�����������ص�����
static void on_BoxFilter(int, void*);	//����
static void on_MeanBulr(int, void*);	//��ֵ
static void on_GaussianBulr(int, void*);	//��˹
static void on_MedianBlur(int, void*);	//��ֵ
static void on_BilateralFiter(int, void*);	//˫��
