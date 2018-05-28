#include "PID.h"

 /**
  * @brief  ����޷�(�����ڵ��������޷����������ڸ�С���ֵ����㣬����PID������PID�����)
  * @param  int32_t amt���������
	*					int32_t low�������Сֵ
	*					int32_t high��������ֵ
  * @retval int32_t X���޷���������
  */
int32_t constrain_int32(int32_t amt, int32_t low, int32_t high) 
{
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}


 /**
  * @brief  PID�������£�����PID�������PWM����Ϊ100��1��->��Ϊ�˱��⸡�����㣬�������Ч�ʺ�׼ȷ�ԣ�
  * @param  struct PID* pidData������ʼ����PID�ṹ�����
	*					uint16_t kp��P����
	*					uint16_t ki��I����
	*					uint16_t kd��D����
	*					int32_t imax�������޷�
	*					int32_t outmax������޷�
  * @retval ��
  */
void PIDparamUpdate(struct PID* pidData,uint16_t kp,uint16_t ki,uint16_t kd,int32_t imax,int32_t outmax)
{
	pidData->Imax=imax;
	pidData->outmax=outmax;
	pidData->kP=kp;
	pidData->kI=ki;
	pidData->kD=kd;
	pidData->P=0;
	pidData->I=0;
	pidData->D=0;
	pidData->error=0;
	pidData->last_error=0;
	pidData->derror=0;
	pidData->set=0;
	pidData->out=0;
}

 /**
  * @brief  PID��������
  * @param  struct PID* pidData����������PID�ṹ�����
  * @retval ��
  */
void PIDdataUpdate(struct PID* pidData)
{
	pidData->error = pidData->set - pidData->feedback;
	pidData->derror = pidData->error - pidData->last_error;
	pidData->last_error = pidData->error;
	
	pidData->P = pidData->kP * pidData->error;
	
	pidData->I += pidData->kI * pidData->error;
	pidData->I = constrain_int32(pidData->I, -pidData->Imax, +pidData->Imax);	
	
	pidData->D = pidData->kD * pidData->derror;
	
}

 /**
  * @brief  ��ջ���ֵ
  * @param  struct PID* pidData����������PID�ṹ�����
  * @retval ��
  */
void ResetI(struct PID* pidData)
{
	pidData->I = 0;
}

 /**
  * @brief  ���P������
  * @param  struct PID* pidData����������PID�ṹ�����
  * @retval ��
  */
void GetP(struct PID* pidData)
{
	pidData->P = pidData->kP * pidData->error;
}

 /**
  * @brief  ���I������
  * @param  struct PID* pidData����������PID�ṹ�����
  * @retval ��
  */
void GetI(struct PID* pidData)
{
	pidData->I += pidData->kI * pidData->error;
	pidData->I = constrain_int32(pidData->I, -pidData->Imax, +pidData->Imax);	
}

 /**
  * @brief  ���D������
  * @param  struct PID* pidData����������PID�ṹ�����
  * @retval ��
  */
void GetD(struct PID* pidData)
{
	pidData->D = pidData->kD * pidData->derror;
}

 /**
  * @brief  ���P��I�������PI���ƣ�
  * @param  struct PID* pidData����������PID�ṹ�����
  * @retval ��
  */
void GetPI(struct PID* pidData)
{
	pidData->out = pidData->P + pidData->I;
	pidData->out = constrain_int32(pidData->out, -pidData->outmax, +pidData->outmax)/100;	
}

 /**
  * @brief  ���P��I��D�������PID���ƣ�
  * @param  struct PID* pidData����������PID�ṹ�����
  * @retval ��
  */
void GetPID(struct PID* pidData)
{
	pidData->out = pidData->P + pidData->I + pidData->D;
	pidData->out = constrain_int32(pidData->out, -pidData->outmax, +pidData->outmax)/100;	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
