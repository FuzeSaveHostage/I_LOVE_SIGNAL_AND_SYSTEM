#include "InterfaceToMaster.h"
#include "InterfaceToF103.h"
#include "STM32F4xx.h"
#include "WatchDog.h"
#include "USART.h"
#include "TIMs.h"
#include "Beep.h"
#include "Base.h"

/* Private  variables ---------------------------------------------------------*/
Flags InfoChannel_flag[FlagsNum];	//�ŵ�״̬��Ĭ��Ϊ��������ͨ��

/**
  * @brief  ����������ʼ��
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
  * @brief  ģ�⿴�Ź���ʼ��
  * @param  None
  * @retval None
  */
void WatchDogInit(void)
{
	WatchDogParaInit();			//����������ʼ��
	WatchDogTIM_Config();		//ģ�⿴�Ź���ʱ����ʼ��
}

/**
  * @brief  ��λ��־(ι��)
  * @param  ��Ӧ��־�ı�ʶ��
  * @retval None
  */
void FeedDog(FlagsTypes flag)
{
	InfoChannel_flag[flag].State = Unblocked;
}

/**
  * @brief  ��־���
  * @param  �����ı�־
  * @retval ����������״̬���򷵻�true
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
  * @brief  ��־�����ʾ�����������Ź���ʱ���ж��е��ã�
  * @param  None
  * @retval None
  */
void Bark(void)
{
	if(Checkflags(InfoChannel_flag[Master]))		//������λ����ͨѶ�ж�
	{
		BeepingON(1000);
		Base_CMD.R_Ctrl_Flag = 1;			//��Ϊң��ģʽ
		Base_CMD.line_speed = 0;			//ɲ��	
	}
	
	if(Checkflags(InfoChannel_flag[F103]))	//����F1��ͨѶ�ж�
	{
		BeepingON(900);
	}
	
//	if(Checkflags(InfoChannel_flag[MicroPC]))	//����С���Ե�ͨѶ�ж�
//	{
//		BeepingON(800);
//	}
//	
//	if(Checkflags(InfoChannel_flag[Equips]))	//���������ص�ͨѶ�ж�
//	{
//		BeepingON(700);
//	}
	
	if(Checkflags(InfoChannel_flag[Map]))	//����ȫ�ֶ�λ��ͨѶ�ж�
	{
		BeepingON(600);
		Base_CMD.R_Ctrl_Flag = 1;			//��Ϊң��ģʽ
		Base_CMD.line_speed = 0;			//ɲ��	
	}
}

/**
  * @brief  ʾ�����������ȶ�ʱ���е��ã�����Ƶ�������Bark������
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
	
	if(isAllUnblocked)			//������еı�־���������������ص�������
	{
		BeepingOFF();
	}
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
