#include "can.h"

CanRxMsg  CAN_Rece_Data;
uint8_t CAN_TX_Success_flag=0;

 /**
  * @brief  can NVIC ��ʼ��
  * @param  ��
  * @retval ��
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
  * @brief  can GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void Can_GPIO_Init(void)
{
	GPIO_InitTypeDef Init;
	
	//����GPIOʱ��
	RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);
	
	//�������赽GPIOԴ(�������ų�ʼ��)
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
  * @brief  can ɸѡ�� ��ʼ��
  * @param  ��
  * @retval ��
  */
static void Can_Filter_Init(void)
{
	CAN_FilterInitTypeDef Init;
	
	/*CANɸѡ����ʼ��*/
	Init.CAN_FilterNumber = 14;							//ɸѡ����14
	Init.CAN_FilterMode = CAN_FilterMode_IdMask;		//����������ģʽ
	Init.CAN_FilterScale = CAN_FilterScale_32bit;		//ɸѡ��λ��Ϊ����32λ��
	
	/* ʹ��ɸѡ�������ձ�־�����ݽ��бȶ�ɸѡ����չID�������µľ����������ǵĻ��������FIFO0��*/
	Init.CAN_FilterIdHigh = 0x0000;						//��ɸѡID��ȫ������
	Init.CAN_FilterIdLow = 0x0000;
	Init.CAN_FilterMaskIdHigh = 0x0000;					//ȫ������ƥ��
	Init.CAN_FilterMaskIdLow = 0x0000;					//ȫ������ƥ��
	Init.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1 ;	//ɸѡ����������FIFO1
	Init.CAN_FilterActivation = ENABLE;					//ʹ��ɸѡ��
	
	CAN_FilterInit(&Init);
	
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CANx, CAN_IT_FMP1, ENABLE);
	CAN_ITConfig(CANx,CAN_IT_TME,ENABLE);

}

 /**
  * @brief  can �������� ��ʼ��
  * @param  ��
  * @retval ��
  */
static void Can_Mode_Init(void)
{
	CAN_InitTypeDef Init;
	
	/*��CAN2����ʱ��*/
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	/*CAN�Ĵ�����ʼ��*/
	CAN_DeInit(CANx);
	CAN_StructInit(&Init);
	
	//����ģʽΪһ��ģʽ
	Init.CAN_Mode = CAN_Mode_Silent_LoopBack;//;CAN_Mode_Normal
	
	/*CAN��Ԫ��ʼ��*/
	Init.CAN_TTCM = DISABLE;		//MCR-TTCM  �ر�ʱ�䴥��ͨ��ģʽʹ��
	Init.CAN_ABOM = ENABLE;			//MCR-ABOM  ʹ���Զ����߹��� 
	Init.CAN_AWUM = ENABLE;			//MCR-AWUM  ʹ���Զ�����ģʽ
	Init.CAN_NART = ENABLE;			//MCR-NART  �رձ����Զ��ش�	  	ENABLE-ʹ���Զ��ش�
	Init.CAN_RFLM = DISABLE;		//MCR-RFLM  ����FIFO ����ģʽ  	DISABLE-���ʱ�±��ĻḲ��ԭ�б���  
	Init.CAN_TXFP = ENABLE;			//MCR-TXFP  ����FIFO���ȼ� 		ENABLE-���ȼ�ȡ������Ϣ���������˳�� DISABLE-���ȼ�ȡ���ڱ��ı�ʾ�� 
	
	/* ss=1 bs1=5 bs2=3 λʱ����Ϊ(1+5+3) �����ʼ�Ϊʱ������tq*(1+3+5)  */
	Init.CAN_BS1=CAN_BS1_5tq;		   	//BTR-TS1 ʱ���1 ռ����6��ʱ�䵥Ԫ
	Init.CAN_BS2=CAN_BS2_3tq;		   	//BTR-TS1 ʱ���2 ռ����3��ʱ�䵥Ԫ	
	
	Init.CAN_SJW=CAN_SJW_2tq;		   	//BTR-SJW ����ͬ����Ծ��� 1��ʱ�䵥Ԫ
	
	/* CAN Baudrate = 1 MBps (1MBps��Ϊstm32��CAN�������) (CAN ʱ��Ƶ��Ϊ APB 1 = 45 MHz) */
	Init.CAN_Prescaler = 5;		   		//BTR-BRP �����ʷ�Ƶ��  ������ʱ�䵥Ԫ��ʱ�䳤�� 45/(1+5+3)/5=1 Mbps

	CAN_Init(CANx, &Init);
}

 /**
  * @brief  can��ʼ��
  * @param  ��
  * @retval ��
  */
void CanInit(void)
{
	Can_GPIO_Init();
	Can_NVIC_Init();
	Can_Mode_Init();
	Can_Filter_Init();
}

/**
  * @brief  ��ʼ�� Rx Message���ݽṹ�壨Ŀ���Ǹ�ʽ�����ݽ��սṹ������еĲ������ݣ���ֹ�������ţ�
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

	/*�ѽ��սṹ������*/
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
 * ��������CAN_SetMsg
 * ����  ��CANͨ�ű�����������,����һ����������Ϊ0-7�����ݰ�
 * ����  �����ͱ��Ľṹ��
 * ���  : ��
 * ����  ���ⲿ����
 */	 
void CAN_SetMsg(CanTxMsg *TxMessage)
{	  
	uint8_t ubCounter = 0;

  //TxMessage.StdId=0x00;						 
  TxMessage->ExtId=0x1314;					 //ʹ�õ���չID
  TxMessage->IDE=CAN_ID_EXT;					 //��չģʽ
  TxMessage->RTR=CAN_RTR_DATA;				 //���͵�������
  TxMessage->DLC=8;							 //���ݳ���Ϊ8�ֽ�
	
	/*����Ҫ���͵�����0-7*/
	for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    TxMessage->Data[ubCounter] = ubCounter;
  }
}

///**
//  * @brief  CAN_SendMsg:CANͨ�ű������������뷢��
//  * @param  StdID: ����������֡��ID
//  * @param  TxMsg: ָ��Ҫ���͵�����֡
//  * @retval None
//  */ 
//void CAN_SendMsg(uint32_t StdID, EquipsCtrlTypeDef* TxMsg)
//{	 
//	CanTxMsg TxMessage;
//	
//	TxMessage.StdId = StdID;			//ʹ�õı�׼ID
//	//TxMessage.ExtId=0x1314;			//ʹ�õ���չID
//	TxMessage.IDE = CAN_ID_STD;			//��׼ģʽ
//	TxMessage.RTR = CAN_RTR_DATA;		//���͵�������
//	TxMessage.DLC = 8;					//���ݳ���Ϊ8�ֽ�
//	
//	TxMessage.Data[0] = TxMsg->CylinderNum;
//	TxMessage.Data[1] = TxMsg->Actions;
//	TxMessage.Data[2] = TxMsg->LCD_Color;
//	TxMessage.Data[3] = TxMsg->LaserCmd;
//	TxMessage.Data[4] = TxMsg->PoseState;
//	TxMessage.Data[5] = 0x00;				//Ԥ��
//	TxMessage.Data[6] = 0x00;				//Ԥ��
//	TxMessage.Data[7] = 0x00;				//Ԥ��
//	
//	CAN_Transmit(CANx, &TxMessage);

//}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
