#ifndef _BASE_H_
#define _BASE_H_

#include <stdint.h>
#include "Macros.h"

struct CMD
{	
	uint8_t R_Ctrl_Flag;				//遥控标志（0为定位模式，1为遥控模式）
	
	int16_t aimx;						//目标坐标(单位：mm)
	int16_t aimy;
	int16_t aimt;						//(单位：ms)
		
	int16_t ward;						//移动方向角
	int16_t line_speed;					//移动速度

	int16_t Pos_Angle;  				//姿态角
	
	int16_t Angle_cnt;					//发布命令时的角度计数器的值（关于角度计数器可以参看Map.c中的介绍）
};

enum BaseMode
{
	AimMode = 0x00,					//目标控制模式
	OpenLoopMode = 0x01,			//开环控制模式
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
