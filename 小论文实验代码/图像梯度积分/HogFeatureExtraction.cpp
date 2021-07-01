#include "HogFeatureExtraction.h"

HogFeatureExtraction::HogFeatureExtraction()
{
	bins = 9;
	theta = 180 / bins;
	cell_size = 20;
	block_size = 2;
	R = cell_size*(block_size)*0.5;
}

HogFeatureExtraction::HogFeatureExtraction(int _bins, int _theta, int _cell_size, int _block_size)
{
	bins = _bins;
	theta = 180 / bins;
	block_size = _block_size;
	cell_size = _cell_size;
	R = _cell_size*(_block_size)*0.5;
}

HogFeatureExtraction::~HogFeatureExtraction()
{
}

void HogFeatureExtraction::Sobel_tl(Mat& src, Mat& dst)
{
	Mat kernel = (Mat_<int>(3, 3) << -2, -1, 0, -1, 0, 1, 0, 1, 2);
	filter2D(src, dst, -1, kernel);
}

void HogFeatureExtraction::Sobel_tr(Mat& src, Mat& dst)
{
	Mat kernel = (Mat_<int>(3, 3) << 0, -1, -2, 1, 0, -1, 2, 1, 0);
	filter2D(src, dst, -1, kernel);
}

// 计算积分图
vector<Mat> HogFeatureExtraction::calculateIntegralHOG(Mat& m_src)
{
	Mat m_sobel_x, m_sobel_y, m_magn, m_angle;

	//x 方向上的差分阶数
	Sobel(m_src, m_sobel_x, CV_32F, 1, 0);
	//y 方向上的差分阶数
	Sobel(m_src, m_sobel_y, CV_32F, 0, 1);

	//根据每一个点X方向和Y方向上的梯度，实现笛卡尔坐标和极坐标的转换
	//得到梯度幅值和梯度方向
	cartToPolar(m_sobel_x, m_sobel_y, m_magn, m_angle, true);
	//将笛卡尔坐标转换为极坐标之后，角度的范围在[0,360],但要转成[0,180]
	//如果m_angle<0，则加180
	add(m_angle, Scalar(180), m_angle, m_angle < 0);
	//如果m_angle>=180，则减180
	add(m_angle, Scalar(-180), m_angle, m_angle >= 180);

	//Mat m_sobel_tl, m_sobel_tr, m_magn_t, m_angle_t;
	////对角线方向
	//Sobel_tl(m_src, m_sobel_tl);
	//Sobel_tr(m_src, m_sobel_tr);
	//m_sobel_tl.convertTo(m_sobel_tl, CV_32F);
	//m_sobel_tr.convertTo(m_sobel_tr, CV_32F);
	////得到梯度幅值和梯度方向
	//cartToPolar(m_sobel_tl, m_sobel_tr, m_magn_t, m_angle_t, true);
	//add(m_angle_t, Scalar(180), m_angle_t, m_angle_t < 0);
	//add(m_angle_t, Scalar(-180), m_angle_t, m_angle_t >= 180);

	////数值水平倾斜方向融合
	//addWeighted(m_angle, 0.5, m_angle_t, 0.5, 0, m_angle);

	//将角度矩阵转换为一个灰度值范围在0~9之间的图像
	m_angle = m_angle / theta;
	//新建9个矩阵
	vector<Mat> m_bins(bins);
	for (int i = 0; i < bins; i++)
	{
		//初始化为全0矩阵
		m_bins[i] = Mat::zeros(m_src.size(), CV_32F);
	}
	//把图像的梯度幅值矩阵按九个不同方向的梯度角度，将每个角度范围内相应点的梯度幅值存储在相应的矩阵图像之上
	for (int y = 0; y < m_src.rows; y++)
	{
		for (int x = 0; x < m_src.cols; x++)
		{
			//int ind = round(m_angle.at<float>(y, x));
			int ind = m_angle.at<float>(y, x);
			m_bins[ind].at<float>(y, x) += m_magn.at<float>(y, x);//每个bin加上幅值,而非加1
			//m_bins[ind].at<float>(y, x) += 1;
		}
	}
	/*根据上面生成的9张不同角度的梯度幅值矩阵生成9张不同的梯度幅值的积分图像，至此以后，
	积分图像的每一点就代表，这一点左上角，所有梯度幅值之和；生成的9幅积分图也就是9个
	bins，不同bins上的HOG强度*/
	vector<Mat> integrals(bins);
	for (int i = 0; i < bins; i++)
	{
		//得到积分图
		integral(m_bins[i], integrals[i]);
		//string path = "./测试图/2/gd" + to_string(i + 1) + ".tif";
		//imwrite(path, m_bins[i]);
	}
	return integrals;
}

