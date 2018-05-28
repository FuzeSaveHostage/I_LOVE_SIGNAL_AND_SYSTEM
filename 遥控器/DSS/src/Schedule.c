#include "InterfaceToMicroPC.h"
#include "InterfaceToEquips.h"
#include "InterfaceToMaster.h"
#include "InterfaceToF103.h"
#include "ActionCtrl.h"
#include "Motion_Ctrl.h"
#include "Schedule.h"
#include "WatchDog.h"
#include "TIMs.h"
#include "Base.h"
#include "Beep.h"
#include "MAP.h"
#include "Control.h"

struct Scheduler scheduler;

static void Loop_1000Hz(void)	//1ms执行一次
{
	
}

static void Loop_200Hz(void)	//5ms进入一次
{	
	static char DealFlag_5ms = 0;
	
	SendDataToF103(PID_flag);
	PID_flag = 0;
	
	if(DealFlag_5ms == 0)
	{	
		//获取实时位置信息
		UpdateRealCoordinate();
	//	SendDataToMaster();
		
		DealFlag_5ms = 1;
	}
	else												
	{
		Base_Loop();	//相当于10ms一次执行

		DealFlag_5ms = 0;
	}
	
	if(BeepMutexflag != 0)
	{
		//通道检查与示警消除
		ShutUp();
	}
}



static void Loop_50Hz(void)	//20ms执行一次
{	
	//发送实时数据
	flagCheck();
	
	SendDataToMaster();
	SendDataToPC();
	SendDataToEquips();
}

static void Loop_20Hz(void)	//50ms执行一次
{	
	Controler_handle();
}
/**
  * @brief  调度定时器初始化
  * @param  无
  * @retval 无
  */
void Schedule_TIM_Init(void)
{
	ScheduleTIM_Config();	
}


/**
  * @brief  选择进入运算
  * @param  无
  * @retval 无
  */
void Loops(void)
{
	if(scheduler.cnt_1ms >= 1){
		Loop_1000Hz();	
		scheduler.cnt_1ms = 0;
	}
	if(scheduler.cnt_5ms >= 5){
		Loop_200Hz();
		scheduler.cnt_5ms = 0;
	}		
	if(scheduler.cnt_20ms >= 20){		
		Loop_50Hz();
		scheduler.cnt_20ms = 0;
	}
	if(scheduler.cnt_50ms >= 50){		
		Loop_20Hz();
		scheduler.cnt_50ms = 0;
	}
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
