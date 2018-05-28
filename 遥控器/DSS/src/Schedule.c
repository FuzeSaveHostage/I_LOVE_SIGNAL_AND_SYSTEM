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

static void Loop_1000Hz(void)	//1msִ��һ��
{
	
}

static void Loop_200Hz(void)	//5ms����һ��
{	
	static char DealFlag_5ms = 0;
	
	SendDataToF103(PID_flag);
	PID_flag = 0;
	
	if(DealFlag_5ms == 0)
	{	
		//��ȡʵʱλ����Ϣ
		UpdateRealCoordinate();
	//	SendDataToMaster();
		
		DealFlag_5ms = 1;
	}
	else												
	{
		Base_Loop();	//�൱��10msһ��ִ��

		DealFlag_5ms = 0;
	}
	
	if(BeepMutexflag != 0)
	{
		//ͨ�������ʾ������
		ShutUp();
	}
}



static void Loop_50Hz(void)	//20msִ��һ��
{	
	//����ʵʱ����
	flagCheck();
	
	SendDataToMaster();
	SendDataToPC();
	SendDataToEquips();
}

static void Loop_20Hz(void)	//50msִ��һ��
{	
	Controler_handle();
}
/**
  * @brief  ���ȶ�ʱ����ʼ��
  * @param  ��
  * @retval ��
  */
void Schedule_TIM_Init(void)
{
	ScheduleTIM_Config();	
}


/**
  * @brief  ѡ���������
  * @param  ��
  * @retval ��
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
