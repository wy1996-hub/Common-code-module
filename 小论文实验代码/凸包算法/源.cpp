#include <opencv2/opencv.hpp>
#include <iostream>
#include <unordered_map>

using namespace cv;
using namespace std;

//�Լ�ʵ�ֵ�͹���㷨��
class Graham
{
public:
	Graham() {}
	Graham(vector<Point> contours) : points(contours) {}
	~Graham() { points.clear(); resPoints.clear(); }

	static bool findStart(Point a, Point b)//�����ҵ�һ����
	{
		if (a.y == b.y)
			return a.x<b.x;
		else
			return a.y>b.y;
	}

	static int cross(Point a, Point b, Point c)//�˴������point2i���͵����ݣ����ؼ�������Ҫʹ��Point2f���͵����ݣ������ؼ������򷵻�ֵfloat
	{
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	}

	static double dist(Point a, Point b)
	{
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	bool polarAngleCmp(Point a, Point b)//��������,���������
	{
		int m = cross(points[0], a, b);
		if (m>0) return 1;
		else if (m == 0 && dist(points[0], a) - dist(points[0], b) <= 0)
			return 1;
		else return 0;
	}

	bool polarAngleCmp2(Point a, Point b)//����������һ�ַ������ٶȿ�
	{
		if (atan2(a.y - grahamPoints[0].y, a.x - grahamPoints[0].x) != atan2(b.y - grahamPoints[0].y, b.x - grahamPoints[0].x))
			return (atan2(a.y - grahamPoints[0].y, a.x - grahamPoints[0].x))<(atan2(b.y - grahamPoints[0].y, b.x - grahamPoints[0].x));
		return a.x < b.x;
	}
	
	vector<Point> sloveGraham()
	{
		grahamPoints = new Point[points.size()];

		//�������½ǵ�
		sort(&points[0], &points[points.size() - 1], findStart);
		grahamPoints[0] = points[0];

		//���Ǵ�С��������
		//sort(&points[1], &points[points.size() - 1], polarAngleCmp);
		//����ĳ�Ա����Ĭ�ϴ���һ��thisָ�����,�����е�Func���Ͳ�û��thisָ��
		for (int i = 0; i < points.size(); i++)
		{
			int t = i;
			for (int j = i + 1; j < points.size(); j++)
			{
				int flag = cross(points[0], points[t], points[j]);
				if (flag > 0 || (flag == 0 && dist(points[0], points[t]) < dist(points[0], points[j])))
					t = j;
			}
			swap(points[i], points[t]);
		}
		grahamPoints[1] = points[1];

		//����ջ�ṹ����ά��͹��
		int top = 1;//���͹����ӵ�е�ĸ���
		for (size_t i = 2; i < points.size(); i++)
		{
			while (cross(grahamPoints[top - 1], grahamPoints[top], points[i]) > 0)
				top--;
			grahamPoints[++top] = points[i]; 
		}
		for (size_t i = 0; i < top; i++)//���͹���ϵĵ�
			resPoints.emplace_back(grahamPoints[i]);
		delete[] grahamPoints;
		return resPoints;
	}

private:
	vector<Point> points;//��������е�
	Point* grahamPoints;//͹�������е�
	vector<Point> resPoints;//������
};

//Ѱ�ҳ���͹���복���������ཻ�ĵ㼯
void findDisjointPts();

//����Ŀ�������ľ�
void calMatrix();

int main()
{
#if 1
	Mat src = imread("3.jpg", IMREAD_COLOR);
	Mat graySrc;
	cvtColor(src, graySrc, COLOR_BGR2GRAY);
	Mat output;
	threshold(graySrc, output, 0, 255, THRESH_OTSU);

	Mat drawing = Mat::zeros(src.size(), src.type());
	Mat drawing1 = src.clone();
	Mat drawing2 = src.clone();
	Mat drawing3 = src.clone();
	RNG rng(12345);

	// �Լ�ʵ�ֵ�͹���㷨
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (int i = 0; i<contours.size(); i++)
	{
		Graham* graham = new Graham(contours[i]);
		vector<Point> pts = graham->sloveGraham();
		for (size_t i = 1; i < pts.size(); i++)
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			line(drawing2, pts[i - 1], pts[i], color, 3, 8, 0);
		}
		delete graham;
	}

	// opencv��͹���㷨
	vector<vector<Point>>hull(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		convexHull(Mat(contours[i]), hull[i], false);
	}	
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing1, contours, i, color, 3, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing3, hull, i, color, 3, 8, vector<Vec4i>(), 0, Point());
	}

	imwrite("./result/����.jpg", drawing1);
	imwrite("./result/�Լ�ʵ�ֵ�͹���㷨.jpg", drawing2);
	imwrite("./result/opencv��͹���㷨.jpg", drawing3);
