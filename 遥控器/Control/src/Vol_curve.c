#include "PositionCtrl.h"
#include "Motion_Ctrl.h"
#include "Vol_curve.h"
#include <math.h>
#include "Map.h"

/*
	改动说明：
	1.决策仅做一次，给出信号量，决定之后进入何种运算（以及该运算的各个参数），这一步在通信接收完成时进行
	2.在通信接收完目标坐标之后，要把接收标志置位，把未到达标志复位，并对选择何种曲线到达目标进行决策，并给出相应参数
	3.简化逻辑与运算：三角形曲线只出现一次，不管时间符不符合要求，决策时便可以确定整个运动的加速度、总时间以及最大速度；
	4.以运算时间为单位，每运算一次，加一个运算周期的时间，在新的指令到来或者到达目标点并稳定之后清零并停止运算

	5.在预测时间过短的时候，应给上位机反馈一个预测到达的时间，并反馈时间过短(未完成)；
	6.时间单位为毫秒，位移单位为毫米，（速度单位：毫米/秒 or 微米/毫秒）
										加速度单位：mm/s^2
*/

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))		//输出限幅
#define T 10							//运算周期（10ms）

/* Public  variables ---------------------------------------------------------*/
Vol_CurveType Vol_Curve;


/**
  * @brief  当位移可以通过三角形实现的时候调用的运算——三角形曲线（位移-时间、速度-位移、时间-位移关系）
  * @param  time:从计时起点到当前的时间（单位：毫秒）
  * @param  distance:当前坐标到目标点的距离
  * @param  t:规划出来的总运动时间
  * @param  a:规划出来的运动加速度
  * @param  x_setted:规划的运动总位移
  * @param  t_stan:预判变速总时间
  * @param  x_stan:预判变速总位移
  * @retval 该时刻应该到达的位移、理论上应该达到的速度以及理论上到达该点所花的时间
  */
static X_V Triangle_Loop(uint32_t time, uint32_t distance, uint32_t t, const double* a,
							uint32_t x_setted, uint32_t t_stan, uint32_t x_stan)
{
	static X_V temp = {0,0};
	
	int32_t displace = x_setted - distance;//已运动的位移
	uint32_t half_time = t_stan/2;
	int32_t half_displace = x_setted/2;
	
	/************  x-t关系  ************/
	
	/*  若处于加速段  */
	if(time <= half_time)					
	{
		temp.x = (int32_t)((*a) * (time * time) / 2.0 /1000000);
		
	}
	/*  若处于减速段  */
	else if(time <= t_stan)					
	{
		uint32_t tem_t1 = t_stan - time;						
		
		temp.x = x_stan - (int32_t)((*a) * (tem_t1 * tem_t1) / 2.0 / 1000000);
	}
	
	/************  v-x关系  ************/
	
	/*  若位移为负  */
	if(displace < 0)
	{
		temp.v = 30;						//保留一个小速度，进行矫正
	}
	/*  若处于加速段  */
	else if(displace <= half_displace)					
	{	
		temp.v = (int32_t)sqrt(2 * (*a) * displace);
	}
	/*  若处于减速段  */
	else if(displace <= x_setted)					
	{	
		temp.v = (int32_t)sqrt(2 * (*a) * distance);
	}
	
	return temp;
}

/**
  * @brief  当三角形不足以满足位移需求的时候调用的运算——梯形曲线（位移-时间、速度-位移、时间-位移关系）
  * @param  time:从计时起点到当前的时间（单位：毫秒）
  * @param  distance:当前坐标到目标点的距离
  * @param  t:规划出来的总运动时间
  * @param  a:规划出来的运动加速度
  * @param  x_setted:规划的运动总位移
  * @param  t_stan:预判变速总时间
  * @param  x_stan:预判变速总位移
  * @retval 该时刻应该到达的位移、理论上应该达到的速度以及理论上到达该点所花的时间
  */
static X_V Trapezoid_Loop(uint32_t time, uint32_t distance, uint32_t t, const double* a,
							uint32_t x_setted, uint32_t t_stan, uint32_t x_stan)
{
	static X_V temp;
	
	int32_t displace = x_setted - distance;//已运动的位移
	uint32_t tem_t = t - t_stan;			//匀速运动的时间
	uint32_t half_stan = x_stan/2;
	
	/************  x-t关系  ************/
	
	/*  若处于加速时间段  */
	if(time <= (t_stan/2))				
	{
		temp.x = (int32_t)((*a) * (time * time) / 2.0 /1000000);
	}
	/*  若处于匀速时间段  */
	else if(time <= (t_stan/2 + tem_t))
	{
		temp.x = x_stan/2 + Vol_Curve.vol_max * (time - t_stan/2) / 1000;
	}
	/*  若处于减速时间段  */
	else if(time <= t)				
	{
		uint32_t tem_t1 = t - time;			//获取剩余运动时间
		
		temp.x = x_setted - (int32_t)((*a) * (tem_t1 * tem_t1) / 2.0 / 1000000);
	}
	
	/************  v-x关系  ************/
	
	/*  若位移为负  */
	if(displace < 0)
	{
		temp.v = 0;//30;						//保留一个小速度，进行矫正
	}
	/*  若处于加速段  */
	else if(displace <= half_stan)				
	{
		temp.v = (int32_t)sqrt(2 * (*a) * displace);
	}
	/*  若处于匀速时间段  */
	else if(displace <= (x_setted - half_stan))
	{
		temp.v = Vol_Curve.vol_max ;
	}
	/*  若处于减速时间段  */
	else if(displace <= x_setted)				
	{	
		temp.v = (int32_t)sqrt(2 * (*a) * distance);
	}
	
	return temp;
		
}

