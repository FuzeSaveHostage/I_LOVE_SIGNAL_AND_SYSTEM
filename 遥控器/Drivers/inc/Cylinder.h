#ifndef _CYLINDER_H_
#define _CYLINDER_H_

#include "IO_act.h"

/** @defgroup 气缸IO定义
  * @{
  */
  
#define Left_ClawCylinder_PORT			GPIOA					//左气爪
#define Left_ClawCylinder_Periph		RCC_AHB1Periph_GPIOA
#define Left_ClawCylinder_PIN 			GPIO_Pin_6
#define Left_ClawCylinderIO 			PAout(6)

#define Left_PushCylinder_PORT			GPIOC					//左推拉
#define Left_PushCylinder_Periph		RCC_AHB1Periph_GPIOC
#define Left_PushCylinder_PIN 			GPIO_Pin_5
#define Left_PushCylinderIO 			PCout(5)

#define Middle_PushCylinder_PORT		GPIOC					//中推拉
#define Middle_PushCylinder_Periph		RCC_AHB1Periph_GPIOC
#define Middle_PushCylinder_PIN 		GPIO_Pin_4
#define Middle_PushCylinderIO			PCout(4)

#define Middle_ClawCylinder_PORT		GPIOA					//中气爪
#define Middle_ClawCylinder_Periph		RCC_AHB1Periph_GPIOA
#define Middle_ClawCylinder_PIN 		GPIO_Pin_5
#define Middle_ClawCylinderIO			PAout(5)

#define Right_ClawCylinder_PORT			GPIOE					//右气爪
#define Right_ClawCylinder_Periph		RCC_AHB1Periph_GPIOE
#define Right_ClawCylinder_PIN 			GPIO_Pin_15
#define Right_ClawCylinderIO			PEout(15)

#define Right_PushCylinder_PORT			GPIOA					//右推拉
#define Right_PushCylinder_Periph		RCC_AHB1Periph_GPIOA
#define Right_PushCylinder_PIN 			GPIO_Pin_7
#define Right_PushCylinderIO			PAout(7)

/**
  * @}
  */

enum CylinderStateType{OUT = 1, IN = 0};	//标识气缸的推出/收回两种状态

void CylinderInit(void);

#endif

