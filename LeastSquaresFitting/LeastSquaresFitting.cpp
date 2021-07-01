void LeastSquaresFitting()
{
	//https://blog.csdn.net/Jacky_Ponder/article/details/70314919?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase
    if (m_nNum<3)
    {
        return;
    }
 
    int i=0;
 
    double X1=0;
    double Y1=0;
    double X2=0;
    double Y2=0;
    double X3=0;
    double Y3=0;
    double X1Y1=0;
    double X1Y2=0;
    double X2Y1=0;
 
    for (i=0;i<m_nNum;i++)
    {
        X1 = X1 + m_points[i].x;
        Y1 = Y1 + m_points[i].y;
        X2 = X2 + m_points[i].x*m_points[i].x;
        Y2 = Y2 + m_points[i].y*m_points[i].y;
        X3 = X3 + m_points[i].x*m_points[i].x*m_points[i].x;
        Y3 = Y3 + m_points[i].y*m_points[i].y*m_points[i].y;
        X1Y1 = X1Y1 + m_points[i].x*m_points[i].y;
        X1Y2 = X1Y2 + m_points[i].x*m_points[i].y*m_points[i].y;
        X2Y1 = X2Y1 + m_points[i].x*m_points[i].x*m_points[i].y;
    }
 
    double C,D,E,G,H,N;
    double a,b,c;
    N = m_nNum;
    C = N*X2 - X1*X1;
    D = N*X1Y1 - X1*Y1;
    E = N*X3 + N*X1Y2 - (X2+Y2)*X1;
    G = N*Y2 - Y1*Y1;
    H = N*X2Y1 + N*Y3 - (X2+Y2)*Y1;
    a = (H*D-E*G)/(C*G-D*D);
    b = (H*C-E*D)/(D*D-G*C);
    c = -(a*X1 + b*Y1 + X2 + Y2)/N;
 
    double A,B,R;
    A = a/(-2);
    B = b/(-2);
    R = sqrt(a*a+b*b-4*c)/2;
 
    m_fCenterX = A;
    m_fCenterY = B;
    m_fRadius = R;
 
    return;
}