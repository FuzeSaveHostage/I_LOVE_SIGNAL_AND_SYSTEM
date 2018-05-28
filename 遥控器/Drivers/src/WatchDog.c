#include "InterfaceToMaster.h"
#include "InterfaceToF103.h"
#include "STM32F4xx.h"
#include "WatchDog.h"
#include "USART.h"
#include "TIMs.h"
#include "Beep.h"
#include "Base.h"

/* Private  variables ---------------------------------------------------------*/
Flags InfoChannel_flag[FlagsNum];	//信道状态：默认为阻塞（不通）

/**
  * @brief  待监测参量初始化
  * @param  None
  * @retval None
  */
static void WatchDogParaInit(void)
{
	for(FlagsTypes i = Master; i < FlagsNum; i = (FlagsTypes)(i+1))
	{
		InfoChannel_flag[i].State = Blocked;
		InfoChannel_flag[i].LastState = Unblocked;
	}
}


/**
  * @brief  模拟看门狗初始化
  * @param  None
  * @retval None
  */
void WatchDogInit(void)
{
	WatchDogParaInit();			//待监测变量初始化
	WatchDogTIM_Config();		//模拟看门狗定时器初始化
}

/**
  * @brief  置位标志(喂狗)
  * @param  对应标志的标识符
  * @retval None
  */
void FeedDog(FlagsTypes flag)
{
	InfoChannel_flag[flag].State = Unblocked;
}

/**
  * @brief  标志检查
  * @param  待检查的标志
  * @retval 若处于阻塞状态，则返回true
  */
static bool Checkflags(Flags &flag)
{
	bool isBlocked = false;
	
	if(Blocked == flag.State)
	{
		if(Blocked == flag.LastState)
		{
			isBlocked = true;
		}
	}
	flag.LastState = flag.State;
	flag.State = Blocked;
	
	return isBlocked;
}

/**
  * @brief  标志检查与示警产生（看门狗定时器中断中调用）
  * @param  None
  * @retval None
  */
void Bark(void)
{
	if(Checkflags(InfoChannel_flag[Master]))		//若跟上位机的通讯中断
	{
		BeepingON(1000);
		Base_CMD.R_Ctrl_Flag = 1;			//置为遥控模式
		Base_CMD.line_speed = 0;			//刹车	
	}
	
	if(Checkflags(InfoChannel_flag[F103]))	//若跟F1的通讯中断
	{
		BeepingON(900);
	}
	
//	if(Checkflags(InfoChannel_flag[MicroPC]))	//若跟小电脑的通讯中断
//	{
//		BeepingON(800);
//	}
//	
//	if(Checkflags(InfoChannel_flag[Equips]))	//若跟次主控的通讯中断
//	{
//		BeepingON(700);
//	}
	
	if(Checkflags(InfoChannel_flag[Map]))	//若跟全局定位的通讯中断
	{
		BeepingON(600);
		Base_CMD.R_Ctrl_Flag = 1;			//置为遥控模式
		Base_CMD.line_speed = 0;			//刹车	
	}
}

/**
  * @brief  示警消除（调度定时器中调用，调用频率须高于Bark函数）
  * @param  None
  * @retval None
  */
void ShutUp(void)
{
	bool isAllUnblocked = true;
	
	for(FlagsTypes i = Master; i < FlagsNum; i = (FlagsTypes)(i+1))
	{
		if(InfoChannel_flag[i].LastState == Blocked)
			isAllUnblocked = false;
	}
	
	if(isAllUnblocked)			//如果所有的标志都进入非阻塞，则关掉蜂鸣器
	{
		BeepingOFF();
	}
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
