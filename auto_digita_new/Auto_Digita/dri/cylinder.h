#ifndef __CYLINDER__H
#define __CYLINDER__H

#include "stm32f10x.h"
#include "iobit.h"

#define CYL_STATE_1 PEout(0)
#define CYL_STATE_2 PEout(1)
#define CYL_STATE_3 PEout(2)
#define CYL_STATE_4 PEout(3)
#define CYL_STATE_5 PEout(4)
#define CYL_STATE_6 PEout(5)
#define CYL_STATE_7 PEout(6)
#define CYL_STATE_8 PEout(7)
#define CYL_STATE_9 PEout(8)
#define CYL_STATE_10 PEout(9)

//********气缸分组宏定义*********//
#define CYL_ON 1
#define CYL_OFF 0
#define CYL_IDLE 0XFF

#define CYL_CLAW_OPEN CYL_ON
#define CYL_CLAW_CLOSE CYL_OFF

#define CYL_PUSH_PUSH CYL_ON
#define CYL_PUSH_PULL CYL_OFF

#define CYL_GROUP_LEFT 	0X01
#define CYL_GROUP_RIGHT 0X02
#define CYL_GROUP_BOTH 	0X03
#define CYL_GROUP_DELAY 0x04
#define CYL_GROUP_SPEC	0X05

#define CYL_TYPE_CLAW_UP   0X01   //上爪
#define CYL_TYPE_CLAW_DOWN 0X02   //下爪
#define CYL_TYPE_LINE      0X00
//#define CYL_TYPE_BOTH      0X03
//******************************//

typedef struct
{
	uint8_t group;
	uint8_t type;
	void (*pSwitch)(uint8_t);
}__CYLINDER_REG_t;

extern __CYLINDER_REG_t cylRegistry[10];

void CYL1(uint8_t i);
void CYL2(uint8_t i);
void CYL3(uint8_t i);
void CYL4(uint8_t i);
void CYL5(uint8_t i);
void CYL6(uint8_t i);
void CYL7(uint8_t i);
void CYL8(uint8_t i);
void CYL9(uint8_t i);
void CYL10(uint8_t i);

void CYLINDER_Init(void);
void CYLINDER_Registry(void);
void CYLINDER_State(uint16_t state);

static inline void CYL1(uint8_t i)
{
	PEout(0) = i;
}

static inline void CYL2(uint8_t i)
{
	PEout(1) = i;
}

static inline void CYL3(uint8_t i)
{
	PEout(2) = i;
}

static inline void CYL4(uint8_t i)
{
	PEout(3) = i;
}

static inline void CYL5(uint8_t i)
{
	PEout(4) = i;
}

static inline void CYL6(uint8_t i)
{
	PEout(5) = i;
}

static inline void CYL7(uint8_t i)
{
	PEout(6) = i;
}

static inline void CYL8(uint8_t i)
{
	PEout(7) = i;
}

static inline void CYL9(uint8_t i)
{
	PEout(8) = i;
}

static inline void CYL10(uint8_t i)
{
	PEout(9) = i;
}

#endif
