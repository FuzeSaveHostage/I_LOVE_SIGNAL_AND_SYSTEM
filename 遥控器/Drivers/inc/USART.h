#ifndef _USART_H_
#define _USART_H_

#include "InterfaceToMicroPC.h"
#include "InterfaceToEquips.h"
#include "InterfaceToMaster.h"
#include "InterfaceToF103.h"
#include "STM32F4xx.h"
#include <stdio.h>
	
// 针对F103的控制端口
#define  CTRL_USARTx                   	USART2
#define  CTRL_USART_CLK                	RCC_APB1Periph_USART2
#define  CTRL_USART_ClkCmd         		RCC_APB1PeriphClockCmd
#define  CTRL_USART_BAUDRATE           	115200

// USART GPIO 引脚宏定义
#define  CTRL_USART_GPIO_CLK           (RCC_AHB1Periph_GPIOA)
#define  CTRL_USART_GPIO_ClkCmd    		RCC_AHB1PeriphClockCmd
    
#define  CTRL_USART_TX_GPIO_PORT        GPIOA   
#define  CTRL_USART_TX_GPIO_PIN         GPIO_Pin_2
#define  CTRL_USART_TX_GPIO_AF         	GPIO_AF_USART2
#define  CTRL_USART_TX_GPIO_PinSource  	GPIO_PinSource2

#define  CTRL_USART_RX_GPIO_PORT       	GPIOA
#define  CTRL_USART_RX_GPIO_PIN        	GPIO_Pin_3
#define  CTRL_USART_RX_GPIO_AF         	GPIO_AF_USART2
#define  CTRL_USART_RX_GPIO_PinSource  	GPIO_PinSource3

#define  CTRL_USART_IRQ                	USART2_IRQn
#define  CTRL_USART_IRQHandler         	USART2_IRQHandler

#define  CTRL_USART_DMAx                DMA1
#define  CTRL_USART_DMAx_BASE           DMA1_BASE
#define  CTRL_USART_DMA_CLK            	RCC_AHB1Periph_DMA1
#define  CTRL_USART_DMAClkCmd         	RCC_AHB1PeriphClockCmd

#define  CTRL_USART_DR_Base             (USART2_BASE + 0x04)

#define  CTRL_USART_Rx_Memory_Base      (uint32_t)(F1_RxBuffer)
#define  CTRL_USART_Rx_DMA_BufferSize	sizeof(F1_RxBuffer[0])
	
#define  CTRL_USART_Tx_Memory_Base      (uint32_t)(&F1_Tx_Buffer)
#define  CTRL_USART_Tx_DMA_BufferSize	sizeof(F1_Tx_Buffer.TxBuffer)

#define  CTRL_USART_Tx_DMA_Stream      	DMA1_Stream6
#define  CTRL_USART_Tx_DMA_Channel      DMA_Channel_4
#define  CTRL_USART_Tx_DMA_StreamNum    6
#define  CTRL_USART_Tx_DMA_TC_FLAG      DMA_FLAG_TCIF6
#define  CTRL_USART_Tx_DMA_HT_FLAG      DMA_FLAG_HTIF6
#define  CTRL_USART_Tx_DMA_IRQ          DMA1_Stream6_IRQn
#define  CTRL_USART_Tx_DMA_IRQHandler   DMA1_Stream6_IRQHandler

#define  CTRL_USART_Rx_DMA_Stream      	DMA1_Stream5
#define  CTRL_USART_Rx_DMA_Channel      DMA_Channel_4
#define  CTRL_USART_Rx_DMA_StreamNum    5
#define  CTRL_USART_Rx_DMA_TC_FLAG      DMA_FLAG_TCIF5
#define  CTRL_USART_Rx_DMA_HT_FLAG      DMA_FLAG_HTIF5
#define  CTRL_USART_Rx_DMA_IRQ      	DMA1_Stream5_IRQn
#define  CTRL_USART_Rx_DMA_IRQHandler   DMA1_Stream5_IRQHandler

// 针对辅控的控制端口
#define  Equips_USARTx                  	UART4
#define  Equips_USART_CLK               	RCC_APB1Periph_UART4
#define  Equips_USART_ClkCmd         		RCC_APB1PeriphClockCmd
#define  Equips_USART_BAUDRATE           	115200

// USART GPIO 引脚宏定义
#define  Equips_USART_GPIO_CLK          	(RCC_AHB1Periph_GPIOC)
#define  Equips_USART_GPIO_ClkCmd    		RCC_AHB1PeriphClockCmd
    
#define  Equips_USART_TX_GPIO_PORT			GPIOC  
#define  Equips_USART_TX_GPIO_PIN        	GPIO_Pin_10
#define  Equips_USART_TX_GPIO_AF         	GPIO_AF_UART4
#define  Equips_USART_TX_GPIO_PinSource  	GPIO_PinSource10

