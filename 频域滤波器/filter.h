#pragma once

#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

cv::Mat image_make_border(cv::Mat &src);

Mat frequency_filter(Mat &scr, Mat &blur);
Mat ideal_low_kernel(Mat &scr, float sigma);
cv::Mat ideal_low_pass_filter(Mat &src, float sigma);
Mat butterworth_low_kernel(Mat &scr, float sigma, int n);
Mat butterworth_low_paass_filter(Mat &src, float d0, int n);
Mat gaussian_low_pass_kernel(Mat scr, float sigma);
Mat gaussian_low_pass_filter(Mat &src, float d0);

Mat ideal_high_kernel(Mat &scr, float sigma);
cv::Mat ideal_high_pass_filter(Mat &src, float sigma);
Mat butterworth_high_kernel(Mat &scr, float sigma, int n);
Mat butterworth_high_paass_filter(Mat &src, float d0, int n);
Mat gaussian_high_pass_kernel(Mat scr, float sigma);
Mat gaussian_high_pass_filter(Mat &src, float d0);

int handleImage(/*int argc, char *argv[]*/);
