#include "PositionCtrl.h"
#include "Motion_Ctrl.h"
#include "Vol_curve.h"
#include <math.h>
#include "Map.h"

/*
	�Ķ�˵����
	1.���߽���һ�Σ������ź���������֮�����������㣨�Լ�������ĸ�������������һ����ͨ�Ž������ʱ����
	2.��ͨ�Ž�����Ŀ������֮��Ҫ�ѽ��ձ�־��λ����δ�����־��λ������ѡ��������ߵ���Ŀ����о��ߣ���������Ӧ����
	3.���߼������㣺����������ֻ����һ�Σ�����ʱ���������Ҫ�󣬾���ʱ�����ȷ�������˶��ļ��ٶȡ���ʱ���Լ�����ٶȣ�
	4.������ʱ��Ϊ��λ��ÿ����һ�Σ���һ���������ڵ�ʱ�䣬���µ�ָ������ߵ���Ŀ��㲢�ȶ�֮�����㲢ֹͣ����

	5.��Ԥ��ʱ����̵�ʱ��Ӧ����λ������һ��Ԥ�⵽���ʱ�䣬������ʱ�����(δ���)��
	6.ʱ�䵥λΪ���룬λ�Ƶ�λΪ���ף����ٶȵ�λ������/�� or ΢��/���룩
										���ٶȵ�λ��mm/s^2
*/

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))		//����޷�
#define T 10							//�������ڣ�10ms��

/* Public  variables ---------------------------------------------------------*/
Vol_CurveType Vol_Curve;


/**
  * @brief  ��λ�ƿ���ͨ��������ʵ�ֵ�ʱ����õ����㡪�����������ߣ�λ��-ʱ�䡢�ٶ�-λ�ơ�ʱ��-λ�ƹ�ϵ��
  * @param  time:�Ӽ�ʱ��㵽��ǰ��ʱ�䣨��λ�����룩
  * @param  distance:��ǰ���굽Ŀ���ľ���
  * @param  t:�滮���������˶�ʱ��
  * @param  a:�滮�������˶����ٶ�
  * @param  x_setted:�滮���˶���λ��
  * @param  t_stan:Ԥ�б�����ʱ��
  * @param  x_stan:Ԥ�б�����λ��
  * @retval ��ʱ��Ӧ�õ����λ�ơ�������Ӧ�ôﵽ���ٶ��Լ������ϵ���õ�������ʱ��
  */
static X_V Triangle_Loop(uint32_t time, uint32_t distance, uint32_t t, const double* a,
							uint32_t x_setted, uint32_t t_stan, uint32_t x_stan)
{
	static X_V temp = {0,0};
	
	int32_t displace = x_setted - distance;//���˶���λ��
	uint32_t half_time = t_stan/2;
	int32_t half_displace = x_setted/2;
	
	/************  x-t��ϵ  ************/
	
	/*  �����ڼ��ٶ�  */
	if(time <= half_time)					
	{
		temp.x = (int32_t)((*a) * (time * time) / 2.0 /1000000);
		
	}
	/*  �����ڼ��ٶ�  */
	else if(time <= t_stan)					
	{
		uint32_t tem_t1 = t_stan - time;						
		
		temp.x = x_stan - (int32_t)((*a) * (tem_t1 * tem_t1) / 2.0 / 1000000);
	}
	
	/************  v-x��ϵ  ************/
	
	/*  ��λ��Ϊ��  */
	if(displace < 0)
	{
		temp.v = 30;						//����һ��С�ٶȣ����н���
	}
	/*  �����ڼ��ٶ�  */
	else if(displace <= half_displace)					
	{	
		temp.v = (int32_t)sqrt(2 * (*a) * displace);
	}
	/*  �����ڼ��ٶ�  */
	else if(displace <= x_setted)					
	{	
		temp.v = (int32_t)sqrt(2 * (*a) * distance);
	}
	
	return temp;
}

/**
  * @brief  �������β���������λ�������ʱ����õ����㡪���������ߣ�λ��-ʱ�䡢�ٶ�-λ�ơ�ʱ��-λ�ƹ�ϵ��
  * @param  time:�Ӽ�ʱ��㵽��ǰ��ʱ�䣨��λ�����룩
  * @param  distance:��ǰ���굽Ŀ���ľ���
  * @param  t:�滮���������˶�ʱ��
  * @param  a:�滮�������˶����ٶ�
  * @param  x_setted:�滮���˶���λ��
  * @param  t_stan:Ԥ�б�����ʱ��
  * @param  x_stan:Ԥ�б�����λ��
  * @retval ��ʱ��Ӧ�õ����λ�ơ�������Ӧ�ôﵽ���ٶ��Լ������ϵ���õ�������ʱ��
  */
