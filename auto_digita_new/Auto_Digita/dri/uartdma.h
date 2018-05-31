#ifndef __UARTDMA__H
#define __UARTDMA__H

#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"

#include "inter2MasCon.h"

#define  CTRL_USARTx                   	USART2
#define  CTRL_USART_CLK                	RCC_APB1Periph_USART2
#define  CTRL_USART_APBxClkCmd         	RCC_APB1PeriphClockCmd
#define  CTRL_USART_BAUDRATE           	115200

#define  CTRL_USART_GPIO_CLK           	(RCC_APB2Periph_GPIOA)
#define  CTRL_USART_GPIO_APBxClkCmd    	RCC_APB2PeriphClockCmd
    
#define  CTRL_USART_TX_GPIO_PORT        GPIOA   
#define  CTRL_USART_TX_GPIO_PIN         GPIO_Pin_2
#define  CTRL_USART_RX_GPIO_PORT       	GPIOA
#define  CTRL_USART_RX_GPIO_PIN        	GPIO_Pin_3

#define  CTRL_USART_DMAx                DMA1
#define  CTRL_USART_DMA_CLK            	RCC_AHBPeriph_DMA1
#define  CTRL_USART_DMAClkCmd         	RCC_AHBPeriphClockCmd

#define  CTRL_UART_DR_Base             	(USART2_BASE + 0x04)

#define  CTRL_UART_Rx_Memory_Base      	(uint32_t)(CMDBuffer)
#define  CTRL_UART_Rx_DMA_BufferSize	sizeof(CMDBuffer[0])
	
#define  CTRL_UART_Tx_Memory_Base      	(uint32_t)(&Tx_Buffer)
#define  CTRL_UART_Tx_DMA_BufferSize	sizeof(Tx_Buffer.TxBuffer)

#define  CTRL_UART_Tx_DMA_Channel      	DMA1_Channel7
#define  CTRL_UART_Tx_DMA_ChannelNum    7
#define  CTRL_UART_Tx_DMA_TC_FLAG       DMA1_FLAG_TC7
#define  CTRL_UART_Tx_DMA_GL_FLAG       DMA1_FLAG_GL7
#define  CTRL_UART_Tx_DMA_HT_FLAG       DMA1_FLAG_HT7
#define  CTRL_UART_Tx_DMA_IRQ          	DMA1_Channel7_IRQn
#define  CTRL_USART_Tx_DMA_IRQHandler   DMA1_Channel7_IRQHandler

#define  CTRL_UART_Rx_DMA_Channel      	DMA1_Channel6
#define  CTRL_UART_Rx_DMA_ChannelNum    6
#define  CTRL_UART_Rx_DMA_TC_FLAG       DMA1_FLAG_TC6
#define  CTRL_UART_Rx_DMA_GL_FLAG       DMA1_FLAG_GL6
#define  CTRL_UART_Rx_DMA_HT_FLAG       DMA1_FLAG_HT6
#define  CTRL_UART_Rx_DMA_IRQ      		DMA1_Channel6_IRQn
#define  CTRL_USART_Rx_DMA_IRQHandler   DMA1_Channel6_IRQHandler

void UART_LASER_Config(void);
void UART_MASTER_Config(void);
void DMA_TX_Config(void);
void DMA_RX_Config(uint32_t RxBase, uint32_t HeadCorrectValue);
void SendMsg_byDMA(void);
void DMA_ReceiveSet_Change(uint32_t RxBase, uint32_t HeadCorrectValue);
void UART_DMA_Config(void);

#endif
