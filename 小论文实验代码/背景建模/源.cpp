#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	VideoCapture capture("./video/1.avi");
	if (!capture.isOpened())
		return 0;

	Mat frame, frameGray;
	Mat foreground;

	namedWindow("Extracted Foreground", WINDOW_NORMAL);
	namedWindow("image", WINDOW_NORMAL);

	Ptr<BackgroundSubtractorMOG2> mog = createBackgroundSubtractorMOG2(500, 80, false);
	//Ptr<BackgroundSubtractorKNN> mog = createBackgroundSubtractorKNN(500, 60, false);

	bool stop = false;
	int count = 0;
	while (!stop) {
		// 读取到最后一帧空图像
		if (!capture.read(frame))
			break;

		// 降采样
		resize(frame, frame, Size(frame.cols / 4, frame.rows / 4));

		//cvtColor(frame, frame, COLOR_BGR2HSV);
		
		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		mog->apply(frameGray, foreground/*, 0.01*/);

		// 去除噪点
		Mat k1 = getStructuringElement(MORPH_RECT, Size(2, 2));
		//morphologyEx(foreground, foreground, MORPH_OPEN, k1);
		morphologyEx(foreground, foreground, MORPH_ERODE, k1);
		//Mat k2 = getStructuringElement(MORPH_RECT, Size(13, 13));
		//morphologyEx(foreground, foreground, MORPH_CLOSE, k2);
		//morphologyEx(foreground, foreground, MORPH_DILATE, k2);

		//string path1 = "./fore/" + to_string(count) + ".jpg";
		//string path2 = "./frame/" + to_string(count) + ".jpg";
		//imwrite(path1, foreground);
		//imwrite(path2, frame);
		//count++;

		imshow("Extracted Foreground", foreground);
		imshow("image", frame);

		if (waitKey(10) >= 0)
			stop = true;
	}

	return 0;
}