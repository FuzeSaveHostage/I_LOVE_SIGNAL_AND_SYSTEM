#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>
#include "Macros.h"
#include "ActionCtrl.h"

typedef struct
{
	bool HaveHanded;		//��Ϊ1,��˵���Ѿ����ӹ�һ��
	bool isCouldExecute;	//��Ϊ1����˵��������ִ��׼��
	
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
	GetNormball = 0,			//ȡ��
	GetGoldenball,
	MiddlePosition_TZ1,
	HandOverPosition_TZ1,
	GiveWay,
	MiddlePosition_TZ2,
	HandOverNormPosition_TZ2,
	HandOverGodenPosition_TZ2,
	GoBack,
	PointNum						//λ�õ�����
};

extern enum MovePointsMarkType GotoThePoint;
extern enum HaveBallSequence HandOverPosition;
extern MovePointsType MovePoint[PointNum];		//�Զ����е��ĵ㼯
extern enum MovePointsMarkType MovePointCnt;	//�Զ��������
extern bool Next;
extern bool ChangeFlag;							//ָ����ı�־��������һ�����Ȳ������κ����㣬Ҳ������
extern bool	CastNormBallSuccessed_TZ1;
extern bool	CastNormBallSuccessed_TZ2;
extern bool	CastGoldenBallSuccessed;
extern uint8_t HaveBallNumber;

void MovePointsInit(MovePointsType* Points);
void CarryOutAction(enum SpecialActions &Act, uint8_t &Cylinders);
void MoveDecision(MovePointsType* Points, enum MovePointsMarkType* PointCnt);
void GoToSpecialPoints(MovePointsType* Points, enum MovePointsMarkType *PointCnt, enum MovePointsMarkType ToPoint);


#endif
