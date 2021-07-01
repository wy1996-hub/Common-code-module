#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


class LBPFeatureExtraction
{
public:
	LBPFeatureExtraction();
	LBPFeatureExtraction(int radius, int neighbors);
	virtual ~LBPFeatureExtraction();
	Mat cacLBPFeature(cv::Mat srcImage);
	static void cacLBPbins(vector<Mat>& src, vector<vector<vector<int>>>& bins);

private:
	int radius;
	int neighbors;
	int w;
	int h;
};

