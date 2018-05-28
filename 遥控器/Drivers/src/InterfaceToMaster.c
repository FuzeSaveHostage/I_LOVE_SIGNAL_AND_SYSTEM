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

#define AIM_MOOD 		AimMode			//目标控制模式
#define OPEN_LOOP_MOOD  OpenLoopMode	//开环控制模式
#define AimValid  		0x02			//目标值有效
#define AngleValid  	0x04			//角度有效
#define PIDValid  		0x08			//PID参量有效
#define CylinderValid	0x10			//气缸控制数据有效
#define NextPoint		0x20			//运动到下一点
#define SpinValid		0x40			//旋转装置指令有效
#define BallflagValid	0x80			//抛球标志有效

/**
  * @}
  */

/** @defgroup CMD2 Type
  * @{
  */

#define NullCMD2		0x00
#define LoseWheelCtrl	0x01			//进入手动测试坐标的模式

/**
  * @}
  */

/** @defgroup Ballflag Type
  * @{
  */

#define NoBallCastSuccessfully 		0x00	//还没抛进一个球
#define CastNormBallSuccessfully1  	0x01	//在TZ1抛进了普通球
#define CastNormBallSuccessfully2  	0x02	//在TZ2抛进了普通球
#define CastGoldenBallSuccessfully 	0x03	//抛进了金色球


/**
  * @}
  */

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))	//输出限幅

/* Public  variables ---------------------------------------------------------*/
CMD_BufferType		CMDBuffer[2];									//双接收缓冲区
Msg_Tx_BufferType	Tx_Buffer;
uint8_t 			BufferFlag = 0;									//目前使用的缓冲区标号
uint8_t 			PID_flag = 0;									//1为PID数据有效，0为无效
int16_t 			DataHeadCorrect = BL_USART_Rx_DMA_BufferSize;	//数据头位置矫正值

struct PIDPara 		PIDSetting;										//PID数据内容

 /**
  * @brief  从控制端获取数据
  * @param  ucData：接收到的字节数据
  * @retval 无
  */
void ReceiveDataFromMaster_Buffer(const CMD_BufferType CMDData)
{
	char i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = BL_USART_Rx_DMA_BufferSize;
	
	if(DataHeadCorrect != BL_USART_Rx_DMA_BufferSize)//说明是在进行补偿操作，纠正数据头位置
	{	
		DataHeadCorrect = BL_USART_Rx_DMA_BufferSize;
		
		return;
	}
	else
	{
		for(i = 0; i < LastBuffersize - 4; i++)	//求和校验
		{
			check += CMDData.Bytes[i];					//求和校验
			
			if(0xaa == CMDData.Bytes[i])				//检索数据头位置
			{
				temp = i;
			}
			if((0xbb == CMDData.Bytes[i]) || (temp == (i-1)))	//如果这一位是0xBB,且前一位是0xAA，
			{													//那么数据头在缓冲区的位置就可以确定
				
				DataHeadCorrect = temp;//下一次接收这么多位就可以消除数据错位的现象
			}
			
			if(0 >= DataHeadCorrect)	//避免出现校准值为零即接收数据长度为零的情况(为负的情况是由内存数据对齐导致的)
			{							//如果是零，那么DMA就不会继续传输数据了
				DataHeadCorrect = BL_USART_Rx_DMA_BufferSize;
			}

		}
		
		if(CMDData.buffer.verify == check)				//若满足校验条件，则刷入数据
		{
			FeedDog(Master);
			Baseflags.InitSuccess = true;				//标记初始化完成
			
			Base_CMD.R_Ctrl_Flag = CMDData.buffer.CMD & 0x01;
			
			Base_CMD.ward = CMDData.buffer.Ward;
			Base_CMD.line_speed = CMDData.buffer.LineSpeed;
			
			if(CMDData.buffer.CMD & AimValid)			//若目标值有效
			{
				Base_CMD.aimx = CMDData.buffer.Aim_X;
				Base_CMD.aimy = CMDData.buffer.Aim_Y;
				Base_CMD.aimt = CMDData.buffer.Aim_t;
				
				//设置新的目标坐标并生成速度曲线
				AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
			}
			
			if(CMDData.buffer.CMD & AngleValid)			//若角度值有效
			{
				AimAngleUpdate(CMDData.buffer.Pos_Angle);
			}
			
			if(CMDData.buffer.CMD & CylinderValid)		//若气缸控制指令有效
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
			
			if(CMDData.buffer.CMD & PIDValid)			//若PID参量值有效
			{
				//电流PID
				PIDSetting.Current.p = CMDData.buffer.Current_P;	
				PIDSetting.Current.i = CMDData.buffer.Current_I;	
				PIDSetting.Current.d = CMDData.buffer.Current_D;
				
				//角度PID
				PIDSetting.Angle.p = CMDData.buffer.Angle_P;	
				PIDSetting.Angle.i = CMDData.buffer.Angle_I;	
				PIDSetting.Angle.d = CMDData.buffer.Angle_D;
				
				//位置PID
				PIDSetting.Location.p = CMDData.buffer.Location_P;	
				PIDSetting.Location.i = CMDData.buffer.Location_I;	
				PIDSetting.Location.d = CMDData.buffer.Location_D;
				
				//时间PID
				PIDSetting.Time.p = CMDData.buffer.Time_P;	
				PIDSetting.Time.i = CMDData.buffer.Time_I;	
				PIDSetting.Time.d = CMDData.buffer.Time_D;
				
				//终点PID
				PIDSetting.End.p = CMDData.buffer.END_P;	
				PIDSetting.End.i = CMDData.buffer.END_I;	
				PIDSetting.End.d = CMDData.buffer.END_D;
				
				//更新PID参数并清空PID运算量
				PIDparamUpdate(&pidData_Time, PIDSetting.Time.p, PIDSetting.Time.i, PIDSetting.Time.d,
								PID_TIME_I_MAX * 100, PID_TIME_OUT_MAX * 100);
								
				PIDparamUpdate(&pidData_End, PIDSetting.End.p, PIDSetting.End.i, PIDSetting.End.d,
								PID_END_I_MAX * 100, PID_END_OUT_MAX * 100);
								
				PIDparamUpdate(&pidData_Location, PIDSetting.Location.p, PIDSetting.Location.i, PIDSetting.Location.d,
								PID_Location_I_MAX * 100, PID_Location_OUT_MAX * 100);
								
				//标记PID参量有效
				PID_flag = 0x01;
				
				//标记接收成功
				Baseflags.RcvPIDSuccess = true;
			}
			
		}
		LastBuffersize = DataHeadCorrect;
	}
}

 
 /**
  * @brief  向上位机发送数据
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
	
	
	for(i = 0; i < (BL_USART_Tx_DMA_BufferSize - 4); i++)	//计算校验
	{
		temp += Tx_Buffer.Bytes[i];
	}
	
	Tx_Buffer.TxBuffer.verify = temp;
	
	SendMsg_byDMA(BL_USART_Tx_DMA_Stream, BL_USART_Tx_DMA_BufferSize);	//发送数据
}



/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
