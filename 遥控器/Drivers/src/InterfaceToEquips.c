#include "InterfaceToEquips.h"
#include "ActionCtrl.h"
#include "WatchDog.h"
#include "Process.h"
#include "USART.h"
#include "Base.h"
#include "Map.h"
#include <stdint.h>

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//输出限幅

/* Public  variables ----------------------*/									//各电机的实际转速值
Eq_Rx_BufferType		Eq_RxBuffer[2];											//双接收缓冲区
Eq_Msg_Tx_BufferType	Eq_Tx_Buffer;
uint8_t				 	Eq_BufferFlag = 0;										//目前使用的缓冲区标号
int16_t 				Eq_DataHeadCorrect = Equips_USART_Rx_DMA_BufferSize;	//数据头位置矫正值

/* Private  variables ---------------------------------------------------------*/
uint8_t RcvEq_Ready = 0;							//接收成功标志，读取数据后复位，复位前不接收数据
bool Eq_isSendingflag = false;						//发送互斥量，在发送结束之前不允许更该缓冲区内容 

/**
  * @brief  初始化气缸初始状态
  * @param  无
  * @retval 无
  */
void InitCylinderState(void)
{
//	Eq_Tx_Buffer.TxBuffer.BasicCylinderState = 0xff;
}

/**
  * @brief  从串口数据缓冲区中读取数据
  * @param  无
  * @retval 无
  */
void ReceiveDataFromEquips_Buffer(const Eq_Rx_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = Equips_USART_Rx_DMA_BufferSize;
	
	if(Eq_DataHeadCorrect != Equips_USART_Rx_DMA_BufferSize)//说明是在进行补偿操作，纠正数据头位置
	{	
		Eq_DataHeadCorrect = Equips_USART_Rx_DMA_BufferSize;
		
		return;
	}
	else
	{
		for(i = 0; i < LastBuffersize - 4; i++)
		{
			check += CMDData.Bytes[i];					//求和校验
			
			if(0xaa == CMDData.Bytes[i])				//检索数据头位置
			{
				temp = i;
			}
			if((0xbb == CMDData.Bytes[i]) || (temp == (i-1)))	//如果这一位是0xBB,且前一位是0xAA，
			{													//那么数据头在缓冲区的位置就可以确定
				
				Eq_DataHeadCorrect = temp;//下一次接收这么多位就可以消除数据错位的现象

			}
			
			if(0 >= Eq_DataHeadCorrect)		//避免出现校准值为零即接收数据长度为零的情况(为负的情况是由内存数据对齐导致的)
			{							//如果是零，那么DMA就不会继续传输数据了
				Eq_DataHeadCorrect = Equips_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//若满足校验条件，则刷入数据
		{
//			FeedDog(Equips);
			
			for(enum ActDevices i = Dev_LeftCylinder; i < DevNum; i = (enum ActDevices)(i+1))
			{
				isReady[i] = (CMDData.buffer.ActFinished & finishFlags[i]);	//获取动作完成状态
			}
			
			if(CMDData.buffer.CoordValid)	//若激光获取的绝对坐标有效
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
  * @brief  向辅控发送数据
  * @param  无
  * @retval 无
  */
void SendDataToEquips(void)
{
	uint8_t i;
	int32_t temp = 0;
	
	SendStateScan();
	
	Eq_Tx_Buffer.TxBuffer.Data_head1 = 0xaa;
	Eq_Tx_Buffer.TxBuffer.Data_head2 = 0xbb;
	
	Eq_Tx_Buffer.TxBuffer.Data_cnt = 0x00;

	Eq_Tx_Buffer.TxBuffer.PointCnt = MovePointCnt;	//标记状态：运动点计数器
	
	for(i = 0; i < (Equips_USART_Tx_DMA_BufferSize - 4); i++)	//计算校验
	{
		temp += Eq_Tx_Buffer.Bytes[i];
	}
	
	Eq_Tx_Buffer.TxBuffer.verify = temp;
	
	Eq_isSendingflag = true;
	
	SendMsg_byDMA(Equips_USART_Tx_DMA_Stream, Equips_USART_Tx_DMA_BufferSize);	//发送数据
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

