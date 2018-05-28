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
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//����޷�

/* Public  variables ----------------------*/
PC_Rx_BufferType		PC_RxBuffer[2];									//˫���ջ�����
PC_Msg_Tx_BufferType	PC_Tx_Buffer;
uint8_t				 	PC_BufferFlag = 0;									//Ŀǰʹ�õĻ��������
int16_t 				PC_DataHeadCorrect = PC_USART_Rx_DMA_BufferSize;		//����ͷλ�ý���ֵ

uint8_t KEY1_value;
uint8_t KEY2_value;
uint8_t Rotate_value;   //�Ƕ����ֵ
uint8_t caculed_ward;  //�����˶��Ƕ�

/* Private  variables ---------------------------------------------------------*/
uint8_t RcvPC_Ready = 0;							//���ճɹ���־����ȡ���ݺ�λ����λǰ����������

SendBuffer sendbuffer;       //����һ���������洢�����ٶȡ����򡢽Ƕ���������
int8_t Axis[3];              //�õ�������ҡ�˱���

 
/**
  * @brief  �Ӵ������ݻ������ж�ȡ����
  * @param  ��
  * @retval ��
  */
void ReceiveDataFromPC_Buffer(const PC_Rx_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = PC_USART_Rx_DMA_BufferSize;
	
//	static uint8_t key = 0,key2 = 0;   //��¼�ϴ�����У��ʱ����������ֵ
//	static uint8_t speed_flag = 0;  //0Ĭ��Ϊ����
//	static uint8_t manual_auto = 0;  //�ֶ����Զ��л���־��0Ϊ�ֶ� 
//	static uint8_t start_flag = 0;   //������һ���ı�־λ
//	static uint8_t R_PUSH_flag = 0;  //����ı�־λ
//	static uint8_t L_PUSH_flag = 0;  //����ı�־λ
//	static uint8_t BOTH_HANDOVER_flag = 0;
//	static uint8_t X_flag = 0;

	
//	static uint8_t Axis7_flag = 0;  //������Ҫ
//	static uint8_t Axis6_flag = 0;
//	
	if(PC_DataHeadCorrect != PC_USART_Rx_DMA_BufferSize)//˵�����ڽ��в�����������������ͷλ��
	{	
		PC_DataHeadCorrect = PC_USART_Rx_DMA_BufferSize;
		
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
				PC_DataHeadCorrect = temp;//��һ�ν�����ô��λ�Ϳ����������ݴ�λ������
			}
			
			if(0 >= PC_DataHeadCorrect)		//�������У׼ֵΪ�㼴�������ݳ���Ϊ������(Ϊ������������ڴ����ݶ��뵼�µ�)
			{							//������㣬��ôDMA�Ͳ����������������
				PC_DataHeadCorrect = PC_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//������У����������ˢ������
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
  * @brief  ��F103��������
  * @param  ��
  * @retval ��
  */
void SendDataToPC(void)
{
	uint8_t i;
	int32_t temp = 0;
	
	PC_Tx_Buffer.TxBuffer.Data_head1 = 0xaa;
	PC_Tx_Buffer.TxBuffer.Data_head2 = 0xbb;

	for(i = 0; i < (PC_USART_Tx_DMA_BufferSize - 4); i++)	//����У��
	{
		temp += PC_Tx_Buffer.Bytes[i];
	}
	
	PC_Tx_Buffer.TxBuffer.verify = temp;
	
	SendMsg_byDMA(PC_USART_Tx_DMA_Stream, PC_USART_Tx_DMA_BufferSize);									//��������
}

//void Robot_State(uint8_t SpeedUp, int8_t *ADC_Gather)
//{
//	static uint32_t cnt = 0;
//	float Robot_Speed_X;
//	float Robot_Speed_Y;
//	float Robot_Speed;
//	float Robot_Direction_Angle;
//	
//	/*********************�������ٶȼ���*********************/
//		/*---------------����x���ٶ�-------------------*/
//				if(ADC_Gather[0]>=0)
//				{
//					Robot_Speed_X=16.11f*ADC_Gather[0];
//				}
//				else
//				{
//					Robot_Speed_X=16.11f*ADC_Gather[0];
//				}
//		/*---------------����Y���ٶ�-------------------*/
//				if(ADC_Gather[1]>=0)
//				{
//					Robot_Speed_Y=16.11f*ADC_Gather[1];
//				}
//				else
//				{
//					Robot_Speed_Y=16.11f*ADC_Gather[1];
//				}
//		/*---------------������ٶ�-------------------*/
//				Robot_Speed=sqrtf(Robot_Speed_X*Robot_Speed_X+Robot_Speed_Y*Robot_Speed_Y);
//				if(Robot_Speed>2894){Robot_Speed=2894;}
//				
//				if(Robot_Speed_Y>=0)//�ٶ�Ϊ����������ǰ
//				{
//					if(SpeedUp == 0)
//					{
//					  if(Robot_Speed < 700)
//				      sendbuffer.RobotSpeed = 0;   //��ֵ���ṹ�����  
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
//				      sendbuffer.RobotSpeed = 0;   //��ֵ���ṹ�����  
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
//				//Y��Ϊ������
//				else
//				{
//					if(SpeedUp == 0)
//					{
//					  if(Robot_Speed < 700)
//				      sendbuffer.RobotSpeed = 0;   //��ֵ���ṹ�����  
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
//				      sendbuffer.RobotSpeed = 0;   //��ֵ���ṹ�����  
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
//				/*********************�����˽Ƕȼ���*********************/
//				if(Robot_Speed_X == 0)
//				{
//					sendbuffer.RobotWard = 90;
//				}
//				else
//				{
//				  Robot_Direction_Angle=atan2f(Robot_Speed_Y,Robot_Speed_X);//���Ǽ���
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
//				/******************��̬�Ǽ���*******************/
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
  * @brief  �ԴﵽĿ�ĵ�����Ҫ��ʱ������ж�
  * @param  ��
  * @retval ��
  */




/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

