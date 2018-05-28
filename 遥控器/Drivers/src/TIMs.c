#include "STM32F4xx.h"
#include "TIMs.h"

 /**
  * @brief  调度器 中断向量配置
  * @param  无
  * @retval 无
  */
static void ScheduleTIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
	/* 配置TIM7为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = Schedule_TIM_IRQ;
	
	/* 抢占优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	
	/* 响应优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  调度定时器配置,工作参数配置
  * @param  无
  * @retval 无
  */
void ScheduleTIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	
	// 打开外设时钟
	Schedule_TIM_ClkCmd(Schedule_TIM_CLK, ENABLE);
	
	//设置计数模式
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	
	//设置计数周期
	TIM_InitStructure.TIM_Period = 1000 - 1;				//计数周期为1ms
	
	//设置预分频系数
	TIM_InitStructure.TIM_Prescaler = 90 - 1;				//挂载在APB1总线上，倍频后频率90MHz
	
	//初始化定时器
	TIM_TimeBaseInit(Schedule_TIMx, &TIM_InitStructure);
	
	// 使能定时器中断
	TIM_ITConfig(Schedule_TIMx, TIM_IT_Update, ENABLE);	
	 
	//初始化定时器中断向量
	ScheduleTIM_NVIC_Config();
	
	// 使能定时器
	TIM_Cmd(Schedule_TIMx, ENABLE);	    
}

/**
  * @brief  模拟看门狗定时器中断向量配置
  * @param  无
  * @retval 无
  */
static void WatchDogTIM_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	
	/* 配置TIM6为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = WatchDog_TIM_IRQ;
	
	/* 抢占优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	
	/* 响应优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  模拟看门狗定时器配置,工作参数配置（400ms）
  * @param  无
  * @retval 无
  */
void WatchDogTIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	
	// 打开外设时钟
	WatchDog_TIM_ClkCmd(WatchDog_TIM_CLK, ENABLE);
	
	//设置计数模式
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	
	//设置计数周期
	TIM_InitStructure.TIM_Period = 50000 - 1;				//计数周期为50 * 8ms
	
	//设置预分频系数
	TIM_InitStructure.TIM_Prescaler = 720 - 1;				//挂载在APB1总线上，倍频后频率90MHz
	
	//初始化定时器
	TIM_TimeBaseInit(WatchDog_TIMx, &TIM_InitStructure);
	
	// 使能定时器中断
	TIM_ITConfig(WatchDog_TIMx, TIM_IT_Update, ENABLE);	
	
	//初始化定时器中断向量
	WatchDogTIM_NVIC_Config();
	 
	// 使能定时器
	TIM_Cmd(WatchDog_TIMx, ENABLE);	    
}

/**
  * @brief  蜂鸣器驱动定时器配置,工作参数配置
  * @param  无
  * @retval 无
  */
void Beep_TIM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef OC_InitStructure;
	
	// 打开外设时钟
	Beep_TIM_ClkCmd(Beep_TIM_CLK, ENABLE);
	
	// 打开PWM输出GPIO的时钟
	Beep_GPIO_ClkCmd(Beep_GPIO_CLK, ENABLE);
	
	//使能复用，即将GPIO连接到目标外设
	GPIO_PinAFConfig(Beep_GPIO_PORT, Beep_GPIO_PinSource, Beep_GPIO_AF);
	
	/*  配置PWM输出GPIO  */
	GPIO_InitStructure.GPIO_Pin = Beep_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(Beep_GPIO_PORT, &GPIO_InitStructure);
	
	/*  配置时基单元  */
	//设置计数模式
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	
	//设置计数周期
	TIM_InitStructure.TIM_Period = 1000 - 1;				//计数周期为1ms
	
	//设置预分频系数
	TIM_InitStructure.TIM_Prescaler = 90 - 1;				//挂载在APB1总线上，倍频后90MHz
	
	//设置重复计数次数
	TIM_InitStructure.TIM_RepetitionCounter = 0;			//不重复计数
	
	//设置死区发生器、数字滤波器对定时器时钟频率的分频
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//不分频
	
	//初始化定时器
	TIM_TimeBaseInit(Beep_TIMx, &TIM_InitStructure);
	
	/*  配置输出通道  */
	//选择输入输出模式
	OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//选择为PWM1模式：该模式下当CNT的值小于预设值时，为输出有效状态
	
	//空闲状态下的引脚电平
	OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//配置为低电平
	
	//配置输出有效电平的极性
	OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//高电平有效
	
	//使能该通道
	OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	//初始脉宽
	OC_InitStructure.TIM_Pulse = 500;
	
	//关闭互补输出使能
	OC_InitStructure.TIM_OutputNState = TIM_OutputState_Disable;
	
	//高电平有效
	OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	//互补引脚空闲时的电平状态
	OC_InitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	BeepOCInit(Beep_TIMx, &OC_InitStructure);
	
	/* 使能通道重载 */
	Beep_OCPreloadConfig(Beep_TIMx, TIM_OCPreload_Enable);
	
	TIM_CCxCmd(Beep_TIMx, Beep_Channel, TIM_CCx_Disable);
	
	// 使能定时器
	TIM_Cmd(Beep_TIMx, ENABLE);	
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
