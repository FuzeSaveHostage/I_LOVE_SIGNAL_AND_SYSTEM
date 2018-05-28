#ifndef _PLANEANALYTICGEOMETRY_H_
#define _PLANEANALYTICGEOMETRY_H_

typedef struct
{
	float X;
	float Y;
	
}Point;

typedef struct					//直线解析式形如：Ax + By + C = 0
{
	double A;					//第一个参数
	double B;					//第二个参数
	double C;					//第三个参数
	
	double k;					//斜率直线斜率（当B != 0时有效）
	
	double M;					//计算距离时用的分母：sqrt(A^2 + B^2);
	
}Line;

typedef struct
{
	Point Center;				//圆心坐标
	double radius;				//半径
	
}Circle;

Circle CreateCircle(Point point1, double k1, Point point2, double k2);
Circle CreateCircle(Point point1, Point point2, Point point3);

Line CreateLine(Point point, double k, bool isK_NotExist);
Line CreateLine(Point StartPoint, Point EndPiont);
Line getVerticalLine(Point point, Line line);

Point GetIntersection(Line line1, Line line2);
Point GetMidpoint(Point point1, Point point2);
Line getMidperpendicular(Point point1, Point point2);

double GetDistanceToLine(Point point, Line line);
double GetDistanceToPoint(Point point1, Point point2);

#endif
