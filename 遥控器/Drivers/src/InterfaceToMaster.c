#include "InterfaceToMaster.h"
#include "Motion_Ctrl.h"
#include "ActionCtrl.h"
#include "Vol_curve.h"
#include "WatchDog.h"
#include "Process.h"
#include "USART.h"
#include "Base.h"
#include "Map.h"

/** @defgroup CMD Type
  * @{
  */

#define AIM_MOOD 		AimMode			//Ŀ�����ģʽ
#define OPEN_LOOP_MOOD  OpenLoopMode	//��������ģʽ
#define AimValid  		0x02			//Ŀ��ֵ��Ч
#define AngleValid  	0x04			//�Ƕ���Ч
#define PIDValid  		0x08			//PID������Ч
#define CylinderValid	0x10			//���׿���������Ч
#define NextPoint		0x20			//�˶�����һ��
#define SpinValid		0x40			//��תװ��ָ����Ч
#define BallflagValid	0x80			//�����־��Ч

/**
  * @}
  */

/** @defgroup CMD2 Type
  * @{
  */

#define NullCMD2		0x00
#define LoseWheelCtrl	0x01			//�����ֶ����������ģʽ

/**
  * @}
  */

/** @defgroup Ballflag Type
  * @{
  */

#define NoBallCastSuccessfully 		0x00	//��û�׽�һ����
#define CastNormBallSuccessfully1  	0x01	//��TZ1�׽�����ͨ��
#define CastNormBallSuccessfully2  	0x02	//��TZ2�׽�����ͨ��
#define CastGoldenBallSuccessfully 	0x03	//�׽��˽�ɫ��


/**
  * @}
  */

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//����޷�

/* Public  variables ---------------------------------------------------------*/
CMD_BufferType		CMDBuffer[2];									//˫���ջ�����
Msg_Tx_BufferType	Tx_Buffer;
uint8_t 			BufferFlag = 0;									//Ŀǰʹ�õĻ��������
uint8_t 			PID_flag = 0;									//1ΪPID������Ч��0Ϊ��Ч
int16_t 			DataHeadCorrect = BL_USART_Rx_DMA_BufferSize;	//����ͷλ�ý���ֵ

struct PIDPara 		PIDSetting;										//PID��������

 /**
  * @brief  �ӿ��ƶ˻�ȡ����
  * @param  ucData�����յ����ֽ�����
  * @retval ��
  */