static X_V Trapezoid_Loop(uint32_t time, uint32_t distance, uint32_t t, const double* a,
							uint32_t x_setted, uint32_t t_stan, uint32_t x_stan)
{
	static X_V temp;
	
	int32_t displace = x_setted - distance;//���˶���λ��
	uint32_t tem_t = t - t_stan;			//�����˶���ʱ��
	uint32_t half_stan = x_stan/2;
	
	/************  x-t��ϵ  ************/
	
	/*  �����ڼ���ʱ���  */
	if(time <= (t_stan/2))				
	{
		temp.x = (int32_t)((*a) * (time * time) / 2.0 /1000000);
	}
	/*  ����������ʱ���  */
	else if(time <= (t_stan/2 + tem_t))
	{
		temp.x = x_stan/2 + Vol_Curve.vol_max * (time - t_stan/2) / 1000;
	}
	/*  �����ڼ���ʱ���  */
	else if(time <= t)				
	{
		uint32_t tem_t1 = t - time;			//��ȡʣ���˶�ʱ��
		
		temp.x = x_setted - (int32_t)((*a) * (tem_t1 * tem_t1) / 2.0 / 1000000);
	}
	
	/************  v-x��ϵ  ************/
	
	/*  ��λ��Ϊ��  */
	if(displace < 0)
	{
		temp.v = 0;//30;						//����һ��С�ٶȣ����н���
	}
	/*  �����ڼ��ٶ�  */
	else if(displace <= half_stan)				
	{
		temp.v = (int32_t)sqrt(2 * (*a) * displace);
	}
	/*  ����������ʱ���  */
	else if(displace <= (x_setted - half_stan))
	{
		temp.v = Vol_Curve.vol_max ;
	}
	/*  �����ڼ���ʱ���  */
	else if(displace <= x_setted)				
	{	
		temp.v = (int32_t)sqrt(2 * (*a) * distance);
	}
	
	return temp;
		
}

/**
  * @brief  �˶��滮��	�����Ժ������ߵ���Ŀ���
  * @param  aim_x��		Ŀ������X����λ����Ϊ1mm��
  * @param  aim_y��		Ŀ������Y����λ����Ϊ1mm��
  * @param  t_target��	�����ӵ�ǰλ�õ���Ŀ��������ѵ�ʱ�䣨��λΪ���룩
  * @param  Pt_stan��	ָ��洢Ԥ�б���ʱ��ı�����ָ�루��λΪ���룩
  * @param  Px_stan��	ָ��洢Ԥ�б���λ�Ƶı�����ָ�루��λ����Ϊ1mm��
  * @retval None
  */
static void Distance_curve(double aim_x, double aim_y, uint32_t t_target, Vol_CurveType* Curve)
{
	int32_t deltaX = 0,deltaY = 0;
	
	deltaX = (int32_t)(aim_x - Coordinate.point.X);
	deltaY = (int32_t)(aim_y - Coordinate.point.Y);
	
	Curve->x_setted = (uint32_t)sqrt((double)(deltaX * deltaX + deltaY * deltaY));//������Ŀ���ľ���
	
	Curve->x_setted *= 1000;		//��λ���㣬����λ����΢��
	
	/*  ����Ԥ�м��ٶ�  */	//������λ�ƺ�ʱ�䣬�����������ļ��ٶ�ֵ��
	if(t_target == 0)		//���Ͱ�ȫ����ֹ���˸�ʱ��
	{
		Curve->a_cacul = Vol_Curve.a_max;
	}
	else
	{
		Curve->a_cacul = (4 * (double)Curve->x_setted / (t_target * t_target)) * 1000;//���ٶȵ�λ��mm/s^2
	}
	
	
	/*  ��Ŀ��ʱ����ıȽ϶�  *///������������Ҫ�ļ��ٶȴ��ڵ�������ṩ�������ٶȣ�
	if(Curve->a_cacul > Vol_Curve.a_max)		
	{
		Curve->a_cacul = Vol_Curve.a_max;
		
		/*  �ش�����Ŀ��㽫���ѵ�ʱ�䣨��ʾ��������ʱ����̣�  */
		Vol_Curve.MotionOverflowflag = true;	
	}
	else
	{
		Vol_Curve.MotionOverflowflag = false;
	}
	
	/*  ����Ԥ��λ��  */		//����������ٶȺ������ٶȣ���������ڼ��ٺͼ��ٹ���ʱ��Ӧ��λ�ƣ�
	Curve->x_stan = 2 * (Vol_Curve.vol_max * Vol_Curve.vol_max / Curve->a_cacul) * 1000;	//ע���ʱλ�Ƶ�λ��um
	
	/*  �������β���������λ������  */
	if(Curve->x_setted > Curve->x_stan)			
	{	
		Vol_Curve.Move_flag = Trapezoid;					//���Ϊ��������
		
		Curve->x_stan = 2 * (Vol_Curve.vol_max / Curve->a_cacul);	//��ȡԤ�б���ʱ��
		
		Curve->t = Curve->x_stan + (Curve->x_setted - Curve->x_stan) / Vol_Curve.vol_max;//��ȡ�˶���ʱ��
	}
	/*  �����������������ߵ���Ŀ���  */
	else 
	{	
		Vol_Curve.Move_flag = Triangle;		//���Ϊ����������
		
		Curve->x_stan = Curve->x_setted;				//��ȡԤ�б���λ��
		
		//��ȡԤ�б���ʱ��(��λms)
		Curve->t_stan = (uint32_t)(sqrt((Curve->x_setted / 1000) / Curve->a_cacul) * 1000 * 2);
		
		Curve->t = Curve->t_stan;						//��ʱ�伴Ϊ����ʱ��
	}
	
	Curve->x_stan /= 1000;				//���㵥λΪ����
	Curve->x_setted /= 1000;				//���㵥λΪ����
}

