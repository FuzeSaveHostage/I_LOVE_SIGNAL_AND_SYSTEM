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
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//输出限幅

/* Public  variables ----------------------*/
wheel_speed AimSpeed;														//各电机的目标转速值
wheel_speed RealSpeed;														//各电机的实际转速值
F1_Rx_BufferType		F1_RxBuffer[2];										//双接收缓冲区
F1_Msg_Tx_BufferType	F1_Tx_Buffer;
uint8_t				 	F1_BufferFlag = 0;									//目前使用的缓冲区标号
uint8_t 				F1_PID_flag = 0;									//1为PID数据有效，0为无效
int16_t 				F1_DataHeadCorrect = CTRL_USART_Rx_DMA_BufferSize;	//数据头位置矫正值

/* Private  variables ---------------------------------------------------------*/
uint8_t f = 0;									//测试用的标志
uint8_t g = 0;

uint8_t RcvF1_Ready = 0;							//接收成功标志，读取数据后复位，复位前不接收数据
 
/**
  * @brief  从串口数据缓冲区中读取数据
  * @param  无
  * @retval 无
  */
void ReceiveDataFromF103_Buffer(const F1_Rx_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = CTRL_USART_Rx_DMA_BufferSize;
	
	if(F1_DataHeadCorrect != CTRL_USART_Rx_DMA_BufferSize)//说明是在进行补偿操作，纠正数据头位置
	{	
		F1_DataHeadCorrect = CTRL_USART_Rx_DMA_BufferSize;
		
		return;
	}
	else
	{
		for(i = 0; i < LastBuffersize - 6; i++)	//求和校验
		{
			check += CMDData.Bytes[i];					//求和校验
			
			if(0xaa == CMDData.Bytes[i])				//检索数据头位置
			{
				temp = i;
			}
			if((0xbb == CMDData.Bytes[i]) || (temp == (i-1)))	//如果这一位是0xBB,且前一位是0xAA，
			{													//那么数据头在缓冲区的位置就可以确定
				
				F1_DataHeadCorrect = temp;//下一次接收这么多位就可以消除数据错位的现象

			}
			
			if(0 >= F1_DataHeadCorrect)		//避免出现校准值为零即接收数据长度为零的情况(为负的情况是由内存数据对齐导致的)
			{							//如果是零，那么DMA就不会继续传输数据了
				F1_DataHeadCorrect = CTRL_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//若满足校验条件，则刷入数据
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
  * @brief  向F103发送数据
  * @param  无
  * @retval 无
  */
void SendDataToF103(uint8_t PIDflag)
{
	uint8_t i;
	int32_t temp = 0;
	
	F1_Tx_Buffer.TxBuffer.Data_head1 = 0xaa;
	F1_Tx_Buffer.TxBuffer.Data_head2 = 0xbb;
	
	F1_Tx_Buffer.TxBuffer.CMD_type = PIDflag;					//标记PID信息
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

	
	for(i = 0; i < (CTRL_USART_Tx_DMA_BufferSize - 6); i++)	//计算校验
	{
		temp += F1_Tx_Buffer.Bytes[i];
	}
	
	F1_Tx_Buffer.TxBuffer.verify = temp;// % 0x100;
	
	SendMsg_byDMA(CTRL_USART_Tx_DMA_Stream, CTRL_USART_Tx_DMA_BufferSize);									//发送数据
	
	PIDflag = 0;
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