#define  Equips_USART_RX_GPIO_PORT       	GPIOC
#define  Equips_USART_RX_GPIO_PIN        	GPIO_Pin_11
#define  Equips_USART_RX_GPIO_AF         	GPIO_AF_UART4
#define  Equips_USART_RX_GPIO_PinSource  	GPIO_PinSource11

#define  Equips_USART_IRQ                	UART4_IRQn
#define  Equips_USART_IRQHandler         	UART4_IRQHandler

#define  Equips_USART_DMAx                	DMA1
#define  Equips_USART_DMAx_BASE           	DMA1_BASE
#define  Equips_USART_DMA_CLK            	RCC_AHB1Periph_DMA1
#define  Equips_USART_DMAClkCmd         	RCC_AHB1PeriphClockCmd

#define  Equips_USART_DR_Base             	(UART4_BASE + 0x04)

#define  Equips_USART_Rx_Memory_Base      	(uint32_t)(Eq_RxBuffer)
#define  Equips_USART_Rx_DMA_BufferSize		sizeof(Eq_RxBuffer[0])
	
#define  Equips_USART_Tx_Memory_Base      	(uint32_t)(&Eq_Tx_Buffer)
#define  Equips_USART_Tx_DMA_BufferSize		sizeof(Eq_Tx_Buffer.TxBuffer)

#define  Equips_USART_Tx_DMA_Stream      	DMA1_Stream4
#define  Equips_USART_Tx_DMA_Channel      	DMA_Channel_4
#define  Equips_USART_Tx_DMA_StreamNum    	4
#define  Equips_USART_Tx_DMA_TC_FLAG      	DMA_FLAG_TCIF4
#define  Equips_USART_Tx_DMA_HT_FLAG      	DMA_FLAG_HTIF4
#define  Equips_USART_Tx_DMA_IRQ          	DMA1_Stream4_IRQn
#define  Equips_USART_Tx_DMA_IRQHandler   	DMA1_Stream4_IRQHandler

#define  Equips_USART_Rx_DMA_Stream      	DMA1_Stream2
#define  Equips_USART_Rx_DMA_Channel      	DMA_Channel_4
#define  Equips_USART_Rx_DMA_StreamNum    	2
#define  Equips_USART_Rx_DMA_TC_FLAG      	DMA_FLAG_TCIF2
#define  Equips_USART_Rx_DMA_HT_FLAG      	DMA_FLAG_HTIF2
#define  Equips_USART_Rx_DMA_IRQ      		DMA1_Stream2_IRQn
#define  Equips_USART_Rx_DMA_IRQHandler   	DMA1_Stream2_IRQHandler

//// 针对小电脑的控制端口
//#define  PC_USARTx                   	UART8
//#define  PC_USART_CLK                	RCC_APB1Periph_UART8
//#define  PC_USART_ClkCmd         		RCC_APB1PeriphClockCmd
//#define  PC_USART_BAUDRATE           	115200

//// USART GPIO 引脚宏定义
//#define  PC_USART_GPIO_CLK			(RCC_AHB1Periph_GPIOE)
//#define  PC_USART_GPIO_ClkCmd    		RCC_AHB1PeriphClockCmd
//    
//#define  PC_USART_TX_GPIO_PORT			GPIOE
//#define  PC_USART_TX_GPIO_PIN			GPIO_Pin_1
//#define  PC_USART_TX_GPIO_AF			GPIO_AF_UART8
//#define  PC_USART_TX_GPIO_PinSource		GPIO_PinSource1

//#define  PC_USART_RX_GPIO_PORT			GPIOE
//#define  PC_USART_RX_GPIO_PIN			GPIO_Pin_0
//#define  PC_USART_RX_GPIO_AF			GPIO_AF_UART8
//#define  PC_USART_RX_GPIO_PinSource		GPIO_PinSource0

//#define  PC_USART_IRQ                	UART8_IRQn
//#define  PC_USART_IRQHandler         	UART8_IRQHandler

//#define  PC_USART_DMAx					DMA1
//#define  PC_USART_DMAx_BASE				DMA1_BASE
//#define  PC_USART_DMA_CLK				RCC_AHB1Periph_DMA1
//#define  PC_USART_DMAClkCmd				RCC_AHB1PeriphClockCmd

//#define  PC_USART_DR_Base				(UART8_BASE + 0x04)

//#define  PC_USART_Rx_Memory_Base		(uint32_t)(PC_RxBuffer)
//#define  PC_USART_Rx_DMA_BufferSize		sizeof(PC_RxBuffer[0])
//	
//#define  PC_USART_Tx_Memory_Base		(uint32_t)(&PC_Tx_Buffer)
//#define  PC_USART_Tx_DMA_BufferSize		sizeof(PC_Tx_Buffer.TxBuffer)

