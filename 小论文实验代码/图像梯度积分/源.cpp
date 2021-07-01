#include <opencv2/opencv.hpp>
#include <iostream>
#include "HogFeatureExtraction.h"
#include "LBPFeatureExtraction.h"

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("0.jpg");
	if (src.empty())
	{
		cout << "could not load the image...\n" << endl;
		return -1;
	}
	//resize(src, src, Size(280, 210));
	namedWindow("src", WINDOW_NORMAL);
	imshow("src", src);

	HogFeatureExtraction hogFeature(9, 180, 8, 2);
	Mat dst = hogFeature.cacHOGFeature(src);
	namedWindow("HOG", WINDOW_NORMAL);
	imshow("HOG", dst);
	//imwrite("HOG.jpg", dst);

	//LBPFeatureExtraction lbpFeature1(1, 8);
	//Mat dst1 = lbpFeature1.cacLBPFeature(src);
	//LBPFeatureExtraction lbpFeature2(2, 16);
	//Mat dst2 = lbpFeature2.cacLBPFeature(src);
	//LBPFeatureExtraction lbpFeature3(3, 24);
	//Mat dst3 = lbpFeature3.cacLBPFeature(src);
	//namedWindow("LBP1", WINDOW_NORMAL);
	//imshow("LBP1", dst1);
	//imwrite("LBP1.jpg", dst1);
	//namedWindow("LBP2", WINDOW_NORMAL);
	//imshow("LBP2", dst1);
	//imwrite("LBP2.jpg", dst2);
	//namedWindow("LBP3", WINDOW_NORMAL);
	//imshow("LBP3", dst1);
	//imwrite("LBP3.jpg", dst3);
	//vector<Mat> lbpimg;
	//lbpimg.push_back(dst1);
	//lbpimg.push_back(dst2);
	//lbpimg.push_back(dst3);
	//vector<vector<vector<int>>> res(49, vector<vector<int>>(lbpimg.size(), vector<int>(9)));
	//LBPFeatureExtraction::cacLBPbins(lbpimg, res);
	//vector<int> rr;
	//for (size_t i = 0; i < res.size(); i++)
	//{
	//	for (size_t m = 0; m < res[i].size(); m++)
	//	{
	//		for (size_t n = 0; n < res[i][m].size(); n++)
	//		{
	//			rr.push_back(res[i][m][n]);
	//			cout << res[i][m][n] << ";";
	//		}
	//		cout << endl;
	//	}
	//}

	waitKey(0);
	return 0;
}
