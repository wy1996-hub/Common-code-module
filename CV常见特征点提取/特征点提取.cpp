extern "C" {
#include <vl/generic.h>
#include <vl/sift.h>
}

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void vlSift(Mat& src);
void opencvSift(Mat& src);
void opencvFast(Mat& src);
void opencvBrisk(Mat& src);
void opencvAkaze(Mat& src);
void opencvOrb(Mat& src);

int main(int argc, char* argv[])
{
	Mat src = imread("0.jpg", 0);
	vlSift(src);
	waitKey(0);
	opencvSift(src);
	waitKey(0);
	opencvFast(src);
	waitKey(0);
	opencvBrisk(src);
	waitKey(0);
	opencvAkaze(src);
	waitKey(0);
	opencvOrb(src);
	waitKey(0);
	cin.get();
	return 0;
}

void vlSift(Mat& src)
{
	Mat& srcImage = src.clone();
	int noctaves = 4, nlevels = 2, o_min = 0;
	//在sift.h头文件中有 typedef float vl_sift_pix 声明
	vl_sift_pix *ImageData = new vl_sift_pix[srcImage.cols*srcImage.rows];
	for (int i = 0; i < srcImage.rows; i++)
	{
		uchar *Pixel = srcImage.ptr<uchar>(i);
		for (int j = 0; j < srcImage.cols; j++)
		{
			ImageData[i*srcImage.cols + j] = Pixel[j];
		}
	}
	// 定义VlSiftFilt结构体指针
	VlSiftFilt *SiftFilt = NULL;
	// 创建一个新的sift滤波器
	SiftFilt = vl_sift_new(srcImage.cols, srcImage.rows, noctaves, nlevels, o_min);
	int KeyPoint = 0;
	int idx = 0;
	if (vl_sift_process_first_octave(SiftFilt, ImageData) != VL_ERR_EOF)
	{
		while (true)
		{
			//计算每组中的关键点  
			vl_sift_detect(SiftFilt);
			//遍历并绘制每个点  
			KeyPoint += SiftFilt->nkeys;//检测到的关键点的数目
			VlSiftKeypoint *pKeyPoint = SiftFilt->keys;//检测到的关键点
			for (int i = 0; i<SiftFilt->nkeys; i++)
			{
				VlSiftKeypoint TemptKeyPoint = *pKeyPoint;
				pKeyPoint++;
				circle(srcImage, Point(static_cast<int>(TemptKeyPoint.x), static_cast<int>(TemptKeyPoint.y)), static_cast<int>(TemptKeyPoint.sigma / 2), /*CV_RGB(255, 0, 0)*/Scalar::all(-1));
			}
			//下一阶  
			if (vl_sift_process_next_octave(SiftFilt) == VL_ERR_EOF) break;
		}
	}
	vl_sift_delete(SiftFilt);
	delete[] ImageData;
	ImageData = NULL;
	namedWindow("vlfeat sift", WINDOW_AUTOSIZE);
	imshow("vlfeat sift", srcImage);
}

//常规视觉特征检测的巅峰
void opencvSift(Mat& src)
{
	Mat& srcImage = src.clone();
	auto sift = SIFT::create();
	vector<KeyPoint> keypoints;
	double t = (double)cv::getTickCount();
	Mat descriptors;
	sift->detectAndCompute(srcImage, Mat(), keypoints, descriptors);
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "opencv sift cost time: " << t << endl;
	drawKeypoints(srcImage, keypoints, srcImage, Scalar::all(-1), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("opencv sift", WINDOW_AUTOSIZE);
	imshow("opencv sift", srcImage);
}

//FAST（Features from Accelerated Segment Test-加速段测试特征）特征点提取算法，是一种简单快速的特征提取算法
//因为其具有比较快的运算速度和还算不错的特征提取效果而被广泛应用于实时检测系统中。
//但是对于某些由于光照不均匀或存在阴影等因素而导致局部对比度不同的图像，FAST算法提取特征的适应性和特征提取效果都很不理想。
void opencvFast(Mat& src)
{
	Mat& srcImage = src.clone();
	auto fast = FastFeatureDetector::create();
	vector<KeyPoint> keypoints;
	double t = (double)cv::getTickCount();
	fast->detect(srcImage, keypoints, Mat());
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "opencv fast cost time: " << t << endl;
	drawKeypoints(srcImage, keypoints, srcImage, Scalar::all(-1), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("opencv fast", WINDOW_AUTOSIZE);
	imshow("opencv fast", srcImage);
}

//BRISK算法是SIFT算法的一种改进型，主要是针对于旋转不变性、鲁棒性、运算速度等方面做了优化,BRISK是一种高速的特征点检测和特征描述子生成
//基本可实时处理
void opencvBrisk(Mat& src)
{
	Mat& srcImage = src.clone();
	auto brisk = BRISK::create();
	vector<KeyPoint> keypoints;
	Mat descriptors; 
	double t = (double)cv::getTickCount();
	brisk->detectAndCompute(srcImage, Mat(), keypoints, descriptors, false);
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "opencv brisk cost time: " << t << endl;
	drawKeypoints(srcImage, keypoints, srcImage, Scalar::all(-1), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("opencv brisk", WINDOW_AUTOSIZE);
	imshow("opencv brisk", srcImage);
}

//AKAZE特征算法是SIFT特征算法的一种改进版本，有了提速，但还不足以实时处理
//但不使用高斯模糊来构建尺度空间，因为高斯模糊具有丢失边缘信息的缺点，进而采用非线性扩散滤波来构建尺度空间，从而保留图像更多的边缘特征
void opencvAkaze(Mat& src)
{
	Mat& srcImage = src.clone();
	auto akaze = AKAZE::create();
	vector<KeyPoint> keypoints;
	Mat descriptors;
	double t = (double)cv::getTickCount();
	akaze->detectAndCompute(srcImage, Mat(), keypoints, descriptors, false);
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "opencv akaze cost time: " << t << endl;
	drawKeypoints(srcImage, keypoints, srcImage, Scalar::all(-1), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("opencv akaze", WINDOW_AUTOSIZE);
	imshow("opencv akaze", srcImage);
}

//ORB特征是将FAST特征点的检测方法与BRIEF特征描述子结合起来，并在它们原来的基础上做了改进与优化。
//尤其是对于BRIEF算法，在ORB算法中针对其不具有旋转不变性这个缺点，做了针对性优化。
//ORB特征算法是SIFT算法处于专利保护期时的一个高效替代算法
void opencvOrb(Mat& src)
{
	Mat& srcImage = src.clone();
	auto orb = ORB::create();
	vector<KeyPoint> keypoints;
	Mat descriptors;
	double t = (double)cv::getTickCount();
	orb->detectAndCompute(srcImage, Mat(), keypoints, descriptors, false);
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	cout << "opencv orb cost time: " << t << endl;
	drawKeypoints(srcImage, keypoints, srcImage, Scalar::all(-1), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	namedWindow("opencv orb", WINDOW_AUTOSIZE);
	imshow("opencv orb", srcImage);
}