#endif

#if 0
	findDisjointPts();
#endif

#if 1
	calMatrix();
#endif

	waitKey(0);
	return 0;
}

void findDisjointPts()
{
	Mat src1 = imread("0.jpg", IMREAD_COLOR);
	Mat graySrc1;
	cvtColor(src1, graySrc1, COLOR_BGR2GRAY);
	Mat thresh1;
	threshold(graySrc1, thresh1, 0, 255, THRESH_OTSU);

	Mat src2 = imread("./result/opencv��͹���㷨.jpg", IMREAD_COLOR);
	Mat graySrc2;
	cvtColor(src2, graySrc2, COLOR_BGR2GRAY);
	Mat thresh2;
	threshold(graySrc2, thresh2, 100, 255, THRESH_BINARY);

	//Ѱ��Ŀ������
	vector<vector<Point>> contours;
	vector<vector<Point>> goalContours;
	vector<Vec4i> hierarchy;
	findContours(thresh2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		Rect rect = boundingRect(contours[i]);
		if ((rect.width > (thresh2.cols / 2)) && (rect.height > thresh2.rows / 2))
		{
			goalContours.emplace_back(contours[i]);
		}
	}
	Mat drawing = Mat::zeros(thresh2.size(), CV_8U);
	for (size_t i = 0; i < goalContours.size(); i++)
	{
		drawContours(drawing, goalContours, -1, Scalar(255), FILLED);
	}
	imwrite("./result/drawing.jpg", drawing);

	//����
	Mat dst;
	subtract(drawing, thresh1, dst);
	Mat kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	erode(dst, dst, kernel);
	morphologyEx(dst, dst, MORPH_RECT, kernel);
	imwrite("./result/subtract.jpg", dst);
}

void calMatrix()
{
	Mat src = imread("./result/subtract.jpg", IMREAD_GRAYSCALE);
	threshold(src, src, 0, 255, THRESH_OTSU);
	Mat thresh;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	erode(src, thresh, kernel);

	//Ѱ��Ŀ������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	unordered_map<int, int> areaInx;
	vector<int> area;
	findContours(thresh, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); i++)
	{
		areaInx.emplace(static_cast<int>(contourArea(contours[i])), i);
		area.emplace_back(static_cast<int>(contourArea(contours[i])));
	}
	sort(area.rbegin(), area.rend());
	int firstPos = areaInx[area[0]];
	int secondPos = areaInx[area[1]];

	//��ʾ�׶�
	Mat drawing = Mat::zeros(thresh.size(), CV_8U);
	drawContours(drawing, contours, firstPos, Scalar(255), FILLED);
	drawContours(drawing, contours, secondPos, Scalar(255), FILLED);
	imwrite("./result/�����������������.jpg", drawing);

	vector<vector<Point>> goalContours;
	goalContours.emplace_back(contours[firstPos]);
	goalContours.emplace_back(contours[secondPos]);

	//���������ľ�
	vector<Moments> g_vMoments(goalContours.size());
	//���ü���õ��ľؼ������ľ�
	vector<Point2f> centerMoments(goalContours.size());
	for (int i = 0; i < goalContours.size(); i++)
	{
		g_vMoments[i] = moments(goalContours[i], true);
		centerMoments[i] = Point2f(static_cast<float>(g_vMoments[i].m10 / g_vMoments[i].m00), static_cast<float>(g_vMoments[i].m01 / g_vMoments[i].m00));
	}
	//���õ������ľ���ʾ����
	for (int i = 0; i < goalContours.size(); i++)
	{
		circle(drawing, (Point)centerMoments[i], 5, Scalar(0), -1, 8);
	}
	imwrite("./result/�������ĵ�.jpg", drawing);
	Mat srcImage = imread("0.jpg", IMREAD_UNCHANGED);
	line(srcImage, (Point)centerMoments[0], (Point)centerMoments[1], Scalar(0, 0, 0), 8, 8);
	imwrite("./result/���ս��ͼ.jpg", srcImage);

	//�����ľؼ���Hu��
	for (int i = 0; i < goalContours.size(); i++)
	{
		double Hu[7];
		HuMoments(g_vMoments[i], Hu);
		for (int j = 0; j<7; j++)
		{
			cout <<"��"<<i << "�����ľصĵ�" <<j<<"��Hu��Ϊ��"<<log(abs(Hu[j])) << endl; // ȡ���� ����Ȼָ��eΪ�ף�
		}
	}
}