#include "cylinder.h"

__CYLINDER_REG_t cylRegistry[10];

void CYLINDER_Init()
{
	GPIO_InitTypeDef GPIOInitStruct;

	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	//电磁阀1
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀2
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀3
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀4
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀5
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀6
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀7
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀8
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀9
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	//电磁阀10
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	CYLINDER_State(0);
}

void CYLINDER_Registry()
{
	cylRegistry[0].pSwitch = &CYL1;
	cylRegistry[0].group = CYL_GROUP_LEFT;
	cylRegistry[0].type = CYL_TYPE_CLAW;

	cylRegistry[1].pSwitch = &CYL2;
	cylRegistry[1].group = CYL_GROUP_RIGHT;
	cylRegistry[1].type = CYL_TYPE_CLAW;

	cylRegistry[2].pSwitch = &CYL3;
	cylRegistry[2].group = CYL_GROUP_LEFT;
	cylRegistry[2].type = CYL_TYPE_LINE;

	cylRegistry[3].pSwitch = &CYL4;
	cylRegistry[3].group = CYL_GROUP_RIGHT;
	cylRegistry[3].type = CYL_TYPE_LINE;

	cylRegistry[4].pSwitch = &CYL5;
	cylRegistry[5].pSwitch = &CYL6;
	cylRegistry[6].pSwitch = &CYL7;
	cylRegistry[7].pSwitch = &CYL8;
	cylRegistry[8].pSwitch = &CYL9;
	cylRegistry[9].pSwitch = &CYL10;
}


/**
 * @param 9bit~0bit 对应 10号到1号气缸
 */
void CYLINDER_State(uint16_t state)
{
	CYL_STATE_10 = (state&512) >> 9;
	CYL_STATE_9 = (state&256) >> 8;
	CYL_STATE_8 = (state&128) >> 7;
	CYL_STATE_7 = (state&64) >> 6;
	CYL_STATE_6 = (state&32) >> 5;
	CYL_STATE_5 = (state&16) >> 4;
	CYL_STATE_4 = (state&8) >> 3;
	CYL_STATE_3 = (state&4) >> 2;
	CYL_STATE_2 = (state&2) >> 1;
	CYL_STATE_1 = (state&1);
}
