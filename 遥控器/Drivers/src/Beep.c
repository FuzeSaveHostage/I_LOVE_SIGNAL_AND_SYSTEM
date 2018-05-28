#include "STM32F4xx.h"
#include "TIMs.h"
#include "Beep.h"

bool BeepMutexflag = false;		//0表示无占用，1表示正在占用

void delay_ms(unsigned int t);
void delay_us(unsigned int t);

/**
  * @brief  初始化蜂鸣器
  * @param  None
  * @retval None
  */
 void BeepInit(void)
 {
	 Beep_TIM_Config();			//蜂鸣器定时器初始化
 }

/**
  * @brief  使蜂鸣器以一定音调响
  * @param  frequency：激励周期，可以是10->1000的值
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
  * @brief  关闭蜂鸣器
  * @param  None
  * @retval None
  */
void BeepingOFF(void)
{
	TIM_CCxCmd(Beep_TIMx, Beep_Channel, TIM_CCx_Disable);
}

/**
  * @brief  初始化完成标志
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
  * @brief  不精确延时
  * @param  待延时的毫秒数
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
  * @brief  不精确延时
  * @param  待延时的微秒数
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
