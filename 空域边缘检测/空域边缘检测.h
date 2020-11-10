#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void SobelDT(Mat& src);
void LaplacianDT(Mat& src);
void ScharrDT(Mat& src);
void CannyDT(Mat& src);