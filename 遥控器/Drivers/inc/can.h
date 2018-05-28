#ifndef _CAN_H_
#define _CAN_H_

#include "STM32F4xx.h"

#define CANx                    CAN2
#define CAN_CLK					RCC_APB1Periph_CAN2

#define CAN_RX_IRQ				CAN2_RX1_IRQn
#define CAN_RX_IRQHandler		CAN2_RX1_IRQHandler

#define CAN_TX_IRQ				CAN2_TX_IRQn
#define CAN_TX_IRQHandler		CAN2_TX_IRQHandler

#define CAN_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define CAN_AF_PORT             GPIO_AF_CAN2

#define	CAN_TX_GPIO_PORT  		GPIOB
#define CAN_TX_GPIO_PIN   		GPIO_Pin_13
#define CAN_TX_GPIO_PinSourcex	GPIO_PinSource13

#define	CAN_RX_GPIO_PORT     	GPIOB
#define	CAN_RX_GPIO_PIN      	GPIO_Pin_12
#define CAN_RX_GPIO_PinSourcex	GPIO_PinSource12

extern CanRxMsg  CAN_Rece_Data;
extern uint8_t CAN_TX_Success_flag;

void CanInit(void);
void Init_RxMes(CanRxMsg *RxMessage);
//void CAN_SendMsg(uint32_t StdID, EquipsCtrlTypeDef* TxMsg);

#endif

