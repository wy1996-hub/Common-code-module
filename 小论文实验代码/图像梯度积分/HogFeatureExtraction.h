#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class HogFeatureExtraction
{
public:
	//���캯��
	HogFeatureExtraction();
	//���캯��
	HogFeatureExtraction(int _bins, int _theta, int _cell_size, int _block_size);
	virtual ~HogFeatureExtraction();

	//�������ͼ
	vector<Mat> calculateIntegralHOG(Mat& srcMat);
	//����cell��HOG
	void cacHOGinCell(Mat& m_HOGCell, Rect roi, vector<Mat>& m_integrals);
	//����block��HOG
	Mat  getBlockHog(cv::Point pt, std::vector<cv::Mat>& integrals);
	//����ͼ���HOG
	Mat cacHOGFeature(cv::Mat srcImage);
	//��Խ����ݶ�ͼ
	void Sobel_tl(Mat& src, Mat& dst);
	void Sobel_tr(Mat& src, Mat& dst);

private:
	int bins;
	int theta;
	int cell_size;
	int block_size;
	float R;
};




