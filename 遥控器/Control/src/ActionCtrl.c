#include "InterfaceToEquips.h"
#include "ActionCtrl.h"

SpinPositionType SpinState = Spin_Null;
enum SpecialActions Act_flag = NullAction;
uint8_t ActCylinders = Null_Cylinders;
//										Left	Right	Spin
const uint8_t finishFlags[DevNum] = {	0x01, 	0x02, 	0x04};
bool isReady[DevNum] = {false};

bool ActACK = false;		//指令应答 -- 标志着辅控行为指令已接收到(未动作)
bool isActFinishied = false;//动作反馈 -- 标志着辅控已将动作完成(执行到位)

bool isWaitingforACK = false;
uint32_t SendCnt = 0;		//发送次数计数器

/**
  * @brief  给出标志应答
  * @param  None
  * @retval None
  */
void GiveACK(void)
{
	while(Eq_isSendingflag);			//等待发送结束
	
	Eq_Tx_Buffer.TxBuffer.Ack = 0x01;
}

void flagCheck(void)
{
	if(isActFinishied)
	{
		GiveACK();
	}
}

/**
  * @brief  清空动作标志
  * @param  None
  * @retval None
  */
void CleanActionFlags(void)
{
	Eq_Tx_Buffer.TxBuffer.CylinderAction = NullAction;
//	Eq_Tx_Buffer.TxBuffer.CylinderGroups = Null_Cylinders;
	Eq_Tx_Buffer.TxBuffer.DoAction = 0x00;						//清空动作标志
	Eq_Tx_Buffer.TxBuffer.Ack = 0;
}

/**
  * @brief  扫描发送状态
  * @param  None
  * @retval None
  */
void SendStateScan(void)
{
	if(isWaitingforACK)
	{
		SendCnt++;
	}
}

/**
  * @brief  等待动作完成
  * @param  flag:待检测的标志
  * @retval 若为false，则说明响应超时
  */
static bool WaitforActFinished(bool &flag)
{
	while(!flag);
	
	flag = 0x00;
	
	return true;
}

/**
  * @brief  更改气缸气压
  * @param  AirP_State:气缸气压状态，参数 @AirPressureStatetype
  * @retval None
  */
bool SetAirPressure(enum AirPressureStatetype AirP_State)
{
	bool isSuccessed = true;
	SendCnt = 0;
	
	Eq_Tx_Buffer.TxBuffer.SetAirPressure = AirP_State;
	
	return isSuccessed;
}

 /**
  * @brief  控制旋转装置
  * @param  Position:旋转装置所处的位置，参数 @SpinPositionType
  * @retval 无
  */
bool SpinDeviceCtrl(SpinPositionType Position)
{
	bool isSuccessed = true;

	while(Eq_isSendingflag);			//等待发送结束
			
	Eq_Tx_Buffer.TxBuffer.SpinPosition = Position;
		
	return isSuccessed;

}

/**
 * @brief  机器人的特定动作集合
 * @param  ACT:机器人所要执行的特殊动作
 * @param  Cylinders:执行动作的气缸
 * @retval 无
 */
bool SpecialAct(enum SpecialActions &Act, uint8_t &Cylinders)
{
	static uint8_t SendTimes = 0;		//标记同样的指令的发送次数，相当于超时时间
	bool isSuccessed = true;
	SendCnt = 0;
	
	InitCylinderState();
//	ActACK = false;  
	
	isWaitingforACK = true;
	
//	Eq_Tx_Buffer.TxBuffer.CylinderGroups = Cylinders;
//	Eq_Tx_Buffer.TxBuffer.CylinderAction = Act;
	
	while(!ActACK)		//若无应答
	{
		if(SendCnt > 3)//若距离发送指令已经又发了三次数据，则再发送一次
		{
			SendCnt = 0;
			
			while(Eq_isSendingflag);			//等待发送结束
			
			Eq_Tx_Buffer.TxBuffer.CylinderGroups = Cylinders;
			Eq_Tx_Buffer.TxBuffer.CylinderAction = Act;
			
			if(SendTimes > 30)		//超时设置：如果这样的过程连续重复30次，就出问题了，需要做异常处理
			{
				SendTimes = 0;
				isSuccessed = false;	
				break;
			}
		}
	}
	isWaitingforACK = false;
	ActACK = false;
	
//	Act = NullAction;
//	Cylinders = Null_Cylinders;
	
	if(!isSuccessed)
	{
		return isSuccessed;
	}
	
	WaitforActFinished(isActFinishied);			//等待动作执行完成
	
	Act = NullAction;
	Cylinders = Null_Cylinders;
	
	return isSuccessed;
}

