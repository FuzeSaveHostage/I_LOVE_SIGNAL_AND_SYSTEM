#ifndef __Vol_curve__
#define __Vol_curve__

#include <stdint.h>
#include "Macros.h"
#include "PID.h"

#define Trapezoid 0
#define Triangle  1

#define GET  1
#define NGET 0

#define ARRIVE  1
#define NARRIVE 0

typedef struct {
	
	int32_t v;
	int32_t x;
	
}X_V;

typedef struct
{
	uint8_t Move_flag;			//��ǽ����������ı�־	
	uint8_t New_flag;			//��Ŀ��������ձ�־
	uint8_t Arrive_flag;		//Ŀ��㵽���־
	bool 	MotionOverflowflag;	//�˶�ָ�������־��ָ���Ƿ񳬳��˶�������
	
	double a_max;				//���ٶ����ֵ
	double vol_max;  			//�ٶ����ֵ
	
	uint32_t x_setted;			//�滮���н�����λ��
	uint32_t x_stan,t_stan;		//������ʱ�䡢��λ��
	uint32_t t;					//Ԥ���˶�����ʱ��
	double a_cacul;				//��������õļ��ٶ�
	
//	int32_t distance;			//��Ŀ���ľ���
	int32_t ExpectedDisplace;	//����λ��
	int32_t RealDisplace;		//ʵ��λ��
	
	int32_t CurveSpeed;			//�ٶ����ߵ�����ٶ�
	
}Vol_CurveType;

extern Vol_CurveType Vol_Curve;

void CurveParamInit(void);
void CreatCurve(double aimx, double aimy, uint32_t aimt);
void CurveLoop(double Distance);

#endif
