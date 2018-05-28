#include "STM32F4xx.h"
#include "TIMs.h"
#include "Beep.h"

bool BeepMutexflag = false;		//0��ʾ��ռ�ã�1��ʾ����ռ��

void delay_ms(unsigned int t);
void delay_us(unsigned int t);

/**
  * @brief  ��ʼ��������
  * @param  None
  * @retval None
  */
 void BeepInit(void)
 {
	 Beep_TIM_Config();			//��������ʱ����ʼ��
 }

/**
  * @brief  ʹ��������һ��������
  * @param  frequency���������ڣ�������10->1000��ֵ
  * @retval None
  */
void BeepingON(uint16_t frequency)
{
	frequency --;
	
	TIM_SetAutoreload(Beep_TIMx, frequency);
	TIM_SetCompare3(Beep_TIMx, frequency/2);
	TIM_CCxCmd(Beep_TIMx, Beep_Channel, TIM_CCx_Enable);	
}

/**
  * @brief  �رշ�����
  * @param  None
  * @retval None
  */
void BeepingOFF(void)
{
	TIM_CCxCmd(Beep_TIMx, Beep_Channel, TIM_CCx_Disable);
}

/**
  * @brief  ��ʼ����ɱ�־
  * @param  None
  * @retval None
  */
void InitCompleteFlag(InitTypes Type)
{
	BeepMutexflag = false;
	
	switch(Type)
	{
		case Start:
					BeepingON(850);
					delay_ms(400);
					
					BeepingON(760);
					delay_ms(400);
					
					BeepingON(670);
					delay_ms(400);
					
					BeepingOFF();
					break;
		
		case PIDUpdate:
					BeepingON(770);
					delay_ms(400);
					
					BeepingON(860);
					delay_ms(400);
					
					BeepingON(950);
					delay_ms(400);
					
					BeepingOFF();
					break;
		
		default:break;
	}
	
	
	BeepMutexflag = true;
}

/**
  * @brief  ����ȷ��ʱ
  * @param  ����ʱ�ĺ�����
  * @retval None
  */
void delay_ms(unsigned int t)
{
	int i;
	for( i = 0;i<t;i++)
	{
		int a = 10300;
 		while(a--);
	}
}

/**
  * @brief  ����ȷ��ʱ
  * @param  ����ʱ��΢����
  * @retval None
  */
void delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a = 9;
		while(a--);
	}
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
