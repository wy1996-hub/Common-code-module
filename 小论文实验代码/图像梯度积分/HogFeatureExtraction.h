#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class HogFeatureExtraction
{
public:
	//构造函数
	HogFeatureExtraction();
	//构造函数
	HogFeatureExtraction(int _bins, int _theta, int _cell_size, int _block_size);
	virtual ~HogFeatureExtraction();

	//计算积分图
	vector<Mat> calculateIntegralHOG(Mat& srcMat);
	//单个cell的HOG
	void cacHOGinCell(Mat& m_HOGCell, Rect roi, vector<Mat>& m_integrals);
	//单个block的HOG
	Mat  getBlockHog(cv::Point pt, std::vector<cv::Mat>& integrals);
	//整个图像的HOG
	Mat cacHOGFeature(cv::Mat srcImage);
	//求对角线梯度图
	void Sobel_tl(Mat& src, Mat& dst);
	void Sobel_tr(Mat& src, Mat& dst);

private:
	int bins;
	int theta;
	int cell_size;
	int block_size;
	float R;
};