void ReceiveDataFromMaster_Buffer(const CMD_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = BL_USART_Rx_DMA_BufferSize;
	
	if(DataHeadCorrect != BL_USART_Rx_DMA_BufferSize)//˵�����ڽ��в�����������������ͷλ��
	{	
		DataHeadCorrect = BL_USART_Rx_DMA_BufferSize;
		
		return;
	}
	else
	{
		for(i = 0; i < LastBuffersize - 4; i++)	//���У��
		{
			check += CMDData.Bytes[i];					//���У��
			
			if(0xaa == CMDData.Bytes[i])				//��������ͷλ��
			{
				temp = i;
			}
			if((0xbb == CMDData.Bytes[i]) || (temp == (i-1)))	//�����һλ��0xBB,��ǰһλ��0xAA��
			{													//��ô����ͷ�ڻ�������λ�þͿ���ȷ��
				
				DataHeadCorrect = temp;//��һ�ν�����ô��λ�Ϳ����������ݴ�λ������
			}
			
			if(0 >= DataHeadCorrect)	//�������У׼ֵΪ�㼴�������ݳ���Ϊ������(Ϊ������������ڴ����ݶ��뵼�µ�)
			{							//������㣬��ôDMA�Ͳ����������������
				DataHeadCorrect = BL_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//������У����������ˢ������
		{
			FeedDog(Master);
			Baseflags.InitSuccess = true;				//��ǳ�ʼ�����
			
			Base_CMD.R_Ctrl_Flag = CMDData.buffer.CMD & 0x01;
			
			Base_CMD.ward = CMDData.buffer.Ward;
			Base_CMD.line_speed = CMDData.buffer.LineSpeed;
			
			if(CMDData.buffer.CMD & AimValid)			//��Ŀ��ֵ��Ч
			{
				Base_CMD.aimx = CMDData.buffer.Aim_X;
				Base_CMD.aimy = CMDData.buffer.Aim_Y;
				Base_CMD.aimt = CMDData.buffer.Aim_t;
				
				//�����µ�Ŀ�����겢�����ٶ�����
				AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
			}
			
			if(CMDData.buffer.CMD & AngleValid)			//���Ƕ�ֵ��Ч
			{
				AimAngleUpdate(CMDData.buffer.Pos_Angle);
			}
			
			if(CMDData.buffer.CMD & CylinderValid)		//�����׿���ָ����Ч
			{
				Act_flag = (enum SpecialActions)CMDData.buffer.Action;
				ActCylinders = CMDData.buffer.Cylinders;
				ActACK = false;
			}
			
			if(CMDData.buffer.CMD & NextPoint)
			{
				Next = true;
				GotoThePoint = (enum MovePointsMarkType)CMDData.buffer.MovePoint;
			}
			
			if(CMDData.buffer.CMD & BallflagValid)
			{
				switch(CMDData.buffer.CastBallflag)
				{
					case CastGoldenBallSuccessfully:CastGoldenBallSuccessed = true;
					case CastNormBallSuccessfully2:	CastNormBallSuccessed_TZ2 = true;
					case CastNormBallSuccessfully1: CastNormBallSuccessed_TZ1 = true;
													break;
					
					default: break;
				}
			}
			
			if(CMDData.buffer.CMD & SpinValid)
			{
				SpinState = (SpinPositionType)CMDData.buffer.CMD2;
			}
			
			if(CMDData.buffer.CMD & PIDValid)			//��PID����ֵ��Ч
			{
				//����PID
				PIDSetting.Current.p = CMDData.buffer.Current_P;	
				PIDSetting.Current.i = CMDData.buffer.Current_I;	
				PIDSetting.Current.d = CMDData.buffer.Current_D;
				
				//�Ƕ�PID
				PIDSetting.Angle.p = CMDData.buffer.Angle_P;	
				PIDSetting.Angle.i = CMDData.buffer.Angle_I;	
				PIDSetting.Angle.d = CMDData.buffer.Angle_D;
				
				//λ��PID
				PIDSetting.Location.p = CMDData.buffer.Location_P;	
				PIDSetting.Location.i = CMDData.buffer.Location_I;	
				PIDSetting.Location.d = CMDData.buffer.Location_D;
				
				//ʱ��PID
				PIDSetting.Time.p = CMDData.buffer.Time_P;	
				PIDSetting.Time.i = CMDData.buffer.Time_I;	
				PIDSetting.Time.d = CMDData.buffer.Time_D;
				
				//�յ�PID
				PIDSetting.End.p = CMDData.buffer.END_P;	
				PIDSetting.End.i = CMDData.buffer.END_I;	
				PIDSetting.End.d = CMDData.buffer.END_D;
				
				//����PID���������PID������
				PIDparamUpdate(&pidData_Time, PIDSetting.Time.p, PIDSetting.Time.i, PIDSetting.Time.d,
								PID_TIME_I_MAX * 100, PID_TIME_OUT_MAX * 100);
								
				PIDparamUpdate(&pidData_End, PIDSetting.End.p, PIDSetting.End.i, PIDSetting.End.d,
								PID_END_I_MAX * 100, PID_END_OUT_MAX * 100);
								
				PIDparamUpdate(&pidData_Location, PIDSetting.Location.p, PIDSetting.Location.i, PIDSetting.Location.d,
								PID_Location_I_MAX * 100, PID_Location_OUT_MAX * 100);
								
				//���PID������Ч
				PID_flag = 0x01;
				
				//��ǽ��ճɹ�
				Baseflags.RcvPIDSuccess = true;
			}
			
		}
		LastBuffersize = DataHeadCorrect;
	}
}

 
 /**
  * @brief  ����λ����������
  * @param  None
  * @retval None
  */
void SendDataToMaster(void)
{
	uint8_t i;
	int32_t temp = 0;
	
	Tx_Buffer.TxBuffer.Data_head1 = 0xaa;
	Tx_Buffer.TxBuffer.Data_head2 = 0xbb;
	
	Tx_Buffer.TxBuffer.Aim_Angle = Base_CMD.Pos_Angle;
	Tx_Buffer.TxBuffer.Real_Angle = Coordinate.Angle;
	
//	Tx_Buffer.TxBuffer.Aim_Displace = AimSpeed.v1;
//	Tx_Buffer.TxBuffer.Real_Displace = AimSpeed.v2;
//	Tx_Buffer.TxBuffer.Pos_Error = AimSpeed.v3;
	
	Tx_Buffer.TxBuffer.Wheel_Speed1 = RealSpeed.v1;
	Tx_Buffer.TxBuffer.Wheel_Speed2 = RealSpeed.v2;
	Tx_Buffer.TxBuffer.Wheel_Speed3 = RealSpeed.v3;
	
	Tx_Buffer.TxBuffer.OutSpeed = Motion.outSpeed;
	
	Tx_Buffer.TxBuffer.Real_X = Coordinate.point.X;
	Tx_Buffer.TxBuffer.Real_Y = Coordinate.point.Y;
	
	Tx_Buffer.TxBuffer.Real_t = Vol_Curve.t;
	
	Tx_Buffer.TxBuffer.flags.CtrlMode = Base_CMD.R_Ctrl_Flag;
	Tx_Buffer.TxBuffer.flags.Endflag = Motion.isENDLoop;
	Tx_Buffer.TxBuffer.flags.MotionOverflow = Vol_Curve.MotionOverflowflag;
	
	
	for(i = 0; i < (BL_USART_Tx_DMA_BufferSize - 4); i++)	//����У��
	{
		temp += Tx_Buffer.Bytes[i];
	}
	
	Tx_Buffer.TxBuffer.verify = temp;
	
	SendMsg_byDMA(BL_USART_Tx_DMA_Stream, BL_USART_Tx_DMA_BufferSize);	//��������
}



/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