//#define  PC_USART_Tx_DMA_Stream			DMA1_Stream0
//#define  PC_USART_Tx_DMA_Channel		DMA_Channel_5
//#define  PC_USART_Tx_DMA_StreamNum		0
//#define  PC_USART_Tx_DMA_TC_FLAG		DMA_FLAG_TCIF0
//#define  PC_USART_Tx_DMA_HT_FLAG		DMA_FLAG_HTIF0
//#define  PC_USART_Tx_DMA_IRQ			DMA1_Stream0_IRQn
//#define  PC_USART_Tx_DMA_IRQHandler		DMA1_Stream0_IRQHandler

//#define  PC_USART_Rx_DMA_Stream			DMA1_Stream6
//#define  PC_USART_Rx_DMA_Channel		DMA_Channel_5
//#define  PC_USART_Rx_DMA_StreamNum		6
//#define  PC_USART_Rx_DMA_TC_FLAG		DMA_FLAG_TCIF6
//#define  PC_USART_Rx_DMA_HT_FLAG		DMA_FLAG_HTIF6
//#define  PC_USART_Rx_DMA_IRQ			DMA1_Stream6_IRQn
//#define  PC_USART_Rx_DMA_IRQHandler		DMA1_Stream6_IRQHandler


// 针对小电脑的控制端口
#define  PC_USARTx                   	USART1
#define  PC_USART_CLK                	RCC_APB2Periph_USART1
#define  PC_USART_ClkCmd         		RCC_APB2PeriphClockCmd
#define  PC_USART_BAUDRATE           	115200

// USART GPIO 引脚宏定义
#define  PC_USART_GPIO_CLK           	(RCC_AHB1Periph_GPIOA)
#define  PC_USART_GPIO_ClkCmd    		RCC_AHB1PeriphClockCmd
    
#define  PC_USART_TX_GPIO_PORT        	GPIOA   
#define  PC_USART_TX_GPIO_PIN         	GPIO_Pin_9
#define  PC_USART_TX_GPIO_AF         	GPIO_AF_USART1
#define  PC_USART_TX_GPIO_PinSource  	GPIO_PinSource9

#define  PC_USART_RX_GPIO_PORT       	GPIOA
#define  PC_USART_RX_GPIO_PIN        	GPIO_Pin_10
#define  PC_USART_RX_GPIO_AF         	GPIO_AF_USART1
#define  PC_USART_RX_GPIO_PinSource  	GPIO_PinSource10

#define  PC_USART_IRQ                	USART1_IRQn
#define  PC_USART_IRQHandler         	USART1_IRQHandler


#define  PC_USART_DMAx                	DMA2
#define  PC_USART_DMAx_BASE           	DMA2_BASE
#define  PC_USART_DMA_CLK            	RCC_AHB1Periph_DMA2
#define  PC_USART_DMAClkCmd         	RCC_AHB1PeriphClockCmd

#define  PC_USART_DR_Base             	(USART1_BASE + 0x04)

#define  PC_USART_Rx_Memory_Base      	(uint32_t)(PC_RxBuffer)
#define  PC_USART_Rx_DMA_BufferSize		sizeof(PC_RxBuffer[0])
	
#define  PC_USART_Tx_Memory_Base      	(uint32_t)(&PC_Tx_Buffer)
#define  PC_USART_Tx_DMA_BufferSize		sizeof(PC_Tx_Buffer.TxBuffer)

#define  PC_USART_Tx_DMA_Stream      	DMA2_Stream7
#define  PC_USART_Tx_DMA_Channel      	DMA_Channel_4
#define  PC_USART_Tx_DMA_StreamNum    	7
#define  PC_USART_Tx_DMA_TC_FLAG       	DMA_FLAG_TCIF7
#define  PC_USART_Tx_DMA_HT_FLAG       	DMA_FLAG_HTIF7
#define  PC_USART_Tx_DMA_IRQ          	DMA2_Stream7_IRQn
#define  PC_USART_Tx_DMA_IRQHandler   	DMA2_Stream7_IRQHandler

#define  PC_USART_Rx_DMA_Stream      	DMA2_Stream5
#define  PC_USART_Rx_DMA_Channel      	DMA_Channel_4
#define  PC_USART_Rx_DMA_StreamNum    	5
#define  PC_USART_Rx_DMA_TC_FLAG       	DMA_FLAG_TCIF5
#define  PC_USART_Rx_DMA_HT_FLAG       	DMA_FLAG_TCIF5
#define  PC_USART_Rx_DMA_IRQ      		DMA2_Stream5_IRQn
#define  PC_USART_Rx_DMA_IRQHandler   	DMA2_Stream5_IRQHandler

// 针对蓝牙的控制端口
#define  BL_USARTx                   	USART3
#define  BL_USART_CLK                	RCC_APB1Periph_USART3
#define  BL_USART_ClkCmd         		RCC_APB1PeriphClockCmd
#define  BL_USART_BAUDRATE           	9600