/**
  * @brief  运动规划：	决定以何种曲线到达目标点
  * @param  aim_x：		目标坐标X（单位长度为1mm）
  * @param  aim_y：		目标坐标Y（单位长度为1mm）
  * @param  t_target：	期望从当前位置到达目标点所花费的时间（单位为毫秒）
  * @param  Pt_stan：	指向存储预判变速时间的变量的指针（单位为毫秒）
  * @param  Px_stan：	指向存储预判变速位移的变量的指针（单位长度为1mm）
  * @retval None
  */
static void Distance_curve(double aim_x, double aim_y, uint32_t t_target, Vol_CurveType* Curve)
{
	int32_t deltaX = 0,deltaY = 0;
	
	deltaX = (int32_t)(aim_x - Coordinate.point.X);
	deltaY = (int32_t)(aim_y - Coordinate.point.Y);
	
	Curve->x_setted = (uint32_t)sqrt((double)(deltaX * deltaX + deltaY * deltaY));//计算与目标点的距离
	
	Curve->x_setted *= 1000;		//单位换算，将单位换成微米
	
	/*  计算预判加速度  */	//（给定位移和时间，计算符合需求的加速度值）
	if(t_target == 0)		//类型安全，防止忘了给时间
	{
		Curve->a_cacul = Vol_Curve.a_max;
	}
	else
	{
		Curve->a_cacul = (4 * (double)Curve->x_setted / (t_target * t_target)) * 1000;//加速度单位是mm/s^2
	}
	
	
	/*  若目标时间给的比较短  *///（以至于所需要的加速度大于电机所能提供的最大加速度）
	if(Curve->a_cacul > Vol_Curve.a_max)		
	{
		Curve->a_cacul = Vol_Curve.a_max;
		
		/*  回传到达目标点将花费的时间（提示给的期望时间过短）  */
		Vol_Curve.MotionOverflowflag = true;	
	}
	else
	{
		Vol_Curve.MotionOverflowflag = false;
	}
	
	/*  计算预判位移  */		//（给定最大速度和最大加速度，计算仅存在加速和减速过程时对应的位移）
	Curve->x_stan = 2 * (Vol_Curve.vol_max * Vol_Curve.vol_max / Curve->a_cacul) * 1000;	//注意此时位移单位是um
	
	/*  若三角形不足以满足位移需求  */
	if(Curve->x_setted > Curve->x_stan)			
	{	
		Vol_Curve.Move_flag = Trapezoid;					//标记为梯形曲线
		
		Curve->x_stan = 2 * (Vol_Curve.vol_max / Curve->a_cacul);	//获取预判变速时间
		
		Curve->t = Curve->x_stan + (Curve->x_setted - Curve->x_stan) / Vol_Curve.vol_max;//获取运动总时间
	}
	/*  若可以以三角形曲线到达目标点  */
	else 
	{	
		Vol_Curve.Move_flag = Triangle;		//标记为三角形曲线
		
		Curve->x_stan = Curve->x_setted;				//获取预判变速位移
		
		//获取预判变速时间(单位ms)
		Curve->t_stan = (uint32_t)(sqrt((Curve->x_setted / 1000) / Curve->a_cacul) * 1000 * 2);
		
		Curve->t = Curve->t_stan;						//总时间即为变速时间
	}
	
	Curve->x_stan /= 1000;				//换算单位为毫米
	Curve->x_setted /= 1000;				//换算单位为毫米
}

/**
  * @brief  速度曲线相关的低频运算
  * @param  None
  * @retval None
  */
void CurveLoop(double Distance)
{
	static uint32_t time = 0;
	
	X_V (*p)(uint32_t time, uint32_t Distance, uint32_t t, const double* a, uint32_t x_setted, uint32_t t_stan, uint32_t x_stan);
	X_V temp = {0,0};
		
	if(Distance == 0)							//如果已经到达终点
	{
		Vol_Curve.CurveSpeed = 0;
		
		Vol_Curve.Arrive_flag = ARRIVE;
		
		return;
	}
	
	/*  若未到达容错范围内的目标点  */
	if(Vol_Curve.Arrive_flag == NARRIVE)
	{
		if(Vol_Curve.New_flag == GET)			//若收到一个新坐标
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
				Vol_Curve.x_setted, Vol_Curve.t_stan, Vol_Curve.x_stan);	//获取当前应达到的位移和速度
		
		//获得反馈数据
		Vol_Curve.RealDisplace = pidData_Time.feedback = Vol_Curve.x_setted - Distance;
		
		//获得目标位移
		Vol_Curve.ExpectedDisplace = pidData_Time.set = temp.x;
		
		//更新PID数据
		PIDdataUpdate(&pidData_Time);
		
		//计算PID
		GetPID(&pidData_Time);
		
		//得到输出量（此项需调整比例）
		Vol_Curve.CurveSpeed = (int32_t)temp.v + pidData_Time.out;
		
		//直线位置闭环
		Line_Location_Loop(MOV_Line);

		
	}
	/*  若已到达容错范围内的目标点  */
	else
	{
		time = 0;
		
		Vol_Curve.CurveSpeed = 0;
	}
}

/**
  * @brief  速度曲线参量初始化（速度单位：毫米/秒 or 微米/毫秒）
  * @param  None			加速度单位：米/秒^2 or 微米/毫秒^2
  * @retval None
  */
void CurveParamInit(void)
{	
	Vol_Curve.Move_flag = Triangle;
	Vol_Curve.New_flag = NGET;
	Vol_Curve.Arrive_flag = ARRIVE;
}

/**
  * @brief  数据接收后的一系列操作，生成速度曲线
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
