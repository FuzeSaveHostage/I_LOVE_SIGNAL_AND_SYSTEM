#ifndef __MOTION_CTRL_H
#define __MOTION_CTRL_H


#include <stdint.h>
#include "Macros.h"
#include "PID.h"

typedef struct
{
	int32_t outSpeed;				//������ٶ�
	double Ward;					//�������õ��˶�����
	
	bool isENDLoop;					//�յ�״̬�����־������Low_Frequency_Loop�����иı�ֵ��
	
	double distance;				//�������õľ���

	
}MotionParamType;

/* Public  macro --------------------------*/
#define PID_END_I_MAX     		2000     	//�յ�ջ��Ļ����޷�
#define PID_END_OUT_MAX     	4000     	//�յ�ջ���������ֵ

#define PID_TIME_I_MAX     		2000     	//ʱ��ջ��Ļ����޷�
#define PID_TIME_OUT_MAX   		4000     	//ʱ��ջ���������ֵ

#define PID_Location_I_MAX     	2000     	//�н���λ�ñջ��Ļ����޷�
#define PID_Location_OUT_MAX   	3000     	//�н���λ�ñջ���������ֵ


/* Public  variables ----------------------*/
extern struct PID pidData_Time;					//ʱ��ջ������ڻ�PID�������ٶȲ���
extern struct PID pidData_End;					//�յ�ջ������ڻ�PID��
extern struct PID pidData_Location;

extern MotionParamType Motion;					//�˶�����

/**		������ʼ��		**/
void PIDInit(void);
void AfterGetAim(double aimx, double aimy, uint32_t aimt);

/**		��ʱ��������ĺ���		**/
void AimMode_Loop(void);
void OpenMode_Loop(void);

#endif

/******************* (C) COPYRIGHT 2017 YANGYIFAN *****END OF FILE****/