//计算单个cell的HOG特征
void HogFeatureExtraction::cacHOGinCell(Mat& m_HOGCell, Rect roi, vector<Mat>& vm_integrals)
{
	//通过9幅积分图像快速实现HOG的计算，HOG这个直方图有9个bins，每个bins就对应一张积分图像
	//确定单个矩形cell的左上角点坐标
	int x0 = roi.x;
	int y0 = roi.y;
	//确定单个矩形cell的右下角点坐标
	int x1 = x0 + roi.width;
	int y1 = y0 + roi.height;

	for (int i = 0; i <bins; i++)
	{
		//初始化容器
		Mat integral = vm_integrals[i];
		float a = integral.at<double>(y0, x0);
		float b = integral.at<double>(y1, x1);
		float c = integral.at<double>(y0, x1);
		float d = integral.at<double>(y1, x0);
		/*每循环一次，计算一个梯度方向上的HOG特征，*/
		/*每循环一次，就计算梯度方向直方图上的一个bins*/
		m_HOGCell.at<float>(0, i) = b - c - d + a;
	}
}

//计算单个Block的HOG梯度方向直方图
Mat HogFeatureExtraction::getBlockHog(Point pt, vector<Mat>& m_integrals)
{
	if (pt.x - R<0 || pt.y - R<0 || pt.x + R >= m_integrals[0].cols || pt.y + R >= m_integrals[0].rows)
	{
		return Mat();
	}
	//初始化一个矩阵
	Mat hist(Size(bins*block_size*block_size, 1), CV_32F);
	Point t1(0, pt.y - R);
	int c = 0;
	//遍历单个block,就是遍历4个cell，并且将4个cell的HOG特征向量组成了一个维数比较大的block的HOG特征向量
	for (int i = 0; i<block_size; i++)
	{
		t1.x = pt.x - R;
		for (int j = 0; j<block_size; j++)
		{
			//获取当前窗口，进行局部HOG直方图计算
			Rect roi(t1, t1 + Point(cell_size, cell_size));
			Mat  hist_temp = hist.colRange(c, c + bins);//为指定的列范围创建矩阵标头
			//根据roi确定的矩形区域，计算单个cell的HOG直方图(其本质就是一个行特征向量)
			cacHOGinCell(hist_temp, roi, m_integrals);
			t1.x += cell_size;
			c += bins;
		}
		t1.y = cell_size;
	}
	//归一化
	normalize(hist, hist, 1, 0, NORM_L2);
	return hist;
}

//计算整幅图像的HOG梯度方向直方图
Mat HogFeatureExtraction::cacHOGFeature(Mat src_image)
{
	Mat gray_image, dst_image;
	vector<Mat> wm_HOG;
	//灰度图
	cvtColor(src_image, gray_image, COLOR_RGB2GRAY);
	//生成9个不同梯度方向上的梯度幅值的积分图像
	vector<Mat> integrals = calculateIntegralHOG(gray_image);
	dst_image = gray_image.clone();

	Mat m_HOGBlock(Size(bins, 1), CV_32F);
	//遍历全图像，计算最终的梯度方向直方图HOG
	for (int y = cell_size / 2; y < gray_image.rows; y += cell_size)
	{
		for (int x = cell_size / 2; x < gray_image.cols; x += cell_size)
		{
			//获取当前block的HOG特征，每个block由四个cell组成，每个cell由8*8个像素组成
			Mat hist = getBlockHog(Point(x, y), integrals);
			if (hist.empty())
			{
				continue;
			}
			//赋值为全0的矩阵
			m_HOGBlock = Scalar(0);
			//计算的就是单个Block的梯度方向直方图HOG
			for (int i = 0; i < bins; i++)
			{
				for (int j = 0; j < block_size; j++)
				{
					m_HOGBlock.at<float>(0, i) += hist.at<float>(0, i + j*bins);
				}
			}
			//对其得到的每个Block的的矩阵进行归一化，使其转变为一个block的HOG特征向量
			normalize(m_HOGBlock, m_HOGBlock, 1, 0, NORM_L2);

			//每得到一个Block的HOG特征向量就存入wm_HOG，等到整个图像的HOG特征向量
			wm_HOG.push_back(m_HOGBlock);
			Point center(x, y);
			//绘制HOG特征图
			for (int i = 0; i < bins; i++)
			{
				double d_theta = (i * theta) * CV_PI / 180.0;
				Point rd(cell_size*0.5*cos(d_theta), cell_size*0.5*sin(d_theta));
				Point rp = center - rd;
				Point lp = center + rd;
				line(dst_image, rp, lp, Scalar(255 * m_HOGBlock.at<float>(0, i), 255, 255));
			}
		}
	}
	return dst_image;
}

