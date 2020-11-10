Mat temp = imread("./test/temp.bmp", IMREAD_COLOR);
Mat ftmp[6];
double minVal; double maxVal; Point minLoc; Point maxLoc;
Point matchLoc;
for (int i = 3; i < 4; i++) {
	matchTemplate(src2, temp, ftmp[i], i);//基本上3种归一化的方法效果最好
	normalize(ftmp[i], ftmp[i], 1, 0, NORM_MINMAX);
	minMaxLoc(ftmp[i], &minVal, &maxVal, &minLoc, &maxLoc); //找到最佳匹配点，可能是最大值位置，也可能是最小值位置
	rectangle(src2, Rect(maxLoc.x, maxLoc.y, temp.cols, temp.rows), Scalar(0, 0, 255), 2, 0);
}
namedWindow("src2", CV_WINDOW_NORMAL);
imshow("src2", src2);