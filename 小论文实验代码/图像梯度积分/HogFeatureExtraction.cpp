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

// �������ͼ
vector<Mat> HogFeatureExtraction::calculateIntegralHOG(Mat& m_src)
{
	Mat m_sobel_x, m_sobel_y, m_magn, m_angle;

	//x �����ϵĲ�ֽ���
	Sobel(m_src, m_sobel_x, CV_32F, 1, 0);
	//y �����ϵĲ�ֽ���
	Sobel(m_src, m_sobel_y, CV_32F, 0, 1);

	//����ÿһ����X�����Y�����ϵ��ݶȣ�ʵ�ֵѿ�������ͼ������ת��
	//�õ��ݶȷ�ֵ���ݶȷ���
	cartToPolar(m_sobel_x, m_sobel_y, m_magn, m_angle, true);
	//���ѿ�������ת��Ϊ������֮�󣬽Ƕȵķ�Χ��[0,360],��Ҫת��[0,180]
	//���m_angle<0�����180
	add(m_angle, Scalar(180), m_angle, m_angle < 0);
	//���m_angle>=180�����180
	add(m_angle, Scalar(-180), m_angle, m_angle >= 180);

	//Mat m_sobel_tl, m_sobel_tr, m_magn_t, m_angle_t;
	////�Խ��߷���
	//Sobel_tl(m_src, m_sobel_tl);
	//Sobel_tr(m_src, m_sobel_tr);
	//m_sobel_tl.convertTo(m_sobel_tl, CV_32F);
	//m_sobel_tr.convertTo(m_sobel_tr, CV_32F);
	////�õ��ݶȷ�ֵ���ݶȷ���
	//cartToPolar(m_sobel_tl, m_sobel_tr, m_magn_t, m_angle_t, true);
	//add(m_angle_t, Scalar(180), m_angle_t, m_angle_t < 0);
	//add(m_angle_t, Scalar(-180), m_angle_t, m_angle_t >= 180);

	////��ֵˮƽ��б�����ں�
	//addWeighted(m_angle, 0.5, m_angle_t, 0.5, 0, m_angle);

	//���ǶȾ���ת��Ϊһ���Ҷ�ֵ��Χ��0~9֮���ͼ��
	m_angle = m_angle / theta;
	//�½�9������
	vector<Mat> m_bins(bins);
	for (int i = 0; i < bins; i++)
	{
		//��ʼ��Ϊȫ0����
		m_bins[i] = Mat::zeros(m_src.size(), CV_32F);
	}
	//��ͼ����ݶȷ�ֵ���󰴾Ÿ���ͬ������ݶȽǶȣ���ÿ���Ƕȷ�Χ����Ӧ����ݶȷ�ֵ�洢����Ӧ�ľ���ͼ��֮��
	for (int y = 0; y < m_src.rows; y++)
	{
		for (int x = 0; x < m_src.cols; x++)
		{
			//int ind = round(m_angle.at<float>(y, x));
			int ind = m_angle.at<float>(y, x);
			m_bins[ind].at<float>(y, x) += m_magn.at<float>(y, x);//ÿ��bin���Ϸ�ֵ,���Ǽ�1
			//m_bins[ind].at<float>(y, x) += 1;
		}
	}
	/*�����������ɵ�9�Ų�ͬ�Ƕȵ��ݶȷ�ֵ��������9�Ų�ͬ���ݶȷ�ֵ�Ļ���ͼ�������Ժ�
	����ͼ���ÿһ��ʹ�����һ�����Ͻǣ������ݶȷ�ֵ֮�ͣ����ɵ�9������ͼҲ����9��
	bins����ͬbins�ϵ�HOGǿ��*/
	vector<Mat> integrals(bins);
	for (int i = 0; i < bins; i++)
	{
		//�õ�����ͼ
		integral(m_bins[i], integrals[i]);
		//string path = "./����ͼ/2/gd" + to_string(i + 1) + ".tif";
		//imwrite(path, m_bins[i]);
	}
	return integrals;
}

