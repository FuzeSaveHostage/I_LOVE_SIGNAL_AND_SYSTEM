#include "uartdma.h"

void UART_LASER_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/*                  串口1初始化(激光1)                      */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//串口1初始化设置
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

	/*                  串口3初始化(激光2)                      */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//串口3初始化设置
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART3,&USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);

	/*                  串口4初始化(激光3)                      */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//串口4初始化设置
	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_Init(UART4,&USART_InitStructure);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART4, ENABLE);
}

void UART_MASTER_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//配置串口GPIO
	GPIO_InitStructure.GPIO_Pin = CTRL_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CTRL_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = CTRL_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CTRL_USART_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = CTRL_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(CTRL_USARTx, &USART_InitStructure);
	USART_Cmd(CTRL_USARTx, ENABLE);	 
}

void DMA_TX_Config()
{
	DMA_InitTypeDef  DMA_InitStructre;

	DMA_DeInit(CTRL_UART_Tx_DMA_Channel); 

	//配置内存缓冲区地址
	DMA_InitStructre.DMA_MemoryBaseAddr = CTRL_UART_Tx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = CTRL_UART_Tx_DMA_BufferSize;


	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = CTRL_UART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	//配置内存地址指向的数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralDST;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	//非循环模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;

	//不允许内存数据相互访问（这个成员其实不用配置，因为该通道并非用于两段内存间的数据传递）
	DMA_InitStructre.DMA_M2M = DMA_M2M_Disable;

	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;

	DMA_Init(CTRL_UART_Tx_DMA_Channel, &DMA_InitStructre);

	//清除DMA数据流传输完成标志位
	DMA_ClearFlag(CTRL_UART_Tx_DMA_TC_FLAG | CTRL_UART_Tx_DMA_GL_FLAG);

	//使能传输完成中断
	DMA_ITConfig(CTRL_UART_Tx_DMA_Channel,DMA_IT_TC,ENABLE); 

	//先不使能接收DMA通道，等到要发送的时候再使能，不然的话会一直发
	DMA_Cmd(CTRL_UART_Tx_DMA_Channel, DISABLE); 

	//采用DMA方式发送  
	USART_DMACmd(CTRL_USARTx,USART_DMAReq_Tx,ENABLE); 
}

void DMA_RX_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位接收DMA通道
    DMA_DeInit(CTRL_UART_Rx_DMA_Channel); 
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = CTRL_UART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_MemoryBaseAddr = RxBase;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	
	//传递的缓冲区长度
	DMA_InitStructre.DMA_BufferSize = HeadCorrectValue;
	
	//配置外设地址指向的数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralSRC;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//非循环模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
//	DMA_InitStructre.DMA_Mode  = DMA_Mode_Circular;
	
	//不允许内存数据相互访问（这个成员其实不用配置，因为该通道并非用于两段内存间的数据传递）
	DMA_InitStructre.DMA_M2M = DMA_M2M_Disable;
	
	//设置优先级为非常高
	DMA_InitStructre.DMA_Priority = DMA_Priority_VeryHigh;
	
	DMA_Init(CTRL_UART_Rx_DMA_Channel, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(CTRL_UART_Rx_DMA_TC_FLAG | CTRL_UART_Rx_DMA_GL_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(CTRL_UART_Rx_DMA_Channel,DMA_IT_TC,ENABLE); 
	
	//使能接收DMA通道
	DMA_Cmd(CTRL_UART_Rx_DMA_Channel, ENABLE); 
	
    //采用DMA方式接收  
    USART_DMACmd(CTRL_USARTx,USART_DMAReq_Rx,ENABLE);	
}

void SendMsg_byDMA(void)
{
	DMA_Cmd(CTRL_UART_Tx_DMA_Channel, DISABLE );  	//关闭CTRL_UART_Tx_DMA_Channel 所指示的通道        
    DMA_SetCurrDataCounter(CTRL_UART_Tx_DMA_Channel, CTRL_UART_Tx_DMA_BufferSize);//DMA通道的DMA缓存的大小  
    DMA_Cmd(CTRL_UART_Tx_DMA_Channel, ENABLE);  	//使能CTRL_UART_Tx_DMA_Channel 所指示的通道   
}

void DMA_ReceiveSet_Change(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	*(uint32_t*)(DMA1_BASE + 0x14 + 20*(CTRL_UART_Rx_DMA_ChannelNum - 1)) =  RxBase;
	
	*(uint32_t*)(DMA1_BASE + 0x0C + 20*(CTRL_UART_Rx_DMA_ChannelNum - 1)) |=  HeadCorrectValue % 0x10000;
}

void UART_DMA_Config()
{
	// 打开串口GPIO的时钟
	CTRL_USART_GPIO_APBxClkCmd(CTRL_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	CTRL_USART_APBxClkCmd(CTRL_USART_CLK, ENABLE);
	
	//启动DMA时钟  
    CTRL_USART_DMAClkCmd(CTRL_USART_DMA_CLK, ENABLE); 
	
	//初始化发送DMA通道
	DMA_TX_Config();
	
	//初始化接收DMA通道
	DMA_RX_Config((uint32_t)(&(CMDBuffer[0])), DataHeadCorrect);
	
	//初始化串口和GPIO参数
	UART_MASTER_Config();
}
