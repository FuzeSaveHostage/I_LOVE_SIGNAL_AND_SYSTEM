#include "led.h"

void LED_Init()
{
	GPIO_InitTypeDef GPIOInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIOInitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	LED_State(7);
}

/**
 * [LED_State description]
 * @param state 3bit二进制数，2bit--R 1bit--G 0bit--B
 *              	0开 1关
 */
void LED_State(uint32_t state)
{	
	LED_R_STATE = ((state&4) >> 2);
	LED_G_STATE = ((state&2) >> 1);
	LED_B_STATE = state&1;
}
