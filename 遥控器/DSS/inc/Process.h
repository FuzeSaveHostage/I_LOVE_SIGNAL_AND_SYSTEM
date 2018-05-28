#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include "Macros.h"
#include "ActionCtrl.h"

typedef struct
{
	bool HaveHanded;		//若为1,则说明已经交接过一次
	bool isCouldExecute;	//若为1，则说明已做好执行准备
	
}HandflagType;

typedef struct 
{
	bool isGetBall;
	bool isActed;
	
}BallStateflagsType;

typedef struct
{
	int32_t Aim_X;
	int32_t Aim_Y;
	int32_t POS_Angle;
	
}MovePointsType;

enum HaveBallSequence
{
	NullPosition = -1,
	Left = 0, 
	Right,
	HaveBallSequenceNum
};

enum MovePointsMarkType	//manul
{
	StartPosition = -1,
	GetNormball = 0,			//取球
	GetGoldenball,
	MiddlePosition_TZ1,
	HandOverPosition_TZ1,
	GiveWay,
	MiddlePosition_TZ2,
	HandOverNormPosition_TZ2,
	HandOverGodenPosition_TZ2,
	GoBack,
	PointNum						//位置的数量
};

extern enum MovePointsMarkType GotoThePoint;
extern enum HaveBallSequence HandOverPosition;
extern MovePointsType MovePoint[PointNum];		//自动运行到的点集
extern enum MovePointsMarkType MovePointCnt;	//自动点计数器
extern bool Next;
extern bool ChangeFlag;							//指令更改标志，若被置一，则先不进行任何运算，也不发送
extern bool	CastNormBallSuccessed_TZ1;
extern bool	CastNormBallSuccessed_TZ2;
extern bool	CastGoldenBallSuccessed;
extern uint8_t HaveBallNumber;

void MovePointsInit(MovePointsType* Points);
void CarryOutAction(enum SpecialActions &Act, uint8_t &Cylinders);
void MoveDecision(MovePointsType* Points, enum MovePointsMarkType* PointCnt);
void GoToSpecialPoints(MovePointsType* Points, enum MovePointsMarkType *PointCnt, enum MovePointsMarkType ToPoint);


#endif
