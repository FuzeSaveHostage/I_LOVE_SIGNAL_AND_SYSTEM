#include "limitswitch.h"

__LIMITSWITCH_REG_t lswLeft, lswRight;

void LIMITSWITCH_Init()
{
	GPIO_InitTypeDef GPIOInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOInitStruct.GPIO_Pin = SWITCH_PIN_NUM_1;

	GPIO_Init(SWITCH_PIN_PORT_1, &GPIOInitStruct);
}

void LIMITSWITCH_Registry()
{
	lswLeft.pRead = &LSW_Left;
	lswRight.pRead = &LSW_Right;
}
