#ifndef __ACT__H
#define __ACT__H

#include "cylinder.h"
#include "limitswitch.h"
#include "process.h"
#include "laser.h"

#include "stdio.h"


typedef struct
{
	uint8_t basicCylGroup;
	uint8_t basicCylType;
	uint8_t basicCylState;
}__CYL_ACT_t;

typedef struct
{
	uint8_t left_roate;
	uint8_t right_roate;
	uint8_t ball_serial_num;
	uint8_t pressure_signal;
}__ASS_CON_ACT_t;

extern __CYL_ACT_t cylAct;
extern __ASS_CON_ACT_t assConActSet;
extern __ASS_CON_ACT_t assConActGet;


#define IDLE 0X00
#define LOST 0X01
#define SET  0X02
extern uint8_t fCOM;

#define LASER_START 0X01
#define LASER_FINISH 0X02
extern uint8_t fLASER;

#define ASS_CON 0X00
#define MAS_CON 0X01

//旋转位置
#define         NULL_POS          0x00
#define         CATCH_POS					0x01
#define         STORE_POS					0x02
#define         LAUNCH_POS       	0x03

//气缸组
#define     LEFT_GROUP        0x01
#define     RIGHT_GROUP       0x03

#define 		DELAY_CLAW				200
#define 		DELAY_AUTO				1000
#define 		DELAY_LINE				1500

void FeedDog(uint8_t i);
void CheckCommunication(uint8_t time);
void LASER_StartMesure(void);
void CYL_BaseAct(uint8_t group, uint8_t type, uint8_t state);
uint8_t LIMITSWITCH_WaitChange(__LIMITSWITCH_REG_t *sw , uint32_t waitMs);
void HighPriorityCylAct(void);
void RoateControl(uint8_t pos, uint8_t group);
void RoateControl_noACK(uint8_t pos, uint8_t group);
void PressureControl(uint8_t pre);
//void SimpleSylinderAct(uint8_t act);
//

void f_FETCH_LEFT_BALL			(void);
void f_FETCH_RIGHT_BALL			(void);
void f_FETCH_BOTH_BALL			(void);
void f_PASS_NORMAL_BALL_1		(void);
void f_PASS_NORMAL_BALL_2		(void);
void f_PASS_GOLDEN_BALL			(void);
void f_NORM_HANDOVER_STEP1		(void);
void f_NORM_HANDOVER_STEP2		(void);
void f_GOLDEN_HANDOVER			(void);
void f_PUSH_LEFT_CLAW			(void);
void f_PUSH_RIGHT_CLAW			(void);
void f_PUSH_BOTH_CLAW			(void);
void f_PULL_LEFT_CLAW			(void);
void f_PULL_RIGHT_CLAW			(void);
void f_PULL_BOTH_CLAW			(void);
void f_REFERSH_ACT				(void);

void f_NORMAL_SHELF				(void);
void f_MID_TZ1					(void);
void f_GOLDEN_SHELF				(void);
void f_MID_TZ2					(void);
void f_NONE_ACT					(void);
void f_TZ1						(void);
void f_TZ2_NORMAL				(void);
void f_TZ2_GOLDEN				(void);

#endif
