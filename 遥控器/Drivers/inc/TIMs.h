#ifndef _TIMS_H_
#define _TIMS_H_

/**  伪调度器——基本定时器TIM7  **/
#define  Schedule_TIMx				TIM7
#define  Schedule_TIM_CLK			RCC_APB1Periph_TIM7
#define  Schedule_TIM_ClkCmd		RCC_APB1PeriphClockCmd

#define  Schedule_TIM_IRQ 			TIM7_IRQn
#define  Schedule_TIM_IRQHandler	TIM7_IRQHandler

/**  蜂鸣器PWM输出定时器——高级定时器TIM1  **/
#define Beep_TIMx					TIM3
#define	Beep_TIM_CLK				RCC_APB1Periph_TIM3
#define	Beep_TIM_ClkCmd				RCC_APB1PeriphClockCmd

#define	BeepOCInit					TIM_OC3Init
#define Beep_Channel				TIM_Channel_3
#define Beep_OCPreloadConfig		TIM_OC3PreloadConfig

// TIM GPIO 引脚宏定义
#define  Beep_GPIO_CLK           	(RCC_AHB1Periph_GPIOB)
#define  Beep_GPIO_ClkCmd    		RCC_AHB1PeriphClockCmd
    
#define  Beep_GPIO_PORT        		GPIOB
#define  Beep_GPIO_PIN         		GPIO_Pin_0
#define  Beep_GPIO_AF         		GPIO_AF_TIM3
#define  Beep_GPIO_PinSource  		GPIO_PinSource0


/**  模拟看门狗——基本定时器TIM6  **/
#define  WatchDog_TIMx				TIM6
#define  WatchDog_TIM_CLK			RCC_APB1Periph_TIM6
#define  WatchDog_TIM_ClkCmd		RCC_APB1PeriphClockCmd

#define  WatchDog_TIM_IRQ 			TIM6_DAC_IRQn
#define  WatchDog_TIM_IRQHandler	TIM6_DAC_IRQHandler

void ScheduleTIM_Config(void);
void Beep_TIM_Config(void);
void WatchDogTIM_Config(void);

#endif
