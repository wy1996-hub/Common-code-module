#include <iostream>
using namespace std;

const int m{ 5 }, n{ 2 };   //m是样本数目，n是theta参数个数
double house_data[][3]{    //二维数组，几行3列
	2104,3,399900,
	1600,3,329900,
	2400,3,369000,
	1416,2,232000,
	3000,4,539900,
	1985,4,299900,
	1534,3,314900,
	1427,3,198999,
	1380,3,212000,
	1494,3,242500,
	1940,4,239999,
	2000,3,347000,
	1890,3,329999,
	4478,5,699900,
	1268,3,259900,
	2300,4,449900,
	1320,2,299900,
	1236,3,199900,
	2609,4,499998,
	3031,4,599000,
	1767,3,252900,
	1888,2,255000,
	1604,3,242900,
	1962,4,259900,
	3890,3,573900,
	1100,3,249900,
	1458,3,464500,
	2526,3,469000,
	2200,3,475000,
	2637,3,299900,
	1839,2,349900,
	1000,1,169900,
	2040,4,314900,
	3137,3,579900,
	1811,4,285900,
	1437,3,249900,
	1239,3,229900,
	2132,4,345000,
	4215,4,549000,
	2162,4,287000,
	1664,2,368500,
	2238,3,329900,
	2567,4,314000,
	1200,3,299000,
	852,2,179900,
	1852,4,299900,
	1203,3,239500 };

//double X[][1] = { 1.0 / 0.5167, 1.0 / 1.2062, 1.0 / 1.9242, 1.0 / 2.6539, 1.0 / 3.4052 };
//double Y[]{ 1.0 / 2.0, 1.0 / 4.0, 1.0 / 6.0, 1.0 / 8.0, 1.0 / 10.0 };
double X[][1] = { 1.0 , 2.0, 3.0, 4.0, 5.0 };
double Y[]{  2.0, 4.0, 6.0, 8.0, 10.0 };

//一个样本的h(x)
inline auto h_theta_x(const double x[], const double theta[], const int n) {
	auto h{ 0. };
	for (int i = 0; i < n; i++)
		h += x[i] * theta[i];
	return h;
}

auto cost_function(double g[], const double X[][n], const double *Y, const double theta[], const int m)
{
	double f{ 0. };
	for (auto j = 0; j != n; j++) g[j] = 0.;
	for (int i = 0; i < m; i++) {
		auto h = h_theta_x(X[i], theta, n);
		auto h_y{ h - Y[i] };
		f += (h_y * h_y);     //累加误差
	    //累加梯度
		for (int j = 0; j < n; j++)
			g[j] += (h_y * X[i][j]);
	}
	f /= (2 * m); //平均误差
	for (int j = 0; j < n; j++) //平均梯度
		g[j] /= m;
	return f;
}

auto gradient_descent(double X[][n], double *Y, double theta[],
	double alpha, const int iterations, const int m, double *cost_history)
{
	for (auto iter = 0; iter != iterations; iter++) {
		double g[n]{};
		auto f = cost_function(g, X, Y, theta, m);
		cost_history[iter] = f;
		for (auto j = 0; j < n; j++) //更新theta
			theta[j] -= alpha * g[j];
	}
}

int main() {
	//准备训练数据，为数据特征增加一列1
	double train_X[m][n]{}, train_Y[m]{};
	for (int i = 0; i < m; i++) {
		train_X[i][0] = 1;
		train_X[i][1] = X[i][0];
		train_Y[i] = Y[i];
	}
	double theta[n]{}; //未知参数
	auto alpha(0.0001);
	auto iterations{ 1300 };
	double cost_history[1000];
	gradient_descent(train_X, train_Y, theta, alpha, iterations, m, cost_history);

	for (int j = 0; j != n; j++)
		cout << theta[j] << '\t';
	cout << '\n';

	for (int j = 0; j != 1000; j++)
		if (j % 99 == 1)
			cout << j << '\t' << cost_history[j] << '\n';
	cout << '\n';

	//cout << 1.0 / (theta[0] + theta[1] * (1.0 / 1.2062)) << endl;
	cin.get();
	return 0;
}