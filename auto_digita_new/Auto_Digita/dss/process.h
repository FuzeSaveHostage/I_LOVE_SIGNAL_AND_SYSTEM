#ifndef __PRPOCESS__H
#define __PRPOCESS__H

#include "act.h"

typedef struct
{
	uint8_t pos;
	void (*pAutoAct)();

	uint8_t cylGroup;
	uint8_t cylAct;
	void (*pHandAct)();

	uint8_t ballNum;
	uint8_t act;
	uint8_t finish;
	uint8_t fHANDORAUTO;
}__PROCESS_t;

extern __PROCESS_t fPROCESS;

#define HAND 1 
#define AUTO 0

/***************状态宏*******************/
#define	START_POS		               0xFF
#define MIDDLE_POS1                0x00
#define HANDOVER_BALL1_TZ1         0x01
#define CAST_NORM_BALL_TZ1         0x02
#define MIDDLE_POS2                0x03
#define HANDOVER_BALL2_TZ1         0x04
#define HANDOVER_GOLDEN            0x05
#define CAST_NORM_BALL_TZ2         0x06
#define CAST_GOLDEN                0x07
#define MIDDLE_POS3                0x08
#define	GOBACK			               0x09
#define HANDOVER_NORM_TZ2          0x0A


//遥控指令
#define FETCH_BALL		0x01
#define GIVE_BALL		0x02
#define PUSH_CLAW		0x03
#define PULL_CLAW		0x04
#define REFERSH			0x10

#define NORM_HANDOVER_STEP1		0x07
#define NORM_HANDOVER_STEP2		0x08
#define GOLDEN_HANDOVER			0x09
/*
//动作指令
#define NONE_ACT				0x00
#define FETCH_LEFT_BALL			0x01
#define FETCH_RIGHT_BALL		0x02
#define FETCH_BOTH_BALL			0x03
#define PASS_NORMAL_BALL_1		0x04
#define PASS_NORMAL_BALL_2		0x05
#define PASS_GOLDEN_BALL		0x06

#define PUSH_LEFT_CLAW			0x0A
#define PUSH_RIGHT_CLAW			0x0B
#define PUSH_BOTH_CLAW			0x0C
#define PULL_LEFT_CLAW			0x0D
#define PULL_RIGHT_CLAW			0x0E
#define PULL_BOTH_CLAW			0x0F
#define REFERSH_ACT				0x10
*/
void AutoProcess(void);
void HandProcess(void);
void AutoActFinish(void);
void HandActFinish(void);

#endif
