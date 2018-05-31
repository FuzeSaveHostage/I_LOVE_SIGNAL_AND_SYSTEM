#ifndef __CAN__H
#define __CAN__H

#include "stm32f10x_can.h"
#include "stm32f10x.h"

#include "delay.h"

#define CAN_ID1 0X01
#define CAN_ID2 0X00

#define CAN_FILTER1 0X0000//0XFFE0
#define CAN_FILTER2 0X0000//0XFFE0

void CAN_Config(void);
uint8_t CAN_SendMassage(uint16_t ID, uint8_t dataLength, uint8_t *canSendMassage);
static void Delay(__IO uint32_t nCount);


#endif