/**
  * @brief  �ٶ�������صĵ�Ƶ����
  * @param  None
  * @retval None
  */
void CurveLoop(double Distance)
{
	static uint32_t time = 0;
	
	X_V (*p)(uint32_t time, uint32_t Distance, uint32_t t, const double* a, uint32_t x_setted, uint32_t t_stan, uint32_t x_stan);
	X_V temp = {0,0};
		
	if(Distance == 0)							//����Ѿ������յ�
	{
		Vol_Curve.CurveSpeed = 0;
		
		Vol_Curve.Arrive_flag = ARRIVE;
		
		return;
	}
	
	/*  ��δ�����ݴ�Χ�ڵ�Ŀ���  */
	if(Vol_Curve.Arrive_flag == NARRIVE)
	{
		if(Vol_Curve.New_flag == GET)			//���յ�һ��������
		{
			time = 0;
			Vol_Curve.New_flag = NGET;
		}
		else
		{
			time += T;
		}
		
		if(Vol_Curve.Move_flag == Trapezoid)
		{
			p = Trapezoid_Loop;
		}
		else if(Vol_Curve.Move_flag == Triangle)
		{
			p = Triangle_Loop;
		}
		
		temp = p(time, Distance, Vol_Curve.t, &(Vol_Curve.a_cacul),
				Vol_Curve.x_setted, Vol_Curve.t_stan, Vol_Curve.x_stan);	//��ȡ��ǰӦ�ﵽ��λ�ƺ��ٶ�
		
		//��÷�������
		Vol_Curve.RealDisplace = pidData_Time.feedback = Vol_Curve.x_setted - Distance;
		
		//���Ŀ��λ��
		Vol_Curve.ExpectedDisplace = pidData_Time.set = temp.x;
		
		//����PID����
		PIDdataUpdate(&pidData_Time);
		
		//����PID
		GetPID(&pidData_Time);
		
		//�õ�����������������������
		Vol_Curve.CurveSpeed = (int32_t)temp.v + pidData_Time.out;
		
		//ֱ��λ�ñջ�
		Line_Location_Loop(MOV_Line);

		
	}
	/*  ���ѵ����ݴ�Χ�ڵ�Ŀ���  */
	else
	{
		time = 0;
		
		Vol_Curve.CurveSpeed = 0;
	}
}

/**
  * @brief  �ٶ����߲�����ʼ�����ٶȵ�λ������/�� or ΢��/���룩
  * @param  None			���ٶȵ�λ����/��^2 or ΢��/����^2
  * @retval None
  */
void CurveParamInit(void)
{	
	Vol_Curve.Move_flag = Triangle;
	Vol_Curve.New_flag = NGET;
	Vol_Curve.Arrive_flag = ARRIVE;
}

/**
  * @brief  ���ݽ��պ��һϵ�в����������ٶ�����
  * @param  None
  * @retval None
  */
void CreatCurve(double aimx, double aimy, uint32_t aimt)
{
	Distance_curve(aimx, aimy, aimt, &Vol_Curve);
	
	Vol_Curve.Arrive_flag = NARRIVE;
	Vol_Curve.New_flag = GET;
}


/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
