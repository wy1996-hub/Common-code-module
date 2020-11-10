vector<cv::Point>ROI;
int ROICap;//ROI数组中的点个数
int pNum = 0;
vector<cv::Point>::iterator it = points.begin();
for (; it != points.end(); it++)
{
	pNum++;
	ROICap = ROI.size();
	if(ROICap == 0)
	{
	    ROI.insert(ROI.begin(),Point(points[pNum-1].x,points[pNum-1].y));
		continue;
	}
	for(int k = 0;k<ROICap;k++)
	{
		int X0,Y0,X1,Y1,Dis;
		X0 = points[pNum-1].x;
		Y0 = points[pNum-1].y;
		X1 = ROI[k].x;
		Y1 = ROI[k].y;
		Dis = (int)sqrt(((X0-X1)*(X0-X1)+(Y0-Y1)*(Y0-Y1))*1.0);
		if(Dis < 64)
		{
			X0 = (X0+X1)/2;
			Y0 = (Y0+Y1)/2;
			ROI[k].x = X0;
			ROI[k].y = Y0;
			break;
		}
		else
		{
			if(k == ROICap - 1)
			{
				ROI.insert(ROI.begin()+ROICap,Point(X0,Y0));
			}
		}
	}
}