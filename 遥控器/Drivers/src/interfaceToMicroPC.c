#include "InterfaceToMicroPC.h"
#include "InterfaceToMaster.h"
#include "Motion_Ctrl.h"
#include "ActionCtrl.h"
#include "STM32F4xx.h"
#include "Vol_curve.h"
#include "WatchDog.h"
#include "Process.h"
#include "USART.h"
#include "Base.h"
#include "math.h"
#include "Map.h"

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//输出限幅

/* Public  variables ----------------------*/
PC_Rx_BufferType		PC_RxBuffer[2];									//双接收缓冲区
PC_Msg_Tx_BufferType	PC_Tx_Buffer;
uint8_t				 	PC_BufferFlag = 0;									//目前使用的缓冲区标号
int16_t 				PC_DataHeadCorrect = PC_USART_Rx_DMA_BufferSize;		//数据头位置矫正值

uint8_t KEY1_value;
uint8_t KEY2_value;
uint8_t Rotate_value;   //角度相关值
uint8_t caculed_ward;  //解算运动角度

/* Private  variables ---------------------------------------------------------*/
uint8_t RcvPC_Ready = 0;							//接收成功标志，读取数据后复位，复位前不接收数据

SendBuffer sendbuffer;       //设置一个缓存区存储底盘速度、方向、角度三个变量
int8_t Axis[3];              //用到的三个摇杆变量

 
/**
  * @brief  从串口数据缓冲区中读取数据
  * @param  无
  * @retval 无
  */
void ReceiveDataFromPC_Buffer(const PC_Rx_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = PC_USART_Rx_DMA_BufferSize;
	
//	static uint8_t key = 0,key2 = 0;   //记录上次满足校验时按键变量的值
//	static uint8_t speed_flag = 0;  //0默认为慢速
//	static uint8_t manual_auto = 0;  //手动与自动切换标志，0为手动 
//	static uint8_t start_flag = 0;   //运行下一步的标志位
//	static uint8_t R_PUSH_flag = 0;  //右伸的标志位
//	static uint8_t L_PUSH_flag = 0;  //左伸的标志位
//	static uint8_t BOTH_HANDOVER_flag = 0;
//	static uint8_t X_flag = 0;

	
//	static uint8_t Axis7_flag = 0;  //消抖需要
//	static uint8_t Axis6_flag = 0;
//	
	if(PC_DataHeadCorrect != PC_USART_Rx_DMA_BufferSize)//说明是在进行补偿操作，纠正数据头位置
	{	
		PC_DataHeadCorrect = PC_USART_Rx_DMA_BufferSize;
		
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
				PC_DataHeadCorrect = temp;//下一次接收这么多位就可以消除数据错位的现象
			}
			
			if(0 >= PC_DataHeadCorrect)		//避免出现校准值为零即接收数据长度为零的情况(为负的情况是由内存数据对齐导致的)
			{							//如果是零，那么DMA就不会继续传输数据了
				PC_DataHeadCorrect = PC_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//若满足校验条件，则刷入数据
		{
			KEY1_value=PC_RxBuffer[0].buffer.key_flag;
			KEY2_value=PC_RxBuffer[0].buffer.key_flag2;
			Rotate_value=PC_RxBuffer[0].buffer.rotate;
			caculed_ward=PC_RxBuffer[0].buffer.ward;	
		}
		
		LastBuffersize = PC_DataHeadCorrect;
	}
}

 
/**
  * @brief  向F103发送数据
  * @param  无
  * @retval 无
  */
void SendDataToPC(void)
{
	uint8_t i;
	int32_t temp = 0;
	
	PC_Tx_Buffer.TxBuffer.Data_head1 = 0xaa;
	PC_Tx_Buffer.TxBuffer.Data_head2 = 0xbb;

	for(i = 0; i < (PC_USART_Tx_DMA_BufferSize - 4); i++)	//计算校验
	{
		temp += PC_Tx_Buffer.Bytes[i];
	}
	
	PC_Tx_Buffer.TxBuffer.verify = temp;
	
	SendMsg_byDMA(PC_USART_Tx_DMA_Stream, PC_USART_Tx_DMA_BufferSize);									//发送数据
}

