#include "can.h"

CanRxMsg  CAN_Rece_Data;
uint8_t CAN_TX_Success_flag=0;

 /**
  * @brief  can NVIC 初始化
  * @param  无
  * @retval 无
  */
static void Can_NVIC_Init(void)
{
	NVIC_InitTypeDef Init;
	
	Init.NVIC_IRQChannel = CAN_RX_IRQ;
	Init.NVIC_IRQChannelPreemptionPriority = 0;
	Init.NVIC_IRQChannelSubPriority = 0;
	Init.NVIC_IRQChannelCmd = ENABLE;
	
	Init.NVIC_IRQChannel = CAN_TX_IRQ;
	Init.NVIC_IRQChannelPreemptionPriority = 0;
	Init.NVIC_IRQChannelSubPriority = 0;
	Init.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&Init);
}

 /**
  * @brief  can GPIO 初始化
  * @param  无
  * @retval 无
  */
static void Can_GPIO_Init(void)
{
	GPIO_InitTypeDef Init;
	
	//开启GPIO时钟
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);
	
	//连接外设到GPIO源(复用引脚初始化)
	GPIO_PinAFConfig(CAN_RX_GPIO_PORT, CAN_RX_GPIO_PinSourcex, CAN_AF_PORT);
	GPIO_PinAFConfig(CAN_TX_GPIO_PORT, CAN_TX_GPIO_PinSourcex, CAN_AF_PORT);
	
	Init.GPIO_Pin = CAN_TX_GPIO_PIN;
	Init.GPIO_Mode = GPIO_Mode_AF;
	Init.GPIO_OType = GPIO_OType_PP;
	Init.GPIO_Speed = GPIO_Speed_50MHz;
	Init.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(CAN_TX_GPIO_PORT, &Init);
	
	Init.GPIO_Pin = CAN_RX_GPIO_PIN;
	GPIO_Init(CAN_RX_GPIO_PORT, &Init);
}

 /**
  * @brief  can 筛选器 初始化
  * @param  无
  * @retval 无
  */
static void Can_Filter_Init(void)
{
	CAN_FilterInitTypeDef Init;
	
	/*CAN筛选器初始化*/
	Init.CAN_FilterNumber = 14;							//筛选器组14
	Init.CAN_FilterMode = CAN_FilterMode_IdMask;		//工作在掩码模式
	Init.CAN_FilterScale = CAN_FilterScale_32bit;		//筛选器位宽为单个32位。
	
	/* 使能筛选器，按照标志的内容进行比对筛选，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。*/
	Init.CAN_FilterIdHigh = 0x0000;						//不筛选ID，全部接收
	Init.CAN_FilterIdLow = 0x0000;
	Init.CAN_FilterMaskIdHigh = 0x0000;					//全部不用匹配
	Init.CAN_FilterMaskIdLow = 0x0000;					//全部不用匹配
	Init.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1 ;	//筛选器被关联到FIFO1
	Init.CAN_FilterActivation = ENABLE;					//使能筛选器
	
	CAN_FilterInit(&Init);
	
	/*CAN通信中断使能*/
	CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE);
	CAN_ITConfig(CANx,CAN_IT_TME,ENABLE);

}

 /**
  * @brief  can 工作参数 初始化
  * @param  无
  * @retval 无
  */
