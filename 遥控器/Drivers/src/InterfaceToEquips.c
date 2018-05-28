#include "InterfaceToEquips.h"
#include "ActionCtrl.h"
#include "WatchDog.h"
#include "Process.h"
#include "USART.h"
#include "Base.h"
#include "Map.h"
#include <stdint.h>

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//����޷�

/* Public  variables ----------------------*/									//�������ʵ��ת��ֵ
Eq_Rx_BufferType		Eq_RxBuffer[2];											//˫���ջ�����
Eq_Msg_Tx_BufferType	Eq_Tx_Buffer;
uint8_t				 	Eq_BufferFlag = 0;										//Ŀǰʹ�õĻ��������
int16_t 				Eq_DataHeadCorrect = Equips_USART_Rx_DMA_BufferSize;	//����ͷλ�ý���ֵ

/* Private  variables ---------------------------------------------------------*/
uint8_t RcvEq_Ready = 0;							//���ճɹ���־����ȡ���ݺ�λ����λǰ����������
bool Eq_isSendingflag = false;						//���ͻ��������ڷ��ͽ���֮ǰ��������û��������� 

/**
  * @brief  ��ʼ�����׳�ʼ״̬
  * @param  ��
  * @retval ��
  */
void InitCylinderState(void)
{
//	Eq_Tx_Buffer.TxBuffer.BasicCylinderState = 0xff;
}

/**
  * @brief  �Ӵ������ݻ������ж�ȡ����
  * @param  ��
  * @retval ��
  */
void ReceiveDataFromEquips_Buffer(const Eq_Rx_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = Equips_USART_Rx_DMA_BufferSize;
	
	if(Eq_DataHeadCorrect != Equips_USART_Rx_DMA_BufferSize)//˵�����ڽ��в�����������������ͷλ��
	{	
		Eq_DataHeadCorrect = Equips_USART_Rx_DMA_BufferSize;
		
		return;
	}
	else
	{
		for(i = 0; i < LastBuffersize - 4; i++)
		{
			check += CMDData.Bytes[i];					//���У��
			
			if(0xaa == CMDData.Bytes[i])				//��������ͷλ��
			{
				temp = i;
			}
			if((0xbb == CMDData.Bytes[i]) || (temp == (i-1)))	//�����һλ��0xBB,��ǰһλ��0xAA��
			{													//��ô����ͷ�ڻ�������λ�þͿ���ȷ��
				
				Eq_DataHeadCorrect = temp;//��һ�ν�����ô��λ�Ϳ����������ݴ�λ������

			}
			
			if(0 >= Eq_DataHeadCorrect)		//�������У׼ֵΪ�㼴�������ݳ���Ϊ������(Ϊ������������ڴ����ݶ��뵼�µ�)
			{							//������㣬��ôDMA�Ͳ����������������
				Eq_DataHeadCorrect = Equips_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//������У����������ˢ������
		{
//			FeedDog(Equips);
			
			for(enum ActDevices i = Dev_LeftCylinder; i < DevNum; i = (enum ActDevices)(i+1))
			{
				isReady[i] = (CMDData.buffer.ActFinished & finishFlags[i]);	//��ȡ�������״̬
			}
			
			if(CMDData.buffer.CoordValid)	//�������ȡ�ľ���������Ч
			{
				OtherCoordinate.point.X = CMDData.buffer.Coord_X;
				OtherCoordinate.point.Y = CMDData.buffer.Coord_Y;
				OtherCoordinate.Angle = CMDData.buffer.Coord_Angle;
			}
			
			HaveBallNumber = CMDData.buffer.HaveBallCnt;
			
			isActFinishied = CMDData.buffer.ActFinished;
						
			if(CMDData.buffer.ACK == true)
			{
				ActACK = true;
			}
			
			AirPressureState = (enum AirPressureStateType)CMDData.buffer.PressureState;
		}
		
		LastBuffersize = Eq_DataHeadCorrect;
	}
}

 
/**
  * @brief  �򸨿ط�������
  * @param  ��
  * @retval ��
  */
void SendDataToEquips(void)
{
	uint8_t i;
	int32_t temp = 0;
	
	SendStateScan();
	
	Eq_Tx_Buffer.TxBuffer.Data_head1 = 0xaa;
	Eq_Tx_Buffer.TxBuffer.Data_head2 = 0xbb;
	
	Eq_Tx_Buffer.TxBuffer.Data_cnt = 0x00;

	Eq_Tx_Buffer.TxBuffer.PointCnt = MovePointCnt;	//���״̬���˶��������
	
	for(i = 0; i < (Equips_USART_Tx_DMA_BufferSize - 4); i++)	//����У��
	{
		temp += Eq_Tx_Buffer.Bytes[i];
	}
	
	Eq_Tx_Buffer.TxBuffer.verify = temp;
	
	Eq_isSendingflag = true;
	
	SendMsg_byDMA(Equips_USART_Tx_DMA_Stream, Equips_USART_Tx_DMA_BufferSize);	//��������
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

