#include "process.h"

__PROCESS_t fPROCESS = {//自动
						.pos = START_POS,
						.pAutoAct = &f_NONE_ACT,

						//手动
						.cylGroup = 0,
						.cylAct = 0,
						.pHandAct = &f_NONE_ACT,

						//通信交互
						.ballNum = 0,
						.act = 0,
						.finish = 0,
						.fHANDORAUTO = AUTO,
					};

void AutoProcess(void)
{
	switch(fPROCESS.pos)
	{
		case START_POS:
			fPROCESS.pAutoAct = &f_NONE_ACT;break;
		case NORMAL_SHELF:
			fPROCESS.pAutoAct = &f_NORMAL_SHELF;break;
		case TZ1:
			fPROCESS.pAutoAct = &f_TZ1;break;
		case MID_TZ1:
			fPROCESS.ballNum = 2;
		  	fPROCESS.pAutoAct = &f_MID_TZ1;
			break;
		case GOLDEN_SHELF:
			fPROCESS.pAutoAct = &f_GOLDEN_SHELF;break;
		case TZ2_NORMAL:
			fPROCESS.pAutoAct = &f_TZ2_NORMAL;break;
		case TZ2_GOLDEN:
			fPROCESS.pAutoAct = &f_TZ2_GOLDEN;break;
		case MID_TZ2:
			fPROCESS.ballNum = 2;
			fPROCESS.pAutoAct = &f_MID_TZ2;
			break;
		default:
			break;
	}
}

void HandProcess(void)
{
	if(cylAct.basicCylState != CYL_IDLE)//高优先级单独气缸动作
	{
		fPROCESS.pHandAct = &f_NONE_ACT;
		HighPriorityCylAct();
	}
	else
	{
		switch(fPROCESS.cylGroup)//正常手动流程
		{
			case CYL_GROUP_LEFT:
				if(fPROCESS.cylAct == FETCH_BALL)
					fPROCESS.pHandAct = &f_FETCH_LEFT_BALL;

				else if(fPROCESS.cylAct == GIVE_BALL)
					fPROCESS.pHandAct = &f_PASS_NORMAL_BALL_2;

				else if(fPROCESS.cylAct == PUSH_CLAW)
					fPROCESS.pHandAct = &f_PUSH_LEFT_CLAW;

				else if(fPROCESS.cylAct == PULL_CLAW)
					fPROCESS.pHandAct = &f_PULL_LEFT_CLAW;
				break;

			case CYL_GROUP_RIGHT:
				if(fPROCESS.cylAct == FETCH_BALL)
					fPROCESS.pHandAct = &f_FETCH_RIGHT_BALL;

				else if(fPROCESS.cylAct == GIVE_BALL)
					fPROCESS.pHandAct = &f_PASS_NORMAL_BALL_1;

				else if(fPROCESS.cylAct == PUSH_CLAW)
					fPROCESS.pHandAct = &f_PUSH_RIGHT_CLAW;

				else if(fPROCESS.cylAct == PULL_CLAW)
					fPROCESS.pHandAct = &f_PULL_RIGHT_CLAW;

				break;
			case CYL_GROUP_BOTH:
				if(fPROCESS.cylAct == FETCH_BALL)
					fPROCESS.pHandAct = &f_FETCH_BOTH_BALL;

				else if(fPROCESS.cylAct == GIVE_BALL)
					fPROCESS.pHandAct = &f_PASS_GOLDEN_BALL;

				else if(fPROCESS.cylAct == PUSH_CLAW)
					fPROCESS.pHandAct = &f_PUSH_BOTH_CLAW;

				else if(fPROCESS.cylAct == PULL_CLAW)
					fPROCESS.pHandAct = &f_PULL_BOTH_CLAW;

				break;
			case CYL_GROUP_SPEC:
				if(fPROCESS.cylAct == NORM_HANDOVER_STEP1)
					fPROCESS.pHandAct = &f_NORM_HANDOVER_STEP1;

				else if(fPROCESS.cylAct == NORM_HANDOVER_STEP2)
					fPROCESS.pHandAct = &f_NORM_HANDOVER_STEP2;

				else if(fPROCESS.cylAct == GOLDEN_HANDOVER)
					fPROCESS.pHandAct = &f_GOLDEN_HANDOVER;

				else if (fPROCESS.cylAct == REFERSH)
				  fPROCESS.pHandAct = &f_REFERSH_ACT;

				break;
			default:
				break;
		}
	}
}

void AutoActFinish(void)
{
	fPROCESS.pAutoAct = &f_NONE_ACT;
	fPROCESS.finish = 1;
}

void HandActFinish(void)
{
	fPROCESS.pHandAct = &f_NONE_ACT;
	fPROCESS.finish = 1;
}
