#ifndef _ACTIONCTRL_H_
#define _ACTIONCTRL_H_

#include <stdint.h>

/** @defgroup Cylinders 执行动作的气缸的标识
  * @{
  */

#define Null_Cylinders		0x00
#define Left_Cylinders		0x01
#define Right_Cylinders		0x02

/**
  * @}
  */

/** @defgroup LaserState 激光使能
  * @{
  */

#define Laser_Disable	false
#define Laser_Enable	true

/**
  * @}
  */

enum AirPressureStatetype
{
	HandOver,
	AirP_Num
};

enum ActDevices
{
	Dev_LeftCylinder = 0,
	Dev_RightCylinder,
	Dev_Spin,
	DevNum
};

enum SpecialActions
{
	NullAction = 0x00,				//无动作
	getBall = 0x01,
	giveBall = 0x02,
	push_claw,
	fetch_claw,
	none1,
	none2,
	TZ1_HandOver_Step1,
	TZ1_HandOver_Step2,
	Gloden_HandOver
};

typedef enum
{
	Spin_Null = 0,
	Spin_GetBall = 1,					
	Spin_GiveNormBall1 = 2,
	Spin_GiveNormBall2,
	Spin_GiveGodenBall,
	
}SpinPositionType;

extern SpinPositionType SpinState;
extern enum SpecialActions Act_flag;
extern uint8_t ActCylinders;
extern const uint8_t finishFlags[DevNum];
extern bool isReady[DevNum];
extern bool ActACK;
extern bool isActFinishied;

void flagCheck(void);
bool DoActions(void);
void SendStateScan(void);
void CleanActionFlags(void);
bool LaserCtrl(bool NewState);
bool SpinDeviceCtrl(SpinPositionType Position);
bool SetAirPressure(enum AirPressureStatetype AirP_State);
bool LightCtrl(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t light);
bool SpecialAct(enum SpecialActions &Act, uint8_t &Cylinders);

#endif