static void Can_Mode_Init(void)
{
	CAN_InitTypeDef Init;
	
	/*开CAN2外设时钟*/
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	/*CAN寄存器初始化*/
	CAN_DeInit(CANx);
	CAN_StructInit(&Init);
	
	//配置模式为一般模式
	Init.CAN_Mode = CAN_Mode_Silent_LoopBack;//;CAN_Mode_Normal
	
	/*CAN单元初始化*/
	Init.CAN_TTCM = DISABLE;		//MCR-TTCM  关闭时间触发通信模式使能
	Init.CAN_ABOM = ENABLE;			//MCR-ABOM  使用自动离线管理 
	Init.CAN_AWUM = ENABLE;			//MCR-AWUM  使用自动唤醒模式
	Init.CAN_NART = ENABLE;			//MCR-NART  关闭报文自动重传	  	ENABLE-使能自动重传
	Init.CAN_RFLM = DISABLE;		//MCR-RFLM  接收FIFO 锁定模式  	DISABLE-溢出时新报文会覆盖原有报文  
	Init.CAN_TXFP = ENABLE;			//MCR-TXFP  发送FIFO优先级 		ENABLE-优先级取决于信息存入邮箱的顺序 DISABLE-优先级取决于报文标示符 
	
	/* ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+5)  */
	Init.CAN_BS1=CAN_BS1_5tq;		   	//BTR-TS1 时间段1 占用了6个时间单元
	Init.CAN_BS2=CAN_BS2_3tq;		   	//BTR-TS1 时间段2 占用了3个时间单元	
	
	Init.CAN_SJW=CAN_SJW_2tq;		   	//BTR-SJW 重新同步跳跃宽度 1个时间单元
	
	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 45 MHz) */
	Init.CAN_Prescaler = 5;		   		//BTR-BRP 波特率分频器  定义了时间单元的时间长度 45/(1+5+3)/5=1 Mbps

	CAN_Init(CANx, &Init);
}

 /**
  * @brief  can初始化
  * @param  无
  * @retval 无
  */
void CanInit(void)
{
	Can_GPIO_Init();
	Can_NVIC_Init();
	Can_Mode_Init();
	Can_Filter_Init();
}

/**
  * @brief  初始化 Rx Message数据结构体（目的是格式化数据接收结构体变量中的残余数据，防止产生干扰）
  * @param  RxMessage: 指向要初始化的数据结构体
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

	/*把接收结构体清零*/
  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage->Data[ubCounter] = 0x00;
  }
}

/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置,设置一个数据内容为0-7的数据包
 * 输入  ：发送报文结构体
 * 输出  : 无
 * 调用  ：外部调用
 */	 
void CAN_SetMsg(CanTxMsg *TxMessage)
{	  
	uint8_t ubCounter = 0;

  //TxMessage.StdId=0x00;						 
  TxMessage->ExtId=0x1314;					 //使用的扩展ID
  TxMessage->IDE=CAN_ID_EXT;					 //扩展模式
  TxMessage->RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage->DLC=8;							 //数据长度为8字节
	
	/*设置要发送的数据0-7*/
	for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    TxMessage->Data[ubCounter] = ubCounter;
  }
}

///**
//  * @brief  CAN_SendMsg:CAN通信报文内容设置与发送
//  * @param  StdID: 待发送数据帧的ID
//  * @param  TxMsg: 指向要发送的数据帧
//  * @retval None
//  */ 
//void CAN_SendMsg(uint32_t StdID, EquipsCtrlTypeDef* TxMsg)
//{	 
//	CanTxMsg TxMessage;
//	
//	TxMessage.StdId = StdID;			//使用的标准ID
//	//TxMessage.ExtId=0x1314;			//使用的扩展ID
//	TxMessage.IDE = CAN_ID_STD;			//标准模式
//	TxMessage.RTR = CAN_RTR_DATA;		//发送的是数据
//	TxMessage.DLC = 8;					//数据长度为8字节
//	
//	TxMessage.Data[0] = TxMsg->CylinderNum;
//	TxMessage.Data[1] = TxMsg->Actions;
//	TxMessage.Data[2] = TxMsg->LCD_Color;
//	TxMessage.Data[3] = TxMsg->LaserCmd;
//	TxMessage.Data[4] = TxMsg->PoseState;
//	TxMessage.Data[5] = 0x00;				//预留
//	TxMessage.Data[6] = 0x00;				//预留
//	TxMessage.Data[7] = 0x00;				//预留
//	
//	CAN_Transmit(CANx, &TxMessage);

//}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
