#include "PositionCtrl.h"
#include "Motion_Ctrl.h"
#include "Vol_curve.h"
#include "Process.h"
#include "Base.h"
#include "Map.h"

#include <Math.h>

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))		//输出限幅	
#define ANGLE_TO_RAD(x) (x)/180.0f*3.14159f
#define RAD_TO_ANGLE(x) (x)/3.14159f*180.0f

/* Public  variables ---------------------------------------------------------*/
struct PID pidData_Time;						//时间闭环——内环PID量
struct PID pidData_End;							//终点闭环——内环PID量
struct PID pidData_Location;					//行进中的位置闭环——内环PID量

MotionParamType Motion;							//运动参量

/**
* @brief  点到另一点的直线距离与方向角计算
* @param  line_posx：目标点x坐标
* @param  line_posy：目标点y坐标
* @param  posx：实际值
* @param  posy：实际值
* @retval None
*/
static void distanceCal(double line_posx, double line_posy,
						double posx, double posy, double* distan, double* LineWard)
{
	if(line_posx == posx)				//类型安全
	{
		*LineWard = 90;
	}
	else
	{	
		if((0 == line_posy-posy) && (line_posx-posx < 0))
		{
			*LineWard = 180;
		}
		else
		{
			*LineWard = RAD_TO_ANGLE(atan((double)(line_posy-posy)/(double)(line_posx-posx)));
			*LineWard = (*LineWard < 0.0)?(*LineWard + 180):*LineWard;	//运算中的Ward取值范围是0->180度

		}
		
	}
	*distan = sqrt((line_posx-posx)*(line_posx-posx)+(line_posy-posy)*(line_posy-posy));
}

/**
  * @brief  PID参数初始化
  * @param  无
  * @retval 无
  */
void PIDInit(void)
{	
	PIDparamUpdate(&pidData_Time, 200, 0, 0, PID_TIME_I_MAX * 100, PID_TIME_OUT_MAX * 100);
	
	PIDparamUpdate(&pidData_End, 800, 0, 0, PID_END_I_MAX * 100, PID_END_OUT_MAX * 100);
	
	PIDparamUpdate(&pidData_Location, 400, 0, 0, PID_Location_I_MAX * 100, PID_Location_OUT_MAX * 100);
	
}	


/**
  * @brief 	进行运动规划
  * @param  None
  * @retval None
  */
void AfterGetAim(double aimx, double aimy, uint32_t aimt)
{
	//设置新的目标坐标并生成速度曲线
	CreatCurve(aimx, aimy, aimt);
	
	//生成直线轨迹
	CreatMoveLine(&MOV_Line, aimx, aimy, &Coordinate);
	
	//获取终点闭环区域
	getEndDistance(Vol_Curve.a_cacul, pidData_End);
	
	Motion.isENDLoop = false;
}
	

/**
  * @brief  运算：目标模式
  * @param  None
  * @retval None
  */
void AimMode_Loop(void)
{
	double Line_Ward = 0;

	//获取运动方向和与目标点的直线距离
	distanceCal(Base_CMD.aimx, Base_CMD.aimy ,Coordinate.point.X, Coordinate.point.Y, &(Motion.distance), &Line_Ward);
		
	/*  若进入终点闭环状态  */
	if(Motion.distance <= End_Distance)
	{
		if(!Motion.isENDLoop)	//初次进入，则切换状态
		{
			ResetI(&pidData_End);//清除积分
			Motion.isENDLoop = true;
			Vol_Curve.Arrive_flag = ARRIVE;
		}
			
		END_Loop(Motion.distance);	
		
		Motion.outSpeed = EndSpeed;
		Motion.Ward = Line_Ward;
			
		if((Base_CMD.aimy - Coordinate.point.Y) < 0)	//△y为负值，说明从当前点指向目标点的向量在第三/四象限，于是速度为负值
		{
			Motion.outSpeed = -Motion.outSpeed;
		}
	
	}
	//这是一种错误状态，就是机器人因为速度过快而收不住导致冲出终点闭环区域的情况
	else if(Motion.distance > End_Distance && ARRIVE == Vol_Curve.Arrive_flag)
	{
		Motion.outSpeed = 300;
		Motion.Ward = Line_Ward;
			
		if((Base_CMD.aimy - Coordinate.point.Y) < 0)	//△y为负值，说明从当前点指向目标点的向量在第三/四象限，于是速度为负值
		{
			Motion.outSpeed = -Motion.outSpeed;
		}
	}
	/*  若未进入终点闭环区域  */
	else
	{	
		if(Motion.isENDLoop)		//初次进入，则切换状态
		{
			ResetI(&pidData_Time); 	//清除积分
			Motion.isENDLoop = false;
		}
		
		CurveLoop(Motion.distance);
		
		if((Base_CMD.aimy - Coordinate.point.Y)<0)	//△y为负值，说明从当前点指向目标点的向量在第三/四象限，于是速度为负值
		{
			Vol_Curve.CurveSpeed = -Vol_Curve.CurveSpeed;
		}
							
		MixSpeed((double)(Vol_Curve.CurveSpeed), Line_Ward, AdjSpeed, MOV_Line, Coordinate, &(Motion.outSpeed), &(Motion.Ward));

	}
}

/**
  * @brief  运算：遥控模式
  * @param  None
  * @retval None
  */
void OpenMode_Loop(void)
{
	Motion.isENDLoop = false;
	
	Motion.outSpeed = Base_CMD.line_speed;
	Motion.Ward = Base_CMD.ward;
}

/************************ (C) COPYRIGHT 2017 YANGYIFAN *****END OF FILE****/
