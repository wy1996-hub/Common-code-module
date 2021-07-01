#include <opencv2/opencv.hpp>
#include <iostream>
#include <unordered_map>

using namespace cv;
using namespace std;

//自己实现的凸包算法类
class Graham
{
public:
	Graham() {}
	Graham(vector<Point> contours) : points(contours) {}
	~Graham() { points.clear(); resPoints.clear(); }

	static bool findStart(Point a, Point b)//排序找第一个点
	{
		if (a.y == b.y)
			return a.x<b.x;
		else
			return a.y>b.y;
	}

	static int cross(Point a, Point b, Point c)//此处是针对point2i类型的数据（像素级），若要使用Point2f类型的数据（亚像素级），则返回值float
	{
		return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
	}

	static double dist(Point a, Point b)
	{
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	bool polarAngleCmp(Point a, Point b)//极角排序,更容易理解
	{
		int m = cross(points[0], a, b);
		if (m>0) return 1;
		else if (m == 0 && dist(points[0], a) - dist(points[0], b) <= 0)
			return 1;
		else return 0;
	}

	bool polarAngleCmp2(Point a, Point b)//极角排序另一种方法，速度快
	{
		if (atan2(a.y - grahamPoints[0].y, a.x - grahamPoints[0].x) != atan2(b.y - grahamPoints[0].y, b.x - grahamPoints[0].x))
			return (atan2(a.y - grahamPoints[0].y, a.x - grahamPoints[0].x))<(atan2(b.y - grahamPoints[0].y, b.x - grahamPoints[0].x));
		return a.x < b.x;
	}
	
	vector<Point> sloveGraham()
	{
		grahamPoints = new Point[points.size()];

		//找最左下角点
		sort(&points[0], &points[points.size() - 1], findStart);
		grahamPoints[0] = points[0];

		//极角从小到大排序
		//sort(&points[1], &points[points.size() - 1], polarAngleCmp);
		//但类的成员函数默认带有一个this指针参数,泛函中的Func类型并没有this指针
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

		//利用栈结构不断维护凸壳
		int top = 1;//最后凸包中拥有点的个数
		for (size_t i = 2; i < points.size(); i++)
		{
			while (cross(grahamPoints[top - 1], grahamPoints[top], points[i]) > 0)
				top--;
			grahamPoints[++top] = points[i]; 
		}
		for (size_t i = 0; i < top; i++)//输出凸包上的点
			resPoints.emplace_back(grahamPoints[i]);
		delete[] grahamPoints;
		return resPoints;
	}

private:
	vector<Point> points;//存入的所有点
	Point* grahamPoints;//凸包中所有点
	vector<Point> resPoints;//输出结果
};

//寻找车辆凸包与车辆轮廓不相交的点集
void findDisjointPts();

//计算目标轮廓的矩
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

	// 自己实现的凸包算法
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

	// opencv的凸包算法
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

	imwrite("./result/轮廓.jpg", drawing1);
	imwrite("./result/自己实现的凸包算法.jpg", drawing2);
	imwrite("./result/opencv的凸包算法.jpg", drawing3);
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

	Mat src2 = imread("./result/opencv的凸包算法.jpg", IMREAD_COLOR);
	Mat graySrc2;
	cvtColor(src2, graySrc2, COLOR_BGR2GRAY);
	Mat thresh2;
	threshold(graySrc2, thresh2, 100, 255, THRESH_BINARY);

	//寻找目标轮廓
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

	//后处理
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

	//寻找目标轮廓
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

	//显示孔洞
	Mat drawing = Mat::zeros(thresh.size(), CV_8U);
	drawContours(drawing, contours, firstPos, Scalar(255), FILLED);
	drawContours(drawing, contours, secondPos, Scalar(255), FILLED);
	imwrite("./result/面积最大的两个子轮廓.jpg", drawing);

	vector<vector<Point>> goalContours;
	goalContours.emplace_back(contours[firstPos]);
	goalContours.emplace_back(contours[secondPos]);

	//计算轮廓的矩
	vector<Moments> g_vMoments(goalContours.size());
	//利用计算得到的矩计算中心矩
	vector<Point2f> centerMoments(goalContours.size());
	for (int i = 0; i < goalContours.size(); i++)
	{
		g_vMoments[i] = moments(goalContours[i], true);
		centerMoments[i] = Point2f(static_cast<float>(g_vMoments[i].m10 / g_vMoments[i].m00), static_cast<float>(g_vMoments[i].m01 / g_vMoments[i].m00));
	}
	//将得到的中心矩显示出来
	for (int i = 0; i < goalContours.size(); i++)
	{
		circle(drawing, (Point)centerMoments[i], 5, Scalar(0), -1, 8);
	}
	imwrite("./result/轮廓中心点.jpg", drawing);
	Mat srcImage = imread("0.jpg", IMREAD_UNCHANGED);
	line(srcImage, (Point)centerMoments[0], (Point)centerMoments[1], Scalar(0, 0, 0), 8, 8);
	imwrite("./result/最终结果图.jpg", srcImage);

	//由中心矩计算Hu矩
	for (int i = 0; i < goalContours.size(); i++)
	{
		double Hu[7];
		HuMoments(g_vMoments[i], Hu);
		for (int j = 0; j<7; j++)
		{
			cout <<"第"<<i << "个中心矩的第" <<j<<"个Hu矩为："<<log(abs(Hu[j])) << endl; // 取对数 （自然指数e为底）
		}
	}
}