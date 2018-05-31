#include "can.h"

void CAN_Config()
{
	//**********************IO*************************//
	GPIO_InitTypeDef GPIO_InitStructure;  
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure; 	

	/* 使能GPIO时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* 配置 CAN TX 引脚 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置 CAN RX 引脚 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//*********************CAN_MODE**********************//
	
	/* Enable CAN clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	/*CAN寄存器初始化*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/*CAN单元初始化*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  关闭时间触发通信模式使能
	CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  使能自动离线管理 
	CAN_InitStructure.CAN_AWUM=ENABLE;			   //MCR-AWUM  使用自动唤醒模式
	CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  报文自动重传	  
	CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 不锁定 溢出时新报文会覆盖原有报文  
	CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 取决于报文标示符 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_Silent_LoopBack;//CAN_Mode_Normal;  //正常工作模式
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW 重新同步跳跃宽度 1个时间单元

	/* ss=1 bs1=5 bs2=3 位时间宽度为(1+5+3) 波特率即为时钟周期tq*(1+3+5)  */
	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq;		   //BTR-TS1 时间段1 占用了5个时间单元
	CAN_InitStructure.CAN_BS2=CAN_BS2_3tq;		   //BTR-TS1 时间段2 占用了3个时间单元	

	/* CAN Baudrate = 1 MBps (1MBps已为stm32的CAN最高速率) (CAN 时钟频率为 APB 1 = 45 MHz) */     
	CAN_InitStructure.CAN_Prescaler =4;		  //BTR-BRP 波特率分频器  定义了时间单元的时间长度 45/(1+5+3)/5=1 Mbps
	CAN_Init(CAN1, &CAN_InitStructure);
	
	
	//***********************CAN_FILITER*******************//
	

	/*CAN筛选器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//筛选器组0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在掩码模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;	//筛选器位宽为单16位。
	/* 使能筛选器，按照标志的内容进行比对筛选，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

	CAN_FilterInitStructure.CAN_FilterIdHigh= (CAN_ID1<<5);		//要筛选的ID高位 
	CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID2<<5); //要筛选的ID低位 
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh= CAN_FILTER1;			//筛选器高16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow= CAN_FILTER2;			//筛选器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//筛选器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能筛选器
	CAN_FilterInit(&CAN_FilterInitStructure);

	/*CAN通信中断使能*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

uint8_t CAN_SendMassage(uint16_t ID, uint8_t dataLength, uint8_t *canSendMassage)
{
	CanTxMsg txMassage;
	uint8_t i = 0,j = 0;
	uint8_t canMailBoxFlag;
	
	txMassage.StdId = (ID & 0x7FF);
	txMassage.IDE = CAN_ID_STD;
	txMassage.RTR = CAN_RTR_DATA;
	txMassage.DLC = dataLength;

	for(i = 0; i< dataLength; i++)
		txMassage.Data[i] = canSendMassage[i];

	canMailBoxFlag = CAN_Transmit(CAN1, &txMassage);
	
	while((canMailBoxFlag == CAN_TxStatus_NoMailBox)&&(j<4))//如果发送邮箱全部被占用，延时在发送
	{
		//Delay(0xffff);
		delay_us(120);
		canMailBoxFlag = CAN_Transmit(CAN1, &txMassage);
		j++;
	}
	
	if((CAN_TransmitStatus(CAN1,canMailBoxFlag) == CAN_TxStatus_Failed)||(j>=5))
		return 0;
	else
		return 1;
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
