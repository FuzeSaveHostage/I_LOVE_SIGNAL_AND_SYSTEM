#ifndef __LED__H
#define __LED__H

#include "stm32f10x.h"
#include "iobit.h"

#define LED_R_STATE PEout(11)
#define LED_G_STATE PEout(12)
#define LED_B_STATE PEout(10)

#define LED_ON 0
#define LED_OFF 1

void SysInit(void);
void LED_Init(void);
void LED_State(uint32_t state);

#endif
