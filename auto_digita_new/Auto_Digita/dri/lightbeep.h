#ifndef __LIGHTBEEP__H
#define __LIGHTBEEP__H

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "delay.h"

typedef struct 
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t bright;
}__LIGHT_BAR_PARA_t;

extern __LIGHT_BAR_PARA_t lightBarPara;

void TIM_LIGHTBEEP_Config(void);
void LightAct(const __LIGHT_BAR_PARA_t* para);

static inline void BEEP_On(uint16_t fre)
{
	TIM4->ARR = fre;
	TIM4->CCR4 = 400;
}

static inline void BEEP_Off(void)
{
	TIM4->CCR4 = 0;
}

void BEEP_InitSucceed(void);
void BEEP_ConnectBreak(void);

#endif
