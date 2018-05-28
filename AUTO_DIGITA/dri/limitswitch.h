#ifndef __LIMITSWITCH__H
#define __LIMITSWITCH__H

#include "stm32f10x.h"
#include "iobit.h"
#include "delay.h"

#define SWITCH_PIN_NUM_1 GPIO_Pin_10
#define SWITCH_PIN_PORT_1 GPIOD

#define SWITCH_PIN_NUM_2 GPIO_Pin_11
#define SWITCH_PIN_PORT_2 GPIOD

#define SWITCH_OFF 0
#define SWITCH_ON 1

//************限位开关分组宏定义*************//
#define SW_LEFT 0
#define SW_RIGHT 1
//*******************************************//

typedef struct
{
	uint8_t (*pRead)(void);
}__LIMITSWITCH_REG_t;

extern __LIMITSWITCH_REG_t lswLeft, lswRight;

void LIMITSWITCH_Init(void);
//uint8_t LIMITSWITCH_WaitChange(__LIMITSWITCH_REG_t *sw, uint32_t waitMs);
void LIMITSWITCH_Registry(void);

static inline uint8_t LSW_Left(void)
{
	return PDin(10);
}

static inline uint8_t LSW_Right(void)
{
	return PDin(11);
}

#endif
