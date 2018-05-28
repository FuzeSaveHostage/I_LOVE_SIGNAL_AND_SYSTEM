#ifndef _BASE_H_
#define _BASE_H_

#include <stdint.h>
#include "Macros.h"

struct CMD
{	
	uint8_t R_Ctrl_Flag;				//ң�ر�־��0Ϊ��λģʽ��1Ϊң��ģʽ��
	
	int16_t aimx;						//Ŀ������(��λ��mm)
	int16_t aimy;
	int16_t aimt;						//(��λ��ms)
		
	int16_t ward;						//�ƶ������
	int16_t line_speed;					//�ƶ��ٶ�

	int16_t Pos_Angle;  				//��̬��
	
	int16_t Angle_cnt;					//��������ʱ�ĽǶȼ�������ֵ�����ڽǶȼ��������Բο�Map.c�еĽ��ܣ�
};

enum BaseMode
{
	AimMode = 0x00,					//Ŀ�����ģʽ
	OpenLoopMode = 0x01,			//��������ģʽ
};

enum AirPressureStateType
{
	AirNorm_TZ1 = 0,
	AirNorm_TZ2,
	AirGoden_TZ2
};

typedef struct 
{
	bool RcvPIDSuccess;
	bool InitSuccess;
	bool restart;
	
}Base_flags;

extern struct CMD 	Base_CMD;
extern Base_flags	Baseflags;
extern enum AirPressureStateType AirPressureState;

void baseInit(void);
void Base_Loop(void);
void CtrlPara_Init(void);
void PromptToneScan(void);
void AimAngleUpdate(double aimAngle);

#endif