//���㵥��cell��HOG����
void HogFeatureExtraction::cacHOGinCell(Mat& m_HOGCell, Rect roi, vector<Mat>& vm_integrals)
{
	//ͨ��9������ͼ�����ʵ��HOG�ļ��㣬HOG���ֱ��ͼ��9��bins��ÿ��bins�Ͷ�Ӧһ�Ż���ͼ��
	//ȷ����������cell�����Ͻǵ�����
	int x0 = roi.x;
	int y0 = roi.y;
	//ȷ����������cell�����½ǵ�����
	int x1 = x0 + roi.width;
	int y1 = y0 + roi.height;

	for (int i = 0; i <bins; i++)
	{
		//��ʼ������
		Mat integral = vm_integrals[i];
		float a = integral.at<double>(y0, x0);
		float b = integral.at<double>(y1, x1);
		float c = integral.at<double>(y0, x1);
		float d = integral.at<double>(y1, x0);
		/*ÿѭ��һ�Σ�����һ���ݶȷ����ϵ�HOG������*/
		/*ÿѭ��һ�Σ��ͼ����ݶȷ���ֱ��ͼ�ϵ�һ��bins*/
		m_HOGCell.at<float>(0, i) = b - c - d + a;
	}
}

//���㵥��Block��HOG�ݶȷ���ֱ��ͼ
Mat HogFeatureExtraction::getBlockHog(Point pt, vector<Mat>& m_integrals)
{
	if (pt.x - R<0 || pt.y - R<0 || pt.x + R >= m_integrals[0].cols || pt.y + R >= m_integrals[0].rows)
	{
		return Mat();
	}
	//��ʼ��һ������
	Mat hist(Size(bins*block_size*block_size, 1), CV_32F);
	Point t1(0, pt.y - R);
	int c = 0;
	//��������block,���Ǳ���4��cell�����ҽ�4��cell��HOG�������������һ��ά���Ƚϴ��block��HOG��������
	for (int i = 0; i<block_size; i++)
	{
		t1.x = pt.x - R;
		for (int j = 0; j<block_size; j++)
		{
			//��ȡ��ǰ���ڣ����оֲ�HOGֱ��ͼ����
			Rect roi(t1, t1 + Point(cell_size, cell_size));
			Mat  hist_temp = hist.colRange(c, c + bins);//Ϊָ�����з�Χ���������ͷ
			//����roiȷ���ľ������򣬼��㵥��cell��HOGֱ��ͼ(�䱾�ʾ���һ������������)
			cacHOGinCell(hist_temp, roi, m_integrals);
			t1.x += cell_size;
			c += bins;
		}
		t1.y = cell_size;
	}
	//��һ��
	normalize(hist, hist, 1, 0, NORM_L2);
	return hist;
}

//��������ͼ���HOG�ݶȷ���ֱ��ͼ
Mat HogFeatureExtraction::cacHOGFeature(Mat src_image)
{
	Mat gray_image, dst_image;
	vector<Mat> wm_HOG;
	//�Ҷ�ͼ
	cvtColor(src_image, gray_image, COLOR_RGB2GRAY);
	//����9����ͬ�ݶȷ����ϵ��ݶȷ�ֵ�Ļ���ͼ��
	vector<Mat> integrals = calculateIntegralHOG(gray_image);
	dst_image = gray_image.clone();

	Mat m_HOGBlock(Size(bins, 1), CV_32F);
	//����ȫͼ�񣬼������յ��ݶȷ���ֱ��ͼHOG
	for (int y = cell_size / 2; y < gray_image.rows; y += cell_size)
	{
		for (int x = cell_size / 2; x < gray_image.cols; x += cell_size)
		{
			//��ȡ��ǰblock��HOG������ÿ��block���ĸ�cell��ɣ�ÿ��cell��8*8���������
			Mat hist = getBlockHog(Point(x, y), integrals);
			if (hist.empty())
			{
				continue;
			}
			//��ֵΪȫ0�ľ���
			m_HOGBlock = Scalar(0);
			//����ľ��ǵ���Block���ݶȷ���ֱ��ͼHOG
			for (int i = 0; i < bins; i++)
			{
				for (int j = 0; j < block_size; j++)
				{
					m_HOGBlock.at<float>(0, i) += hist.at<float>(0, i + j*bins);
				}
			}
			//����õ���ÿ��Block�ĵľ�����й�һ����ʹ��ת��Ϊһ��block��HOG��������
			normalize(m_HOGBlock, m_HOGBlock, 1, 0, NORM_L2);

			//ÿ�õ�һ��Block��HOG���������ʹ���wm_HOG���ȵ�����ͼ���HOG��������
			wm_HOG.push_back(m_HOGBlock);
			Point center(x, y);
			//����HOG����ͼ
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

