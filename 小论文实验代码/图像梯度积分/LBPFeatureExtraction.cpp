#include "LBPFeatureExtraction.h"

LBPFeatureExtraction::LBPFeatureExtraction()
{
	this->radius = 1;
	this->neighbors = 8;
	this->w = 3;
	this->h = 3;
}

LBPFeatureExtraction::LBPFeatureExtraction(int radius, int neighbors)
{
	this->radius = radius;
	this->neighbors = neighbors;
	this->w = 3;
	this->h = 3;
}

LBPFeatureExtraction::~LBPFeatureExtraction()
{
}

Mat LBPFeatureExtraction::cacLBPFeature(cv::Mat srcImage)
{
	cvtColor(srcImage, srcImage, COLOR_BGR2GRAY);
	Mat dstImage(srcImage.size(), srcImage.type());
	for (int n = 0; n<neighbors; n++)
	{
		// 采样点的计算
		float x = static_cast<float>(-radius * sin(2.0*CV_PI*n / static_cast<float>(neighbors)));
		float y = static_cast<float>(radius * cos(2.0*CV_PI*n / static_cast<float>(neighbors)));
		// 上取整和下取整的值
		int fx = static_cast<int>(floor(x));
		int fy = static_cast<int>(floor(y));
		int cx = static_cast<int>(ceil(x));
		int cy = static_cast<int>(ceil(y));
		// 小数部分
		float ty = y - fy;
		float tx = x - fx;
		// 设置插值权重
		float w1 = (1 - tx) * (1 - ty);
		float w2 = tx  * (1 - ty);
		float w3 = (1 - tx) * ty;
		float w4 = tx * ty;
		// 循环处理图像数据
		for (int i = radius; i < srcImage.rows - radius; i++)
		{
			for (int j = radius; j < srcImage.cols - radius; j++)
			{
				// 计算插值
				float t = static_cast<float>(w1*srcImage.at<uchar>(i + fy, j + fx) + w2*srcImage.at<uchar>(i + fy, j + cx) + w3*srcImage.at<uchar>(i + cy, j + fx) + w4*srcImage.at<uchar>(i + cy, j + cx));
				// 进行编码
				dstImage.at<uchar>(i - radius, j - radius) += ((t > srcImage.at<uchar>(i, j)) || (std::abs(t - srcImage.at<uchar>(i, j)) < std::numeric_limits<float>::epsilon())) << n;
			}
		}
	}
	return dstImage;
}

void LBPFeatureExtraction::cacLBPbins(vector<Mat>& src, vector<vector<vector<int>>>& bins)
{
	vector<Mat> srcimg(src.size(), Mat(src[0].size(), CV_8U));
	//将三个特征图压缩到9以下
	for (size_t n = 0; n < src.size(); n++)
	{
		Mat t = src[n];
		for (size_t i = 0; i < src[n].rows; i ++)
		{
			for (size_t j = 0; j < src[n].cols; j ++)
			{
				int val = src[n].at<uchar>(i, j);
				srcimg[n].at<uchar>(i, j) = round(val/30);
			}
		}
	}
	int width = src[0].cols;
	int height = src[0].rows;
	int index = 0;
	for (size_t i = 0; i < height; i+=30)
	{
		for (size_t j = 0; j < width; j+=40)
		{
			Point pt(j, i);
			Rect r(pt.x, pt.y, 40, 30);
			for (size_t k = 0; k < src.size(); k++)
			{
				Mat img = srcimg[k](r);
				for (size_t m = 0; m < img.rows; m++)
				{
					for (size_t n = 0; n < img.cols; n++)
					{
						int value = round(*img.ptr<uchar>(m, n));
						bins[index][k][value]++;
					}
				}
			}
			index++;
		}
	}
	return;
}