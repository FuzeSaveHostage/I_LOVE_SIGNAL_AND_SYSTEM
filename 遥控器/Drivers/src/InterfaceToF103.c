#include "InterfaceToMaster.h"
#include "InterfaceToF103.h"
#include "Motion_Ctrl.h"
#include "Vol_curve.h"
#include "STM32F4xx.h"
#include "WatchDog.h"
#include "USART.h"
#include "Base.h"
#include "Map.h"

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//����޷�

/* Public  variables ----------------------*/
wheel_speed AimSpeed;														//�������Ŀ��ת��ֵ
wheel_speed RealSpeed;														//�������ʵ��ת��ֵ
F1_Rx_BufferType		F1_RxBuffer[2];										//˫���ջ�����
F1_Msg_Tx_BufferType	F1_Tx_Buffer;
uint8_t				 	F1_BufferFlag = 0;									//Ŀǰʹ�õĻ��������
uint8_t 				F1_PID_flag = 0;									//1ΪPID������Ч��0Ϊ��Ч
int16_t 				F1_DataHeadCorrect = CTRL_USART_Rx_DMA_BufferSize;	//����ͷλ�ý���ֵ

/* Private  variables ---------------------------------------------------------*/
uint8_t f = 0;									//�����õı�־
uint8_t g = 0;

uint8_t RcvF1_Ready = 0;							//���ճɹ���־����ȡ���ݺ�λ����λǰ����������
 
/**
  * @brief  �Ӵ������ݻ������ж�ȡ����
  * @param  ��
  * @retval ��
  */
void ReceiveDataFromF103_Buffer(const F1_Rx_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = CTRL_USART_Rx_DMA_BufferSize;
	
	if(F1_DataHeadCorrect != CTRL_USART_Rx_DMA_BufferSize)//˵�����ڽ��в�����������������ͷλ��
	{	
		F1_DataHeadCorrect = CTRL_USART_Rx_DMA_BufferSize;
		
		return;
	}
	else
	{
		for(i = 0; i < LastBuffersize - 6; i++)	//���У��
		{
			check += CMDData.Bytes[i];					//���У��
			
			if(0xaa == CMDData.Bytes[i])				//��������ͷλ��
			{
				temp = i;
			}
			if((0xbb == CMDData.Bytes[i]) || (temp == (i-1)))	//�����һλ��0xBB,��ǰһλ��0xAA��
			{													//��ô����ͷ�ڻ�������λ�þͿ���ȷ��
				
				F1_DataHeadCorrect = temp;//��һ�ν�����ô��λ�Ϳ����������ݴ�λ������

			}
			
			if(0 >= F1_DataHeadCorrect)		//�������У׼ֵΪ�㼴�������ݳ���Ϊ������(Ϊ������������ڴ����ݶ��뵼�µ�)
			{							//������㣬��ôDMA�Ͳ����������������
				F1_DataHeadCorrect = CTRL_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//������У����������ˢ������
		{
			AimSpeed.v1 = CMDData.buffer.AimSpeed1;	
			AimSpeed.v2 = CMDData.buffer.AimSpeed2;	
			AimSpeed.v3 = CMDData.buffer.AimSpeed3;
			
			RealSpeed.v1 = CMDData.buffer.RealSpeed1;	
			RealSpeed.v2 = CMDData.buffer.RealSpeed2;	
			RealSpeed.v3 = CMDData.buffer.RealSpeed3;	
			
			Vol_Curve.vol_max = CMDData.buffer.max_vol;
			
			Vol_Curve.a_max = CMDData.buffer.max_a;
		}
		
		LastBuffersize = F1_DataHeadCorrect;
	}
}

 
/**
  * @brief  ��F103��������
  * @param  ��
  * @retval ��
  */
void SendDataToF103(uint8_t PIDflag)
{
	uint8_t i;
	int32_t temp = 0;
	
	F1_Tx_Buffer.TxBuffer.Data_head1 = 0xaa;
	F1_Tx_Buffer.TxBuffer.Data_head2 = 0xbb;
	
	F1_Tx_Buffer.TxBuffer.CMD_type = PIDflag;					//���PID��Ϣ
	F1_Tx_Buffer.TxBuffer.Data_cnt = 0x00;
	
	F1_Tx_Buffer.TxBuffer.lineSpeed 	= Motion.outSpeed;
	F1_Tx_Buffer.TxBuffer.Ward 			= Motion.Ward;
	F1_Tx_Buffer.TxBuffer.Aim_PosAngle 	= Base_CMD.Pos_Angle;
	F1_Tx_Buffer.TxBuffer.AngleCnt 		= Base_CMD.Angle_cnt;
	F1_Tx_Buffer.TxBuffer.PosAngle 		= Coordinate.Angle;
	
	F1_Tx_Buffer.TxBuffer.PID_Current_P = PIDSetting.Current.p;
	F1_Tx_Buffer.TxBuffer.PID_Current_I = PIDSetting.Current.i;
	F1_Tx_Buffer.TxBuffer.PID_Current_D = PIDSetting.Current.d;
	
	F1_Tx_Buffer.TxBuffer.PID_Angle_P = PIDSetting.Angle.p;
	F1_Tx_Buffer.TxBuffer.PID_Angle_I = PIDSetting.Angle.i;
	F1_Tx_Buffer.TxBuffer.PID_Angle_D = PIDSetting.Angle.d;

	
	for(i = 0; i < (CTRL_USART_Tx_DMA_BufferSize - 6); i++)	//����У��
	{
		temp += F1_Tx_Buffer.Bytes[i];
	}
	
	F1_Tx_Buffer.TxBuffer.verify = temp;// % 0x100;
	
	SendMsg_byDMA(CTRL_USART_Tx_DMA_Stream, CTRL_USART_Tx_DMA_BufferSize);									//��������
	
	PIDflag = 0;
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
