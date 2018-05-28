#ifndef _PLANEANALYTICGEOMETRY_H_
#define _PLANEANALYTICGEOMETRY_H_

typedef struct
{
	float X;
	float Y;
	
}Point;

typedef struct					//ֱ�߽���ʽ���磺Ax + By + C = 0
{
	double A;					//��һ������
	double B;					//�ڶ�������
	double C;					//����������
	
	double k;					//б��ֱ��б�ʣ���B != 0ʱ��Ч��
	
	double M;					//�������ʱ�õķ�ĸ��sqrt(A^2 + B^2);
	
}Line;

typedef struct
{
	Point Center;				//Բ������
	double radius;				//�뾶
	
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
