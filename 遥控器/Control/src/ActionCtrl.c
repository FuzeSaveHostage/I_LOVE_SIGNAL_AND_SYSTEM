#include "InterfaceToEquips.h"
#include "ActionCtrl.h"

SpinPositionType SpinState = Spin_Null;
enum SpecialActions Act_flag = NullAction;
uint8_t ActCylinders = Null_Cylinders;
//										Left	Right	Spin
const uint8_t finishFlags[DevNum] = {	0x01, 	0x02, 	0x04};
bool isReady[DevNum] = {false};

bool ActACK = false;		//ָ��Ӧ�� -- ��־�Ÿ�����Ϊָ���ѽ��յ�(δ����)
bool isActFinishied = false;//�������� -- ��־�Ÿ����ѽ��������(ִ�е�λ)

bool isWaitingforACK = false;
uint32_t SendCnt = 0;		//���ʹ���������

/**
  * @brief  ������־Ӧ��
  * @param  None
  * @retval None
  */
void GiveACK(void)
{
	while(Eq_isSendingflag);			//�ȴ����ͽ���
	
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
  * @brief  ��ն�����־
  * @param  None
  * @retval None
  */
void CleanActionFlags(void)
{
	Eq_Tx_Buffer.TxBuffer.CylinderAction = NullAction;
//	Eq_Tx_Buffer.TxBuffer.CylinderGroups = Null_Cylinders;
	Eq_Tx_Buffer.TxBuffer.DoAction = 0x00;						//��ն�����־
	Eq_Tx_Buffer.TxBuffer.Ack = 0;
}

/**
  * @brief  ɨ�跢��״̬
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
  * @brief  �ȴ��������
  * @param  flag:�����ı�־
  * @retval ��Ϊfalse����˵����Ӧ��ʱ
  */
static bool WaitforActFinished(bool &flag)
{
	while(!flag);
	
	flag = 0x00;
	
	return true;
}

/**
  * @brief  ����������ѹ
  * @param  AirP_State:������ѹ״̬������ @AirPressureStatetype
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
  * @brief  ������תװ��
  * @param  Position:��תװ��������λ�ã����� @SpinPositionType
  * @retval ��
  */
bool SpinDeviceCtrl(SpinPositionType Position)
{
	bool isSuccessed = true;

	while(Eq_isSendingflag);			//�ȴ����ͽ���
			
	Eq_Tx_Buffer.TxBuffer.SpinPosition = Position;
		
	return isSuccessed;

}

/**
 * @brief  �����˵��ض���������
 * @param  ACT:��������Ҫִ�е����⶯��
 * @param  Cylinders:ִ�ж���������
 * @retval ��
 */
bool SpecialAct(enum SpecialActions &Act, uint8_t &Cylinders)
{
	static uint8_t SendTimes = 0;		//���ͬ����ָ��ķ��ʹ������൱�ڳ�ʱʱ��
	bool isSuccessed = true;
	SendCnt = 0;
	
	InitCylinderState();
//	ActACK = false;  
	
	isWaitingforACK = true;
	
//	Eq_Tx_Buffer.TxBuffer.CylinderGroups = Cylinders;
//	Eq_Tx_Buffer.TxBuffer.CylinderAction = Act;
	
	while(!ActACK)		//����Ӧ��
	{
		if(SendCnt > 3)//�����뷢��ָ���Ѿ��ַ����������ݣ����ٷ���һ��
		{
			SendCnt = 0;
			
			while(Eq_isSendingflag);			//�ȴ����ͽ���
			
			Eq_Tx_Buffer.TxBuffer.CylinderGroups = Cylinders;
			Eq_Tx_Buffer.TxBuffer.CylinderAction = Act;
			
			if(SendTimes > 30)		//��ʱ���ã���������Ĺ��������ظ�30�Σ��ͳ������ˣ���Ҫ���쳣����
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
	
	WaitforActFinished(isActFinishied);			//�ȴ�����ִ�����
	
	Act = NullAction;
	Cylinders = Null_Cylinders;
	
	return isSuccessed;
}

/**
 * @brief  �������ȿ���
 * @param  Red:��ɫ��� 0-255
 * @param  Green:��ɫ��� 0-255
 * @param  Blue:��ɫ��� 0-255
 * @param  light:�������� 0-100
 * @retval ָ���Ƿ��ͳɹ�
 */
bool LightCtrl(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t light)
{
	bool isSuccessed = true;
			
	while(Eq_isSendingflag);			//�ȴ����ͽ���
			
	Eq_Tx_Buffer.TxBuffer.Light.Red = Red;
	Eq_Tx_Buffer.TxBuffer.Light.Green = Green;
	Eq_Tx_Buffer.TxBuffer.Light.Blue = Blue;
	Eq_Tx_Buffer.TxBuffer.Light.Light = light;
	
	return isSuccessed;
}

/**
 * @brief  �������
 * @param  NewState:�������״̬
 * @retval ��
 */
bool LaserCtrl(bool NewState)
{
	static uint8_t SendTimes = 0;		//���ͬ����ָ��ķ��ʹ������൱�ڳ�ʱʱ��
	bool isSuccessed = true;
	SendCnt = 0;
	
//	ActACK = false;
	
	isWaitingforACK = true;
	
	Eq_Tx_Buffer.TxBuffer.LaserRequest = NewState;
	
	while(!ActACK)		//����Ӧ��
	{
		if(SendCnt > 3)//�����뷢��ָ���Ѿ��ַ����������ݣ����ٷ���һ��
		{
			SendCnt = 0;
			
			while(Eq_isSendingflag);			//�ȴ����ͽ���
			
			Eq_Tx_Buffer.TxBuffer.LaserRequest = NewState;
			
			if(SendTimes > 30)		//��ʱ���ã���������Ĺ��������ظ�30�Σ��ͳ������ˣ���Ҫ���쳣����
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
 * @brief  �������˴����ض�״̬ʱ�Ķ���(һһ��Ӧ,һ���ض�״̬��ֻ��һ���ض�����)
 * @param  NewState:�������״̬
 * @retval ����ִ���Ƿ�ɹ�
 */
bool DoActions(void)
{
	static uint8_t SendTimes = 0;		//���ͬ����ָ��ķ��ʹ������൱�ڳ�ʱʱ��
	bool isSuccessed = true;
	SendCnt = 0;
	
//	ActACK = false;
	
	isWaitingforACK = true;
	
	Eq_Tx_Buffer.TxBuffer.DoAction = 0x01;		//��Ƕ���ָ����Ч
	
	while(!ActACK)		//����Ӧ��
	{
		if(SendCnt > 3)//�����뷢��ָ���Ѿ��ַ����������ݣ����ٷ���һ��
		{
			SendCnt = 0;
			
			while(Eq_isSendingflag);			//�ȴ����ͽ���
			
			Eq_Tx_Buffer.TxBuffer.DoAction = 0x01;		//��Ƕ���ָ����Ч
			
			if(SendTimes > 30)		//��ʱ���ã���������Ĺ��������ظ�30�Σ��ͳ������ˣ���Ҫ���쳣����
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
	
	WaitforActFinished(isActFinishied);			//�ȴ�����ִ�����
	
	return isSuccessed;
}
