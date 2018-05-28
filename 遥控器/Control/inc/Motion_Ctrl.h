#ifndef __MOTION_CTRL_H
#define __MOTION_CTRL_H


#include <stdint.h>
#include "Macros.h"
#include "PID.h"

typedef struct
{
	int32_t outSpeed;				//输出合速度
	double Ward;					//计算所得的运动方向
	
	bool isENDLoop;					//终点状态进入标志（仅在Low_Frequency_Loop函数中改变值）
	
	double distance;				//计算所得的距离

	
}MotionParamType;

/* Public  macro --------------------------*/
#define PID_END_I_MAX     		2000     	//终点闭环的积分限幅
#define PID_END_OUT_MAX     	4000     	//终点闭环的输出最大值

#define PID_TIME_I_MAX     		2000     	//时间闭环的积分限幅
#define PID_TIME_OUT_MAX   		4000     	//时间闭环的输出最大值

#define PID_Location_I_MAX     	2000     	//行进中位置闭环的积分限幅
#define PID_Location_OUT_MAX   	3000     	//行进中位置闭环的输出最大值


/* Public  variables ----------------------*/
extern struct PID pidData_Time;					//时间闭环——内环PID量——速度补偿
extern struct PID pidData_End;					//终点闭环——内环PID量
extern struct PID pidData_Location;

extern MotionParamType Motion;					//运动参量

/**		参量初始化		**/
void PIDInit(void);
void AfterGetAim(double aimx, double aimy, uint32_t aimt);

/**		定时调用运算的函数		**/
void AimMode_Loop(void);
void OpenMode_Loop(void);

#endif

/******************* (C) COPYRIGHT 2017 YANGYIFAN *****END OF FILE****/
