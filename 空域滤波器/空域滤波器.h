#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//声明滚动条回调函数
static void on_BoxFilter(int, void*);	//方框
static void on_MeanBulr(int, void*);	//均值
static void on_GaussianBulr(int, void*);	//高斯
static void on_MedianBlur(int, void*);	//中值
static void on_BilateralFiter(int, void*);	//双边