//void Robot_State(uint8_t SpeedUp, int8_t *ADC_Gather)
//{
//	static uint32_t cnt = 0;
//	float Robot_Speed_X;
//	float Robot_Speed_Y;
//	float Robot_Speed;
//	float Robot_Direction_Angle;
//	
//	/*********************机器人速度计算*********************/
//		/*---------------计算x轴速度-------------------*/
//				if(ADC_Gather[0]>=0)
//				{
//					Robot_Speed_X=16.11f*ADC_Gather[0];
//				}
//				else
//				{
//					Robot_Speed_X=16.11f*ADC_Gather[0];
//				}
//		/*---------------计算Y轴速度-------------------*/
//				if(ADC_Gather[1]>=0)
//				{
//					Robot_Speed_Y=16.11f*ADC_Gather[1];
//				}
//				else
//				{
//					Robot_Speed_Y=16.11f*ADC_Gather[1];
//				}
//		/*---------------计算合速度-------------------*/
//				Robot_Speed=sqrtf(Robot_Speed_X*Robot_Speed_X+Robot_Speed_Y*Robot_Speed_Y);
//				if(Robot_Speed>2894){Robot_Speed=2894;}
//				
//				if(Robot_Speed_Y>=0)//速度为正，方向向前
//				{
//					if(SpeedUp == 0)
//					{
//					  if(Robot_Speed < 700)
//				      sendbuffer.RobotSpeed = 0;   //赋值给结构体变量  
//					  else if(Robot_Speed < 826)
//						  sendbuffer.RobotSpeed = 60;
//						else if(Robot_Speed < 1239)
//							sendbuffer.RobotSpeed = 100;
//						else if(Robot_Speed < 1652)
//							sendbuffer.RobotSpeed = 150;
//						else if(Robot_Speed < 2065)
//							sendbuffer.RobotSpeed = 200;
//						else if(Robot_Speed < 2478)
//							sendbuffer.RobotSpeed = 240;
//						else
//							sendbuffer.RobotSpeed = 280;
//					}
//					
//					else
//					{
//						if(Robot_Speed < 700)
//				      sendbuffer.RobotSpeed = 0;   //赋值给结构体变量  
//					  else if(Robot_Speed < 1158)
//						  sendbuffer.RobotSpeed = 500;
//						else if(Robot_Speed < 1737)
//							sendbuffer.RobotSpeed = 800;
//						else if(Robot_Speed < 2316)
//							sendbuffer.RobotSpeed = 1100;
//						else
//							sendbuffer.RobotSpeed = 1400;
//					}
//				}
//				
//				//Y轴为负方向
//				else
//				{
//					if(SpeedUp == 0)
//					{
//					  if(Robot_Speed < 700)
//				      sendbuffer.RobotSpeed = 0;   //赋值给结构体变量  
//					  else if(Robot_Speed < 826)
//						  sendbuffer.RobotSpeed = -60;
//						else if(Robot_Speed < 1239)
//							sendbuffer.RobotSpeed = -100;
//						else if(Robot_Speed < 1652)
//							sendbuffer.RobotSpeed = -150;
//						else if(Robot_Speed < 2065)
//							sendbuffer.RobotSpeed = -200;
//						else if(Robot_Speed < 2478)
//							sendbuffer.RobotSpeed = -240;
//						else
//							sendbuffer.RobotSpeed = -280;
//					}
//					else
//					{
//						if(Robot_Speed < 700)
//				      sendbuffer.RobotSpeed = 0;   //赋值给结构体变量  
//					  else if(Robot_Speed < 1158)
//						  sendbuffer.RobotSpeed = -500;
//						else if(Robot_Speed < 1737)
//							sendbuffer.RobotSpeed = -800;
//						else if(Robot_Speed < 2316)
//							sendbuffer.RobotSpeed = -1100;
//						else
//							sendbuffer.RobotSpeed = -1400;
//					}
//				}
//				
//				/*********************机器人角度计算*********************/
//				if(Robot_Speed_X == 0)
//				{
//					sendbuffer.RobotWard = 90;
//				}
//				else
//				{
//				  Robot_Direction_Angle=atan2f(Robot_Speed_Y,Robot_Speed_X);//三角计算
//					
//					if(Robot_Direction_Angle<0.-2.512)
//						sendbuffer.RobotWard = 0;
//					else if(Robot_Direction_Angle<-1.884)
//					  sendbuffer.RobotWard = 45;
//					else if(Robot_Direction_Angle<-1.256)
//					  sendbuffer.RobotWard = 90;
//					else if(Robot_Direction_Angle<-0.628)
//					  sendbuffer.RobotWard = 135;
//					else if(Robot_Direction_Angle<0)
//					  sendbuffer.RobotWard = 180;
//					else if(Robot_Direction_Angle<0.628)
//						sendbuffer.RobotWard = 0;
//					else if(Robot_Direction_Angle<1.256)
//						sendbuffer.RobotWard = 45;
//					else if(Robot_Direction_Angle<1.884)
//						sendbuffer.RobotWard = 90;
//					else if(Robot_Direction_Angle<2.512)
//						sendbuffer.RobotWard = 135;
//					else sendbuffer.RobotWard = 180;
//				}
//				
//				/******************姿态角计算*******************/
//				if(cnt%20 == 1)
//				{
//				  if(SpeedUp == 0)
//				  {
//				    if(ADC_Gather[2] < -96)
//					    sendbuffer.RockerPosAngel -= 2;
//            else if(ADC_Gather[2] < -69)
//              sendbuffer.RockerPosAngel -= 1;
//            else if(ADC_Gather[2] < 48)
//              sendbuffer.RockerPosAngel -= 0;
//            else if(ADC_Gather[2] < 85)
//              sendbuffer.RockerPosAngel += 1;
//            else sendbuffer.RockerPosAngel += 2;
//				  }
//				  else
//				  {
//				    if(ADC_Gather[2] < -85)
//					    sendbuffer.RockerPosAngel -= 6;
//            else if(ADC_Gather[2] < -48)
//              sendbuffer.RockerPosAngel -= 4;
//            else if(ADC_Gather[2] < 48)
//              sendbuffer.RockerPosAngel -= 0;
//            else if(ADC_Gather[2] < 85)
//              sendbuffer.RockerPosAngel += 4;
//            else sendbuffer.RockerPosAngel += 6;
//				  }
//				
//				  if(sendbuffer.RockerPosAngel > 360)
//					  sendbuffer.RockerPosAngel = 360;
//				  else if(sendbuffer.RockerPosAngel < 0)
//					  sendbuffer.RockerPosAngel = 0;
//				}
//				cnt++;
//}

/**
  * @brief  对达到目的地所需要的时间进行判断
  * @param  无
  * @retval 无
  */




/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