/**
 * @brief  灯条亮度控制
 * @param  Red:红色深度 0-255
 * @param  Green:绿色深度 0-255
 * @param  Blue:蓝色深度 0-255
 * @param  light:总体亮度 0-100
 * @retval 指令是否发送成功
 */
bool LightCtrl(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t light)
{
	bool isSuccessed = true;
			
	while(Eq_isSendingflag);			//等待发送结束
			
	Eq_Tx_Buffer.TxBuffer.Light.Red = Red;
	Eq_Tx_Buffer.TxBuffer.Light.Green = Green;
	Eq_Tx_Buffer.TxBuffer.Light.Blue = Blue;
	Eq_Tx_Buffer.TxBuffer.Light.Light = light;
	
	return isSuccessed;
}

/**
 * @brief  激光控制
 * @param  NewState:激光的新状态
 * @retval 无
 */
bool LaserCtrl(bool NewState)
{
	static uint8_t SendTimes = 0;		//标记同样的指令的发送次数，相当于超时时间
	bool isSuccessed = true;
	SendCnt = 0;
	
//	ActACK = false;
	
	isWaitingforACK = true;
	
	Eq_Tx_Buffer.TxBuffer.LaserRequest = NewState;
	
	while(!ActACK)		//若无应答
	{
		if(SendCnt > 3)//若距离发送指令已经又发了三次数据，则再发送一次
		{
			SendCnt = 0;
			
			while(Eq_isSendingflag);			//等待发送结束
			
			Eq_Tx_Buffer.TxBuffer.LaserRequest = NewState;
			
			if(SendTimes > 30)		//超时设置：如果这样的过程连续重复30次，就出问题了，需要做异常处理
			{
				SendTimes = 0;
				isSuccessed = false;	
				break;
			}
		}
	}
	isWaitingforACK = false;
	
	return isSuccessed;
}

/**
 * @brief  做机器人处于特定状态时的动作(一一对应,一个特定状态下只有一种特定动作)
 * @param  NewState:激光的新状态
 * @retval 动作执行是否成功
 */
bool DoActions(void)
{
	static uint8_t SendTimes = 0;		//标记同样的指令的发送次数，相当于超时时间
	bool isSuccessed = true;
	SendCnt = 0;
	
//	ActACK = false;
	
	isWaitingforACK = true;
	
	Eq_Tx_Buffer.TxBuffer.DoAction = 0x01;		//标记动作指令有效
	
	while(!ActACK)		//若无应答
	{
		if(SendCnt > 3)//若距离发送指令已经又发了三次数据，则再发送一次
		{
			SendCnt = 0;
			
			while(Eq_isSendingflag);			//等待发送结束
			
			Eq_Tx_Buffer.TxBuffer.DoAction = 0x01;		//标记动作指令有效
			
			if(SendTimes > 30)		//超时设置：如果这样的过程连续重复30次，就出问题了，需要做异常处理
			{
				SendTimes = 0;
				isSuccessed = false;	
				break;
			}
		}
	}

	isWaitingforACK = false;
	
	if(!isSuccessed)
	{
		return isSuccessed;
	}
	
	WaitforActFinished(isActFinishied);			//等待动作执行完成
	
	return isSuccessed;
}
