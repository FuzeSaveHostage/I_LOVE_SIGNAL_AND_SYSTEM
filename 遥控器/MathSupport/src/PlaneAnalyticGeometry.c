#include "PlaneAnalyticGeometry.h"
#include "SolveSystem.h"
#include <math.h>

/*  这里综合了一些平面解析几何相关的运算  */

/**
  * @brief  以点斜式生成直线解析式(kx - y + (y1 - kx1) = 0)
  * @param  point:已知点
  * @param  k:直线斜率
  * @param  isK_NotExist:标志某直线斜率是否存在
  * @retval 生成的直线解析式的标准形式
  */
 Line CreateLine(Point point, double k, bool isK_NotExist)
 {
	Line result = {0};
	
	result.k = k;
	
	if(isK_NotExist)
	{
		result.A = 1;
		result.B = 0;
		result.C = -point.X;
	}
	else
	{
		result.A = k;
		result.B = -1;
		result.C = (point.Y - k * point.X);
	}
	
	result.M = sqrt(result.A * result.A + result.B * result.B);
	 
	return result;
 }

/**
  * @brief  以两点式生成直线解析式
  * @param  StartPoint:第一个点
  * @param  EndPiont:第二个点
  * @retval 生成的直线解析式的标准形式
  */
Line CreateLine(Point StartPoint, Point EndPiont)
{
	double temp = 0;
	bool is_K_Notexist = true;
	
	is_K_Notexist = (fabs(EndPiont.X - StartPoint.X) < 1*10e-10);
	
	if(!is_K_Notexist)		//如果K存在，计算斜率
		temp = (EndPiont.Y - StartPoint.Y) / (EndPiont.X - StartPoint.X);
	
	else if(fabs(EndPiont.Y - StartPoint.Y) < 1*10e-10)			//若给出的点是一样的,则返回空直线（各参量值为零）
	{
		Line Nullline = {0};
		return Nullline;
	}
	
	return CreateLine(StartPoint, temp, is_K_Notexist);
}

/**
  * @brief  获取两直线的交点
  * @param  line1:直线1
  * @param  line1:直线2
  * @retval 两直线交点坐标
  */
Point GetIntersection(Line line1, Line line2)
{
	Point result;
	Vector temp;
	CoefficientMatrix co_mat;
	
	temp.b[0] = -line1.C;							//为方程增广部分赋值
	temp.b[1] = -line2.C;
	
	co_mat.Mat.a11 = line1.A;						//为系数矩阵赋值
	co_mat.Mat.a12 = line1.B;
	
	co_mat.Mat.a21 = line2.A;
	co_mat.Mat.a22 = line2.B;
	
	temp = SolveSecondOrder_System(co_mat, temp);	//解二阶线性方程组，获得两直线交点
	
	result.X = temp.b[0];
	result.Y = temp.b[1];
	
	return result;
}

/**
  * @brief  获取两点连线的中点
  * @param  point1:点1
  * @param  point2:点2
  * @retval 中点坐标
  */
Point GetMidpoint(Point point1, Point point2)
{
	Point result;
	
	result.X = (point1.X + point2.X) / 2;
	result.Y = (point1.Y + point2.Y) / 2;
	
	return result;
}

/**
  * @brief  生成与已知直线垂直的直线解析式
  * @param  line:目标直线的解析式
  * @param  point:直线上已知点的坐标
  * @retval 生成的直线解析式的标准形式
  */
Line getVerticalLine(Point point, Line line)
{
	Line result = {0};

	result.A = line.B;
	result.B = -line.A;
	result.C = (line.A * point.Y - line.B * point.X);
	
	result.M = sqrt(result.A * result.A + result.B * result.B);
	
	return result;
}

/**
  * @brief  生成两点连线的中垂线
  * @param  point1:点1
  * @param  point2:点2
  * @retval 生成的直线解析式的标准形式
  */
Line getMidperpendicular(Point point1, Point point2)
{
	Line tempLine = CreateLine(point1, point2);
	Point Midpoint = GetMidpoint(point1, point2);
	
	return getVerticalLine(Midpoint, tempLine);
}

/**
  * @brief  获得某点到某直线的距离
  * @param  point:平面上一点
  * @param  line:直线解析式
  * @retval 点到直线的距离
  */
double GetDistanceToLine(Point point, Line line)
{
	if(fabs(line.M) < 1*10e-10)						//如果直线没有被初始化
		return 0;
	
	return (fabs(line.A * point.X + line.B * point.Y + line.C) / line.M);
}

/**
  * @brief  获得两点间的距离
  * @param  point1:平面上一点
  * @param  point2:平面上另一点
  * @retval 点到直线的距离
  */
double GetDistanceToPoint(Point point1, Point point2)
{
	double tempx = point1.X - point2.X;
	double tempy = point1.Y - point2.Y;
	
	return sqrt(tempx * tempx + tempy * tempy);
}

/**
  * @brief  生成一个圆
  * @param  point1:平面上一点
  * @param  point2:平面上另一点
  * @param  k1:point1所在直线的斜率
  * @param  k2:point2所在直线的斜率
  * @retval 生成的圆
  */
Circle CreateCircle(Point point1, double k1, Point point2, double k2)
{
	double temp = 0;	
	Circle result = {0};
	
	Line line1;			
	Line line2 = CreateLine(point2, -1.0 / k2, false);
	
	/*  获得两点处法线所在直线的解析式  */
	if(fabs(k1) < 1 * 10e-10)							//如果原直线斜率为零
		line1 = CreateLine(point1, 0, true);
	else	
		line1 = CreateLine(point1, -1.0 / k1, false);
	
	if(fabs(k2) < 1 * 10e-10)							//如果原直线斜率为零
		line2 = CreateLine(point2, 0, true);
	else
		line2 = CreateLine(point2, -1.0 / k2, false);
		
	result.Center = GetIntersection(line1, line2);		//获取圆心
	
	temp = GetDistanceToPoint(point1, result.Center);	//获得半径（均值）
	temp += GetDistanceToPoint(point2, result.Center);
	
	result.radius = temp / 2.0;
	
	return result;
}


/**
  * @brief  生成一个圆
  * @param  point1:平面上一点
  * @param  point2:平面上另一点
  * @param  point3:平面上另一点
  * @retval 生成的圆
  */
Circle CreateCircle(Point point1, Point point2, Point point3)
{
	double temp = 0;	
	Circle result = {0};
	
	Line line1 = getMidperpendicular(point1, point2);	//获得两点连线的中垂线
	Line line2 = getMidperpendicular(point2, point3);
	
	result.Center = GetIntersection(line1, line2);		//获取圆心
	
	temp = GetDistanceToPoint(point1, result.Center);	//获得半径（均值）
	temp += GetDistanceToPoint(point2, result.Center);
	
	result.radius = temp / 2.0;
	
	return result;
}



