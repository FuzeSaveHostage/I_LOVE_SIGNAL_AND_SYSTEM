#include "STM32F4xx.h"
#include "USART.h"
#include "Map.h"

 /**
  * @brief  USART 中断向量配置
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	
	/* 配置F103接口USART为中断源 */
	//DMA发送中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = CTRL_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA接收中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = CTRL_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* 配置辅控接口USART为中断源 */
	//DMA发送中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = Equips_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA接收中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = Equips_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* 配置小电脑接口USART为中断源 */
	//DMA发送中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = PC_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA接收中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = PC_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* 配置蓝牙控制USART为中断源 */
	//DMA发送中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = BL_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA接收中断设置  
    NVIC_InitStructure.NVIC_IRQChannel = BL_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* 配置全局定位USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = MAP_USART_IRQ;
	/* 抢占优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 响应优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief 针对F103的USART 发送DMA 配置
  * @param  无
  * @retval 无
  */
static void Ctrl_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(CTRL_USART_Tx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = CTRL_USART_Tx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = CTRL_USART_Tx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = CTRL_USART_Tx_DMA_BufferSize;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = CTRL_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//配置内存地址指向的数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(CTRL_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(CTRL_USART_Tx_DMA_Stream, CTRL_USART_Tx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(CTRL_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//先不使能接收DMA通道，等到要发送的时候再使能，不然的话会一直发
	DMA_Cmd(CTRL_USART_Tx_DMA_Stream, DISABLE); 
	
	//采用DMA方式发送  
    USART_DMACmd(CTRL_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  针对F103的USART 接收DMA 配置
  * @param  RxBase:接收内存缓冲区地址
  * @param  HeadCorrectValue:数据接收校准值
  * @retval 无
  */
static void Ctrl_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(CTRL_USART_Rx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = CTRL_USART_Rx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = CTRL_USART_Rx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = CTRL_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = CTRL_USART_Rx_DMA_BufferSize;
	//配置外设数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(CTRL_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(CTRL_USART_Rx_DMA_Stream, CTRL_USART_Rx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(CTRL_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//使能DMA通道
	DMA_Cmd(CTRL_USART_Rx_DMA_Stream, ENABLE); 
	
	//采用DMA方式接收
    USART_DMACmd(CTRL_USARTx, USART_DMAReq_Rx, ENABLE);  
}

/**
  * @brief  与F103通信的USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
static void Ctrl_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	CTRL_USART_GPIO_ClkCmd(CTRL_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	CTRL_USART_ClkCmd(CTRL_USART_CLK, ENABLE);
	
	//启动DMA时钟  
    CTRL_USART_DMAClkCmd(CTRL_USART_DMA_CLK, ENABLE);
	
	//使能复用，即将GPIO连接到目标外设
	GPIO_PinAFConfig(CTRL_USART_TX_GPIO_PORT,CTRL_USART_TX_GPIO_PinSource,CTRL_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(CTRL_USART_RX_GPIO_PORT,CTRL_USART_RX_GPIO_PinSource,CTRL_USART_RX_GPIO_AF);
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = CTRL_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CTRL_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = CTRL_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(CTRL_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = CTRL_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(CTRL_USARTx, &USART_InitStructure);
	
	//配置发送DMA
	Ctrl_USART_Tx_DMA_Config();
	
	//配置接收DMA
	Ctrl_USART_Rx_DMA_Config((uint32_t)(&(F1_RxBuffer[0])), F1_DataHeadCorrect);
	
	// 使能串口
	USART_Cmd(CTRL_USARTx, ENABLE);	    
}

/**
  * @brief 针对辅控的USART 发送DMA 配置
  * @param  无
  * @retval 无
  */
static void Equips_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(Equips_USART_Tx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = Equips_USART_Tx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = Equips_USART_Tx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = Equips_USART_Tx_DMA_BufferSize;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = Equips_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//配置内存地址指向的数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(Equips_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(Equips_USART_Tx_DMA_Stream, Equips_USART_Tx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(Equips_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//先不使能接收DMA通道，等到要发送的时候再使能，不然的话会一直发
	DMA_Cmd(Equips_USART_Tx_DMA_Stream, DISABLE); 
	
	//采用DMA方式发送  
    USART_DMACmd(Equips_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  针对辅控的USART 接收DMA 配置
  * @param  RxBase:接收内存缓冲区地址
  * @param  HeadCorrectValue:数据接收校准值
  * @retval 无
  */
static void Equips_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(Equips_USART_Rx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = Equips_USART_Rx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = Equips_USART_Rx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = Equips_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = Equips_USART_Rx_DMA_BufferSize;
	//配置外设数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(Equips_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(Equips_USART_Rx_DMA_Stream, Equips_USART_Rx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(Equips_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//使能DMA通道
	DMA_Cmd(Equips_USART_Rx_DMA_Stream, ENABLE); 
	
	//采用DMA方式接收
    USART_DMACmd(Equips_USARTx, USART_DMAReq_Rx, ENABLE);  
}

/**
  * @brief  与辅控通信的USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
static void Equips_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	Equips_USART_GPIO_ClkCmd(Equips_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	Equips_USART_ClkCmd(Equips_USART_CLK, ENABLE);
	
	//启动DMA时钟  
    Equips_USART_DMAClkCmd(Equips_USART_DMA_CLK, ENABLE);
	
	//使能复用，即将GPIO连接到目标外设
	GPIO_PinAFConfig(Equips_USART_TX_GPIO_PORT,Equips_USART_TX_GPIO_PinSource,Equips_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(Equips_USART_RX_GPIO_PORT,Equips_USART_RX_GPIO_PinSource,Equips_USART_RX_GPIO_AF);
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = Equips_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(Equips_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = Equips_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(Equips_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = Equips_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(Equips_USARTx, &USART_InitStructure);
	
	//配置发送DMA
	Equips_USART_Tx_DMA_Config();
	
	//配置接收DMA
	Equips_USART_Rx_DMA_Config((uint32_t)(&(Eq_RxBuffer[0])), Eq_DataHeadCorrect);
	
	// 使能串口
	USART_Cmd(Equips_USARTx, ENABLE);	    
}

/**
  * @brief 针对小电脑的USART 发送DMA 配置
  * @param  无
  * @retval 无
  */
static void PC_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(PC_USART_Tx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = PC_USART_Tx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = PC_USART_Tx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = PC_USART_Tx_DMA_BufferSize;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = PC_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//配置内存地址指向的数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(PC_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(PC_USART_Tx_DMA_Stream, PC_USART_Tx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(PC_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//先不使能接收DMA通道，等到要发送的时候再使能，不然的话会一直发
	DMA_Cmd(PC_USART_Tx_DMA_Stream, DISABLE); 
	
	//采用DMA方式发送  
    USART_DMACmd(PC_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  针对小电脑的USART 接收DMA 配置
  * @param  RxBase:接收内存缓冲区地址
  * @param  HeadCorrectValue:数据接收校准值
  * @retval 无
  */
static void PC_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(PC_USART_Rx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = PC_USART_Rx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = PC_USART_Rx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = PC_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = PC_USART_Rx_DMA_BufferSize;
	//配置外设数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(PC_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(PC_USART_Rx_DMA_Stream, PC_USART_Rx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(PC_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//使能DMA通道
	DMA_Cmd(PC_USART_Rx_DMA_Stream, ENABLE); 
	
	//采用DMA方式接收
    USART_DMACmd(PC_USARTx, USART_DMAReq_Rx, ENABLE);  
}



/**
  * @brief  与小电脑通信的USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
static void PC_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	PC_USART_GPIO_ClkCmd(PC_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	PC_USART_ClkCmd(PC_USART_CLK, ENABLE);
	
	//启动DMA时钟  
    PC_USART_DMAClkCmd(PC_USART_DMA_CLK, ENABLE);
	
	//使能复用，即将GPIO连接到目标外设
	GPIO_PinAFConfig(PC_USART_TX_GPIO_PORT,PC_USART_TX_GPIO_PinSource,PC_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(PC_USART_RX_GPIO_PORT,PC_USART_RX_GPIO_PinSource,PC_USART_RX_GPIO_AF);
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = PC_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PC_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = PC_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(PC_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = PC_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(PC_USARTx, &USART_InitStructure);
	
	//配置发送DMA
	PC_USART_Tx_DMA_Config();
	
	//配置接收DMA
	PC_USART_Rx_DMA_Config((uint32_t)(&(PC_RxBuffer[0])), PC_DataHeadCorrect);
	
	// 使能串口
	USART_Cmd(PC_USARTx, ENABLE);	    
}

/**
  * @brief 针对蓝牙控制端的USART 发送DMA 配置
  * @param  无
  * @retval 无
  */
static void BL_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(BL_USART_Tx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = BL_USART_Tx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = BL_USART_Tx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = BL_USART_Tx_DMA_BufferSize;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = BL_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//配置内存地址指向的数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(BL_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(BL_USART_Tx_DMA_Stream, BL_USART_Tx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(BL_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//先不使能接收DMA通道，等到要发送的时候再使能，不然的话会一直发
	DMA_Cmd(BL_USART_Tx_DMA_Stream, DISABLE); 
	
	//采用DMA方式发送  
    USART_DMACmd(BL_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  针对蓝牙控制端的USART 接收DMA 配置（因为一开始用的是蓝牙所以就缩写为BL）
  * @param  RxBase:接收内存缓冲区地址
  * @param  HeadCorrectValue:数据接收校准值
  * @retval 无
  */
static void BL_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//复位发送DMA通道
    DMA_DeInit(BL_USART_Rx_DMA_Stream); 
	
	//选择DMA通道
	DMA_InitStructre.DMA_Channel = BL_USART_Rx_DMA_Channel;
	
	//配置内存缓冲区地址
	DMA_InitStructre.DMA_Memory0BaseAddr = BL_USART_Rx_Memory_Base;
	//内存数据单元长度为一个字节（8b）
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//使能内存地址递增
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//内存突发传输为单字节
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//配置外设地址
	DMA_InitStructre.DMA_PeripheralBaseAddr = BL_USART_DR_Base;
	//外设地址对应的数据单元长度为一个字节
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//禁止外设地址递增
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//外设突发传输为单字节
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//传递的数据长度
	DMA_InitStructre.DMA_BufferSize = BL_USART_Rx_DMA_BufferSize;
	//配置外设数据为数据源
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//非循环（普通）模式
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//非FIFO模式
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//配置FIFO阈值（因为没有使用FIFO，于是随便选了一个成员<1/4字即一个字节>进行填充）
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//设置优先级为高
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(BL_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//清除DMA数据流传输完成标志位
    DMA_ClearFlag(BL_USART_Rx_DMA_Stream, BL_USART_Rx_DMA_TC_FLAG);
	
	//使能传输完成中断
	DMA_ITConfig(BL_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//使能DMA通道
	DMA_Cmd(BL_USART_Rx_DMA_Stream, ENABLE); 
	
	//采用DMA方式接收
    USART_DMACmd(BL_USARTx, USART_DMAReq_Rx, ENABLE);  
}

/**
  * @brief  与蓝牙控制端的USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
static void BlueTooth_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	BL_USART_GPIO_ClkCmd(BL_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	BL_USART_ClkCmd(BL_USART_CLK, ENABLE);
	
	//启动DMA时钟  
    BL_USART_DMAClkCmd(BL_USART_DMA_CLK, ENABLE);
	
	//使能复用，即将GPIO连接到目标外设
	GPIO_PinAFConfig(BL_USART_TX_GPIO_PORT,BL_USART_TX_GPIO_PinSource,BL_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(BL_USART_RX_GPIO_PORT,BL_USART_RX_GPIO_PinSource,BL_USART_RX_GPIO_AF);
	
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = BL_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(BL_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = BL_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(BL_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = BL_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(BL_USARTx, &USART_InitStructure);
	
	// 使能串口接收中断
	USART_ITConfig(BL_USARTx, USART_IT_RXNE, ENABLE);

	//配置发送DMA
	BL_USART_Tx_DMA_Config();
	
	//配置接收DMA
	BL_USART_Rx_DMA_Config((uint32_t)(&(CMDBuffer[0])), DataHeadCorrect);
	 
	// 使能串口
	USART_Cmd(BL_USARTx, ENABLE);	    
}

/**
  * @brief  与全局定位相连的串口配置
  * @param  无
  * @retval 无
  */
static void Map_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	MAP_USART_GPIO_APBxClkCmd(MAP_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	MAP_USART_APBxClkCmd(MAP_USART_CLK, ENABLE);
	
	//使能复用，即将GPIO连接到目标外设
	GPIO_PinAFConfig(MAP_USART_TX_GPIO_PORT,MAP_USART_TX_GPIO_PinSource,MAP_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(MAP_USART_RX_GPIO_PORT,MAP_USART_RX_GPIO_PinSource,MAP_USART_RX_GPIO_AF);


	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = MAP_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MAP_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = MAP_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(MAP_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = MAP_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(MAP_USARTx, &USART_InitStructure);
	
	// 使能串口接收中断
	USART_ITConfig(MAP_USARTx, USART_IT_RXNE, ENABLE);	

	// 使能串口
	USART_Cmd(MAP_USARTx, ENABLE);	
	
}

/**
* @brief  初始化定位系统
* @param  无
* @retval 无
*/
static void Map_Config(void)
{
	Map_USART_Config();
	MapParamsInit();							//全局定位参量初始化}
}

/**
  * @brief  初始化通信串口
  * @param  无
  * @retval 无
  */
void USART_Config(void)
{
	//初始化中断向量
	NVIC_Configuration();
	
	Map_Config();
	Ctrl_USART_Config();		//初始化控制F103的串口
	Equips_USART_Config();		//初始化控制辅控的串口
	PC_USART_Config();			//初始化与小电脑的接口
	BlueTooth_USART_Config();	//初始化蓝牙遥控端口
}

/**
  * @brief  以DMA进行发送
  * @param  Tx_DMA_Stream：承担发送任务的流
  * @param  BufferSize：发送缓冲区的大小
  * @retval 无
  */
void SendMsg_byDMA(DMA_Stream_TypeDef * Tx_DMA_Stream, uint32_t BufferSize)
{
	DMA_Cmd(Tx_DMA_Stream, DISABLE );  				//关闭Tx_DMA_Channel 所指示的通道        
    DMA_SetCurrDataCounter(Tx_DMA_Stream, BufferSize);//DMA通道的DMA缓存的大小  
    DMA_Cmd(Tx_DMA_Stream, ENABLE);  				//使能Tx_DMA_Channel 所指示的通道   
}

 /**
  * @brief  更改接收缓冲区地址以及数量
  * @param  DMAx_BASE:DMA外设基地址，x可以为1或2
  * @param  ChannelNum:传输通道号
  * @param  HeadCorrectValue:数据接收校准值
  * @param  RxBase:接收内存缓冲区地址
  * @retval 无
  */
void DMA_ReceiveSet_Change(uint32_t DMAx_BASE, uint32_t ChannelNum, uint32_t RxBase, uint32_t HeadCorrectValue)
{
	*(uint32_t*)(DMAx_BASE + 0x1C + 0x18 * ChannelNum) =  RxBase;
	
	*(uint32_t*)(DMAx_BASE + 0x14 + 0x18 * ChannelNum) |=  HeadCorrectValue % 0x10000;
}



/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

