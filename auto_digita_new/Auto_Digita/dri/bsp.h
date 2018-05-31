#ifndef __BSP__H
#define __BSP__H

#include "stm32f10x.h"

#include "can.h"
#include "delay.h"
#include "led.h"
#include "iobit.h"
#include "cylinder.h"
#include "limitswitch.h"
#include "uartdma.h"
#include "lightbeep.h"
#include "tim.h"

extern uint8_t fSTART;

void SysInit(void);
void NVIC_Config(void);
uint8_t RCC_HSE_Configuration(void);
void AllThreadStart(void);


#endif
