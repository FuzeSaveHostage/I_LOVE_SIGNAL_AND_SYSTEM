#include "PID.h"

 /**
  * @brief  输出限幅(不用于电机的输出限幅，仅运用于各小部分的运算，如横滚PID、俯仰PID的输出)
  * @param  int32_t amt：待输出量
	*					int32_t low：输出最小值
	*					int32_t high：输出最大值
  * @retval int32_t X：限幅后的输出量
  */
int32_t constrain_int32(int32_t amt, int32_t low, int32_t high) 
{
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}


 /**
  * @brief  PID参数更新（最终PID输出与电机PWM比例为100：1）->（为了避免浮点运算，提高运算效率和准确性）
  * @param  struct PID* pidData：待初始化的PID结构体变量
	*					uint16_t kp：P参数
	*					uint16_t ki：I参数
	*					uint16_t kd：D参数
	*					int32_t imax：积分限幅
	*					int32_t outmax：输出限幅
  * @retval 无
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
  * @brief  PID参数更新
  * @param  struct PID* pidData：待操作的PID结构体变量
  * @retval 无
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
  * @brief  清空积分值
  * @param  struct PID* pidData：待操作的PID结构体变量
  * @retval 无
  */
void ResetI(struct PID* pidData)
{
	pidData->I = 0;
}

 /**
  * @brief  获得P计算结果
  * @param  struct PID* pidData：待操作的PID结构体变量
  * @retval 无
  */
void GetP(struct PID* pidData)
{
	pidData->P = pidData->kP * pidData->error;
}

 /**
  * @brief  获得I计算结果
  * @param  struct PID* pidData：待操作的PID结构体变量
  * @retval 无
  */
void GetI(struct PID* pidData)
{
	pidData->I += pidData->kI * pidData->error;
	pidData->I = constrain_int32(pidData->I, -pidData->Imax, +pidData->Imax);	
}

 /**
  * @brief  获得D计算结果
  * @param  struct PID* pidData：待操作的PID结构体变量
  * @retval 无
  */
void GetD(struct PID* pidData)
{
	pidData->D = pidData->kD * pidData->derror;
}

 /**
  * @brief  获得P、I输出量（PI控制）
  * @param  struct PID* pidData：待操作的PID结构体变量
  * @retval 无
  */
void GetPI(struct PID* pidData)
{
	pidData->out = pidData->P + pidData->I;
	pidData->out = constrain_int32(pidData->out, -pidData->outmax, +pidData->outmax)/100;	
}

 /**
  * @brief  获得P、I、D输出量（PID控制）
  * @param  struct PID* pidData：待操作的PID结构体变量
  * @retval 无
  */
void GetPID(struct PID* pidData)
{
	pidData->out = pidData->P + pidData->I + pidData->D;
	pidData->out = constrain_int32(pidData->out, -pidData->outmax, +pidData->outmax)/100;	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
