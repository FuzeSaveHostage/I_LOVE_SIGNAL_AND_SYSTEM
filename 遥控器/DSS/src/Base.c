#include "Motion_Ctrl.h"
#include "ActionCtrl.h"
#include "Vol_curve.h"
#include "Schedule.h"
#include "WatchDog.h"
#include "Process.h"
#include "USART.h"
#include "Base.h"
#include "Beep.h"
#include "Map.h"

struct CMD 	Base_CMD;
Base_flags 	Baseflags = {false, false, true};
enum AirPressureStateType AirPressureState = AirNorm_TZ1;

/**
  * @brief  初始化控制量
  * @param  无
  * @retval 无
  */
void CtrlPara_Init(void)
{
	Base_CMD.R_Ctrl_Flag = OpenLoopMode;	//默认为开环控制
	
	Base_CMD.aimx = 0;
	Base_CMD.aimy = 0;
		
	Base_CMD.line_speed = 0;
	Base_CMD.ward = 0;
	
	Base_CMD.Pos_Angle = 0;
	
	Base_CMD.Angle_cnt = 0;
}

/**
  * @brief  底盘初始化
  * @param  无
  * @retval 无
  */
void baseInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	InitCylinderState();
	
	USART_Config();								//串口初始化
	
	PIDInit();									//PID参量初始化
		
	MovePointsInit(MovePoint);					//初始化运动点
		
	CtrlPara_Init();							//控制量初始化
	
	CurveParamInit();							//速度曲线参量初始化
		
	BeepInit();									//蜂鸣器初始化
	
//	while(false == Baseflags.InitSuccess);
	
	WatchDogInit();								//模拟看门狗初始化
	
	Schedule_TIM_Init();						//调度定时器初始化	
	
//	LightCtrl(0, 0, 255, 100);					//设定灯条为蓝色
	
//	SpinDeviceCtrl(Spin_GetBall);				//设定旋转装置初始位置

}

/**
  * @brief  提示音扫描
  * @param  None
  * @retval None
  */
void PromptToneScan(void)
{
	
	if(true == Baseflags.RcvPIDSuccess)									//接收提示音
	{
		Baseflags.RcvPIDSuccess = false;
		InitCompleteFlag(PIDUpdate);
	}
	
	if(true == Baseflags.restart && true == Baseflags.InitSuccess)		//初始化完成
	{
		Baseflags.restart = false;
		InitCompleteFlag(Start);
	}
}

/**
  * @brief  更新目标姿态角
  * @param  aimAngle:目标姿态角
  * @retval 无
  */
void AimAngleUpdate(double aimAngle)
{
	Base_CMD.Pos_Angle = aimAngle;
				
	Base_CMD.Angle_cnt = Angle_cnt;	//记下发布新命令时的圈数，作为之后姿态角闭环的参考值
}

/**
  * @brief  在手动模式对目标坐标做记录
  * @param  None
  * @retval None
  */
void saveAimPosition(void)
{
	Base_CMD.aimx = Coordinate.point.X;
	Base_CMD.aimy = Coordinate.point.Y;
}

/**
  * @brief  运算
  * @param  None
  * @retval None
  */
void Base_Loop(void)
{
	/*  定位模式  */
	if(Base_CMD.R_Ctrl_Flag == AimMode)
	{
		AimMode_Loop();
	}
	/*  开环遥控模式  */
	else if(Base_CMD.R_Ctrl_Flag == OpenLoopMode)
	{
		OpenMode_Loop();
		saveAimPosition();
	}
}