// USART GPIO 引脚宏定义
#define  BL_USART_GPIO_CLK				(RCC_AHB1Periph_GPIOB)
#define  BL_USART_GPIO_ClkCmd    		RCC_AHB1PeriphClockCmd
    
#define  BL_USART_TX_GPIO_PORT			GPIOB
#define  BL_USART_TX_GPIO_PIN			GPIO_Pin_10
#define  BL_USART_TX_GPIO_AF			GPIO_AF_USART3
#define  BL_USART_TX_GPIO_PinSource		GPIO_PinSource10

#define  BL_USART_RX_GPIO_PORT			GPIOB
#define  BL_USART_RX_GPIO_PIN			GPIO_Pin_11
#define  BL_USART_RX_GPIO_AF			GPIO_AF_USART3
#define  BL_USART_RX_GPIO_PinSource		GPIO_PinSource11

#define  BL_USART_IRQ                	USART3_IRQn
#define  BL_USART_IRQHandler         	USART3_IRQHandler

#define  BL_USART_DMAx					DMA1
#define  BL_USART_DMAx_BASE				DMA1_BASE
#define  BL_USART_DMA_CLK				RCC_AHB1Periph_DMA1
#define  BL_USART_DMAClkCmd				RCC_AHB1PeriphClockCmd

#define  BL_USART_DR_Base				(USART3_BASE + 0x04)

#define  BL_USART_Rx_Memory_Base		(uint32_t)(CMDBuffer)
#define  BL_USART_Rx_DMA_BufferSize		sizeof(CMDBuffer[0])
	
#define  BL_USART_Tx_Memory_Base		(uint32_t)(&Tx_Buffer)
#define  BL_USART_Tx_DMA_BufferSize		sizeof(Tx_Buffer.TxBuffer)

#define  BL_USART_Tx_DMA_Stream			DMA1_Stream3
#define  BL_USART_Tx_DMA_Channel		DMA_Channel_4
#define  BL_USART_Tx_DMA_StreamNum		3
#define  BL_USART_Tx_DMA_TC_FLAG		DMA_FLAG_TCIF3
#define  BL_USART_Tx_DMA_HT_FLAG		DMA_FLAG_HTIF3
#define  BL_USART_Tx_DMA_IRQ			DMA1_Stream3_IRQn
#define  BL_USART_Tx_DMA_IRQHandler		DMA1_Stream3_IRQHandler

#define  BL_USART_Rx_DMA_Stream			DMA1_Stream1
#define  BL_USART_Rx_DMA_Channel		DMA_Channel_4
#define  BL_USART_Rx_DMA_StreamNum		1
#define  BL_USART_Rx_DMA_TC_FLAG		DMA_FLAG_TCIF1
#define  BL_USART_Rx_DMA_HT_FLAG		DMA_FLAG_TCIF1
#define  BL_USART_Rx_DMA_IRQ			DMA1_Stream1_IRQn
#define  BL_USART_Rx_DMA_IRQHandler		DMA1_Stream1_IRQHandler

// 全局定位的串行接口
#define  MAP_USARTx                   	UART7
#define  MAP_USART_CLK                	RCC_APB1Periph_UART7
#define  MAP_USART_APBxClkCmd         	RCC_APB1PeriphClockCmd
#define  MAP_USART_BAUDRATE           	115200

// MAP USART GPIO 引脚宏定义
#define  MAP_USART_GPIO_CLK           	(RCC_AHB1Periph_GPIOE)
#define  MAP_USART_GPIO_APBxClkCmd    	RCC_AHB1PeriphClockCmd
    
#define  MAP_USART_TX_GPIO_PORT         GPIOE
#define  MAP_USART_TX_GPIO_PIN          GPIO_Pin_8
#define  MAP_USART_TX_GPIO_AF         	GPIO_AF_UART7
#define  MAP_USART_TX_GPIO_PinSource  	GPIO_PinSource8

#define  MAP_USART_RX_GPIO_PORT       	GPIOE
#define  MAP_USART_RX_GPIO_PIN        	GPIO_Pin_7
#define  MAP_USART_RX_GPIO_AF         	GPIO_AF_UART7
#define  MAP_USART_RX_GPIO_PinSource 	GPIO_PinSource7


#define  MAP_USART_IRQ                	UART7_IRQn
#define  MAP_USART_IRQHandler         	UART7_IRQHandler

void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void DMA_ReceiveSet_Change(uint32_t DMAx, uint32_t ChannelNum, uint32_t RxBase, uint32_t HeadCorrectValue);
void SendMsg_byDMA(DMA_Stream_TypeDef * Tx_DMA_Stream, uint32_t BufferSize);



#endif /* __USART_H */
