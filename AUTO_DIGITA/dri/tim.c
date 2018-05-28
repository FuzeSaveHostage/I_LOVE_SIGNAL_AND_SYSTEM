#include "tim.h"

void TIM_SCHEDULAR_Config()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	

	// 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=999;
	
	// 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= 71;	
	
	// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	
	// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  
	// 初始化定时器
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	 
	// 清除计数器中断标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	  
	// 开启计数器中断
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
	// 使能计数器
    TIM_Cmd(TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}

void TIM_THREAD_Config()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
	
	// 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=999;
	
	// 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= 71;	
	
	// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	
	// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  
	// 初始化定时器
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		
	// 清除计数器中断标志位
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	  
	// 开启计数器中断
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
	// 使能计数器
    TIM_Cmd(TIM3, ENABLE);
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, DISABLE);
}
