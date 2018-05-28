#include "Cylinder.h"
#include "ActionCtrl.h"

/**
* @brief  气缸初始化
* @param  无
* @retval 无
*/
void CylinderInit(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	// 打开GPIO的时钟
	RCC_AHB1PeriphClockCmd(	Left_ClawCylinder_Periph | Middle_ClawCylinder_Periph | Right_ClawCylinder_Periph
							| 	Left_PushCylinder_Periph | Middle_PushCylinder_Periph | Right_PushCylinder_Periph, ENABLE);
	
	//引脚参数配置
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;				//设置GPIO模式为输出
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;			//设置为既不上拉也不下拉
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;			//输出速度
	
	//选定引脚
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;				//设置输出模式为推挽
	
	GPIOInitStruct.GPIO_Pin = Left_ClawCylinder_PIN;
	GPIO_Init(Left_ClawCylinder_PORT, &GPIOInitStruct);	
	
	GPIOInitStruct.GPIO_Pin = Left_PushCylinder_PIN;
	GPIO_Init(Left_PushCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = Middle_ClawCylinder_PIN;
	GPIO_Init(Middle_ClawCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = Middle_PushCylinder_PIN;
	GPIO_Init(Middle_PushCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;				//设置输出模式为开漏
	
	GPIOInitStruct.GPIO_Pin = Right_ClawCylinder_PIN;
	GPIO_Init(Right_ClawCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = Right_PushCylinder_PIN;
	GPIO_Init(Right_PushCylinder_PORT, &GPIOInitStruct);
	
	//指定气缸初始状态
	Left_ClawCylinderIO = IN;
	Left_PushCylinderIO = IN;
	Middle_ClawCylinderIO = IN;
	Middle_PushCylinderIO = IN;
	Right_ClawCylinderIO = OUT;
	Right_PushCylinderIO = OUT;
}



/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

