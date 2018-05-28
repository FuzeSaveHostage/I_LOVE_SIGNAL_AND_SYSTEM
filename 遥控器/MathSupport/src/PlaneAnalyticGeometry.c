#include "PlaneAnalyticGeometry.h"
#include "SolveSystem.h"
#include <math.h>

/*  �����ۺ���һЩƽ�����������ص�����  */

/**
  * @brief  �Ե�бʽ����ֱ�߽���ʽ(kx - y + (y1 - kx1) = 0)
  * @param  point:��֪��
  * @param  k:ֱ��б��
  * @param  isK_NotExist:��־ĳֱ��б���Ƿ����
  * @retval ���ɵ�ֱ�߽���ʽ�ı�׼��ʽ
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
  * @brief  ������ʽ����ֱ�߽���ʽ
  * @param  StartPoint:��һ����
  * @param  EndPiont:�ڶ�����
  * @retval ���ɵ�ֱ�߽���ʽ�ı�׼��ʽ
  */
Line CreateLine(Point StartPoint, Point EndPiont)
{
	double temp = 0;
	bool is_K_Notexist = true;
	
	is_K_Notexist = (fabs(EndPiont.X - StartPoint.X) < 1*10e-10);
	
	if(!is_K_Notexist)		//���K���ڣ�����б��
		temp = (EndPiont.Y - StartPoint.Y) / (EndPiont.X - StartPoint.X);
	
	else if(fabs(EndPiont.Y - StartPoint.Y) < 1*10e-10)			//�������ĵ���һ����,�򷵻ؿ�ֱ�ߣ�������ֵΪ�㣩
	{
		Line Nullline = {0};
		return Nullline;
	}
	
	return CreateLine(StartPoint, temp, is_K_Notexist);
}

/**
  * @brief  ��ȡ��ֱ�ߵĽ���
  * @param  line1:ֱ��1
  * @param  line1:ֱ��2
  * @retval ��ֱ�߽�������
  */
Point GetIntersection(Line line1, Line line2)
{
	Point result;
	Vector temp;
	CoefficientMatrix co_mat;
	
	temp.b[0] = -line1.C;							//Ϊ�������㲿�ָ�ֵ
	temp.b[1] = -line2.C;
	
	co_mat.Mat.a11 = line1.A;						//Ϊϵ������ֵ
	co_mat.Mat.a12 = line1.B;
	
	co_mat.Mat.a21 = line2.A;
	co_mat.Mat.a22 = line2.B;
	
	temp = SolveSecondOrder_System(co_mat, temp);	//��������Է����飬�����ֱ�߽���
	
	result.X = temp.b[0];
	result.Y = temp.b[1];
	
	return result;
}

/**
  * @brief  ��ȡ�������ߵ��е�
  * @param  point1:��1
  * @param  point2:��2
  * @retval �е�����
  */
Point GetMidpoint(Point point1, Point point2)
{
	Point result;
	
	result.X = (point1.X + point2.X) / 2;
	result.Y = (point1.Y + point2.Y) / 2;
	
	return result;
}

/**
  * @brief  ��������ֱ֪�ߴ�ֱ��ֱ�߽���ʽ
  * @param  line:Ŀ��ֱ�ߵĽ���ʽ
  * @param  point:ֱ������֪�������
  * @retval ���ɵ�ֱ�߽���ʽ�ı�׼��ʽ
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
  * @brief  �����������ߵ��д���
  * @param  point1:��1
  * @param  point2:��2
  * @retval ���ɵ�ֱ�߽���ʽ�ı�׼��ʽ
  */
Line getMidperpendicular(Point point1, Point point2)
{
	Line tempLine = CreateLine(point1, point2);
	Point Midpoint = GetMidpoint(point1, point2);
	
	return getVerticalLine(Midpoint, tempLine);
}

/**
  * @brief  ���ĳ�㵽ĳֱ�ߵľ���
  * @param  point:ƽ����һ��
  * @param  line:ֱ�߽���ʽ
  * @retval �㵽ֱ�ߵľ���
  */
double GetDistanceToLine(Point point, Line line)
{
	if(fabs(line.M) < 1*10e-10)						//���ֱ��û�б���ʼ��
		return 0;
	
	return (fabs(line.A * point.X + line.B * point.Y + line.C) / line.M);
}

/**
  * @brief  ��������ľ���
  * @param  point1:ƽ����һ��
  * @param  point2:ƽ������һ��
  * @retval �㵽ֱ�ߵľ���
  */
double GetDistanceToPoint(Point point1, Point point2)
{
	double tempx = point1.X - point2.X;
	double tempy = point1.Y - point2.Y;
	
	return sqrt(tempx * tempx + tempy * tempy);
}

/**
  * @brief  ����һ��Բ
  * @param  point1:ƽ����һ��
  * @param  point2:ƽ������һ��
  * @param  k1:point1����ֱ�ߵ�б��
  * @param  k2:point2����ֱ�ߵ�б��
  * @retval ���ɵ�Բ
  */
Circle CreateCircle(Point point1, double k1, Point point2, double k2)
{
	double temp = 0;	
	Circle result = {0};
	
	Line line1;			
	Line line2 = CreateLine(point2, -1.0 / k2, false);
	
	/*  ������㴦��������ֱ�ߵĽ���ʽ  */
	if(fabs(k1) < 1 * 10e-10)							//���ԭֱ��б��Ϊ��
		line1 = CreateLine(point1, 0, true);
	else	
		line1 = CreateLine(point1, -1.0 / k1, false);
	
	if(fabs(k2) < 1 * 10e-10)							//���ԭֱ��б��Ϊ��
		line2 = CreateLine(point2, 0, true);
	else
		line2 = CreateLine(point2, -1.0 / k2, false);
		
	result.Center = GetIntersection(line1, line2);		//��ȡԲ��
	
	temp = GetDistanceToPoint(point1, result.Center);	//��ð뾶����ֵ��
	temp += GetDistanceToPoint(point2, result.Center);
	
	result.radius = temp / 2.0;
	
	return result;
}


/**
  * @brief  ����һ��Բ
  * @param  point1:ƽ����һ��
  * @param  point2:ƽ������һ��
  * @param  point3:ƽ������һ��
  * @retval ���ɵ�Բ
  */
Circle CreateCircle(Point point1, Point point2, Point point3)
{
	double temp = 0;	
	Circle result = {0};
	
	Line line1 = getMidperpendicular(point1, point2);	//����������ߵ��д���
	Line line2 = getMidperpendicular(point2, point3);
	
	result.Center = GetIntersection(line1, line2);		//��ȡԲ��
	
	temp = GetDistanceToPoint(point1, result.Center);	//��ð뾶����ֵ��
	temp += GetDistanceToPoint(point2, result.Center);
	
	result.radius = temp / 2.0;
	
	return result;
}



