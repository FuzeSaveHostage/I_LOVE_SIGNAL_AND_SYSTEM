#include "Map.h"
#include "Base.h"
#include "Process.h"
#include "Motion_Ctrl.h"
#include "InterfaceToMaster.h"
#include "InterfaceToMicroPC.h"

#include <math.h>

#define AverageVol 1.0f

//const uint32_t ClawInterval = 150;								//צ�Ӽ�ࣨ��λmm��

bool Next = false;
bool ChangeFlag = false;										//ָ����Ļ����־��������һ�����Ȳ������κ����㣬Ҳ������
bool CastNormBallSuccessed_TZ1 = false;
bool CastNormBallSuccessed_TZ2 = false;
bool CastGoldenBallSuccessed = false;
enum MovePointsMarkType MovePointCnt = StartPosition;			//�Զ������������ʶ�����˵�״̬��λ��
MovePointsType MovePoint[PointNum] = {0};						//�Զ����е��ĵ㼯

HandflagType HandReadyflag = {true, false};						//��ȡ����׼����ϱ�־
uint8_t HaveBallNumber = 0;										//��������ʣ����ĸ���

enum MovePointsMarkType GotoThePoint = StartPosition;

/**
  * @brief  ��ʼ���˶�����Ŀ���
  * @param  Points:ָ��Ŀ��������ָ��
  * @param  PointsNum:Ŀ�������
  * @retval ��
  */
void MovePointsInit(MovePointsType* Points)
{
	Points[GetNormball].Aim_X = 0;						//ȡ��
	Points[GetNormball].Aim_Y = -2400;
	Points[GetNormball].POS_Angle = 0;
	
	Points[GetGoldenball].Aim_X = 0;						//ȡ��ɫ��
	Points[GetGoldenball].Aim_Y = -2400;
	Points[GetGoldenball].POS_Angle = 0;
	
	Points[MiddlePosition_TZ1].Aim_X = -550;				//�м�λ��1
	Points[MiddlePosition_TZ1].Aim_Y = 5703;
	Points[MiddlePosition_TZ1].POS_Angle = 0;
	
	Points[HandOverPosition_TZ1].Aim_X = -500;//-550		//TZ1����
	Points[HandOverPosition_TZ1].Aim_Y = 5851;
	Points[HandOverPosition_TZ1].POS_Angle = 0;
	
	Points[GiveWay].Aim_X = -217;							//���Զ�����·
	Points[GiveWay].Aim_Y = 8300;
	Points[GiveWay].POS_Angle = 0;
	
	Points[MiddlePosition_TZ2].Aim_X = -350;				//�м�λ��2
	Points[MiddlePosition_TZ2].Aim_Y = 8300;
	Points[MiddlePosition_TZ2].POS_Angle = 0;
	
	Points[HandOverNormPosition_TZ2].Aim_X = -1000;				//TZ2����
	Points[HandOverNormPosition_TZ2].Aim_Y = 8300;
	Points[HandOverNormPosition_TZ2].POS_Angle = 0;
	
	Points[HandOverGodenPosition_TZ2].Aim_X = -700;				//TZ2����
	Points[HandOverGodenPosition_TZ2].Aim_Y = 8441;
	Points[HandOverGodenPosition_TZ2].POS_Angle = 0;
	
	Points[GoBack].Aim_X = 0;								//��ԭ��
	Points[GoBack].Aim_Y = 0;
	Points[GoBack].POS_Angle = 0;
	
}

void delay_ms(unsigned int t);
void delay_us(unsigned int t);


/**
 * @brief  ���½��ӱ�־
 * @param  OutSpeed:������ٶ�
 * @param  isArrived:�Ƿ񵽴���Ŀ��λ�ø���
 * @retval ��
 */
static void UpdateHandOverflags(int32_t OutSpeed, bool isArrived)
{
	if(isArrived)
	{
		if(OutSpeed == 0 && !HandReadyflag.HaveHanded)
		{
			HandReadyflag.isCouldExecute = true;
			HandReadyflag.HaveHanded = true;
		}
	}
	else
	{
		if(HandReadyflag.HaveHanded)
		{
			HandReadyflag.HaveHanded = false;
		}
	}
}

/**
  * @brief  ��ȡ�Զ��˶���ʱ��
  * @param  aimx:Ŀ�������x
  * @param  aimy:Ŀ�������y
  * @param  thisx:��ǰ������x
  * @param  thisy:��ǰ������y
  * @param  ave_Vol:Ԥ�ڵ�ƽ���ٶ�(��λ��mm/s)
  * @retval ��
  */
uint32_t GetAimTime(int32_t aimx, int32_t aimy, int32_t thisx, int32_t thisy, double ave_Vol)
{
	double result;
	
	double deltax = aimx - thisx;
	double deltay = aimy - thisy;
	
	result = sqrt(deltax * deltax + deltay * deltay);
	
	return (uint32_t)(result / ave_Vol);
}

void GoToSpecialPoints(MovePointsType* Points, enum MovePointsMarkType *PointCnt,  enum MovePointsMarkType ToPoint)
{
	if(true == Next && Motion.isENDLoop)		//��Ҫ������һ��
	{
		Next = false;
		
		switch(*PointCnt)
		{
			case GetNormball:
								if(HandOverPosition_TZ1 == ToPoint)
								{
									SpinState = Spin_GiveNormBall1;
									(*PointCnt) = HandOverPosition_TZ1;
									Base_CMD.aimt = 5000;
								}
								else if(HandOverGodenPosition_TZ2 == ToPoint)
								{
									SpinState = Spin_GiveGodenBall;
									(*PointCnt) = HandOverGodenPosition_TZ2;
									Base_CMD.aimt = 6000;
								}

								
								break;
			
			case HandOverPosition_TZ1:
			case HandOverGodenPosition_TZ2:
								
								if(GetNormball == ToPoint)
								{
									SpinState = Spin_GetBall;
									(*PointCnt) = GetNormball;
									Base_CMD.aimt = 6000;
								}
									
								
								break;
			
			case StartPosition:
				
								if(GetNormball == ToPoint)
								{
									SpinState = Spin_GetBall;
									(*PointCnt) = GetNormball;
									Base_CMD.aimt = 3000;
								}
								
								break;
								
			default:			break;
		}
		
		ChangeFlag = true;

		Base_CMD.aimx = Points[*PointCnt].Aim_X;
		Base_CMD.aimy = Points[*PointCnt].Aim_Y;
		
		AimAngleUpdate(Points[*PointCnt].POS_Angle);
		
		//�����µ�Ŀ�����겢�����ٶ�����
		AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
			
		ChangeFlag = false;
	}
	else if(false == Next && Motion.isENDLoop)	//���ߵ���ĳһ����յ�
	{
		
	}
	else				//���������һ��ָ����Ч
	{
		Next = false;
	}
	
}

/**
  * @brief  �Զ��˶�����
  * @param  Points:�Զ��㼯��
  * @param  PointCnt:�Զ��������
  * @retval ��
  */
void MoveDecision(MovePointsType* Points, enum MovePointsMarkType* PointCnt)
{	
	static uint8_t ActionCnt = 0;
	
	if(true == Next && Motion.isENDLoop)		//������ĳһ����յ�ջ�����
	{
		Next = false;
		ActionCnt = 0;
		
		if(*PointCnt < PointNum)						//���г̻�δ����
		{
			if(0 == HaveBallNumber && !((*PointCnt) == GetNormball || (*PointCnt) == GetGoldenball))	//������ڽ�����������û����
			{								
				if(!CastNormBallSuccessed_TZ1 || !CastNormBallSuccessed_TZ2)
				{
					(*PointCnt) = GetNormball;
					Base_CMD.aimt = 4500;
				}
				else if(!CastGoldenBallSuccessed)
				{
					(*PointCnt) = GetGoldenball;
					Base_CMD.aimt = 4500;
				}
			}
			else
			{
				switch((*PointCnt))
				{	
					case HandOverPosition_TZ1:	if(0)//(CastNormBallSuccessed_TZ1)
												{
													(*PointCnt) = GiveWay;
													Base_CMD.aimt = 3000;
												}
												else
												{
													(*PointCnt) = GetGoldenball;
													Base_CMD.aimt = 3000;
												}
												

												break;
					
					case GetNormball:			
												if(1)//(true == CastNormBallSuccessed_TZ1)
												{
													(*PointCnt) = HandOverPosition_TZ1;
													Base_CMD.aimt = 4500;
												}
												else
												{
													(*PointCnt) = MiddlePosition_TZ1;
													CastNormBallSuccessed_TZ1 = true;
													Base_CMD.aimt = 4100;
												}
												
												break;
												
					case MiddlePosition_TZ1:	(*PointCnt) = HandOverPosition_TZ1;
												Base_CMD.aimt = 1700;
												
												break;
												
					case GetGoldenball:			//SpinDeviceCtrl(Spin_HandOver);
												(*PointCnt) = HandOverGodenPosition_TZ2;
												Base_CMD.aimt = 4500;
												
												break;
												
					case MiddlePosition_TZ2:	
												if(CastNormBallSuccessed_TZ2)
												{
													(*PointCnt) = HandOverGodenPosition_TZ2;
													Base_CMD.aimt = 2000;
												}
												else
												{
													(*PointCnt) = HandOverNormPosition_TZ2;
													Base_CMD.aimt = 2000;
												}
												
												
												break;
												
					case HandOverGodenPosition_TZ2:	
						
												(*PointCnt) = GetGoldenball;
												Base_CMD.aimt = 6000;
												break;
													
					case HandOverNormPosition_TZ2:	
												if(true == CastNormBallSuccessed_TZ2)
												{
													if(CastGoldenBallSuccessed)
													{
														(*PointCnt) = GoBack;
														Base_CMD.aimt = 6000;
													}
													else
													{
//														SpinDeviceCtrl(Spin_GetBall);
														(*PointCnt) = GetGoldenball;
														Base_CMD.aimt = 4700;
													}
												}
												else
												{
													(*PointCnt) = HandOverGodenPosition_TZ2;	
												}
												break;
												
					case StartPosition:			(*PointCnt) = GetNormball;
												Base_CMD.aimt = 2700;
												break;
												
					case GiveWay:			(*PointCnt) = HandOverNormPosition_TZ2;
												Base_CMD.aimt = 2700;
												break;
												
					default:	(*PointCnt) = (enum MovePointsMarkType)((*PointCnt) + 1);	break;
				}
			}

			ChangeFlag = true;

			Base_CMD.aimx = Points[*PointCnt].Aim_X;
			Base_CMD.aimy = Points[*PointCnt].Aim_Y;
			
			AimAngleUpdate(Points[*PointCnt].POS_Angle);
			
			//�����µ�Ŀ�����겢�����ٶ�����
			AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
				
			ChangeFlag = false;
		}
		else
		{
			(*PointCnt) = StartPosition;							//������һ������
		}
	}
	else
	{
		Next = false;									//δ��Ŀ���ġ���һ����ָ����Ч
	}
	
	if(Motion.isENDLoop)
	{			
		if(HandReadyflag.isCouldExecute)			//����������ͨ���λ�����Ѿ�׼���ö���
		{	
			delay_ms(2000);	//�ȳ�ͣ��
				
	//		KeepCoordinateValue();
								
			DoActions();
								
			HandReadyflag.isCouldExecute = false;
						
			delay_ms(9000); 		//�ȵ���ֹͣ��
							
	//		GetCoordinateOffset();
							
			HandReadyflag.isCouldExecute = false;
		}			
	}
	
	UpdateHandOverflags(Motion.outSpeed, Motion.isENDLoop);
	
	switch(*PointCnt)
	{
		case GetNormball:							//��ͣ�Ⱥ����˶�
		case GetGoldenball:
//		case HandOverPosition_TZ1:
//		case HandOverNormPosition_TZ2:
//		case HandOverGodenPosition_TZ2:
				
					if(HandReadyflag.isCouldExecute)			//����������ͨ���λ�����Ѿ�׼���ö���
					{	
						delay_ms(2000);	//�ȳ�ͣ��
						
//						KeepCoordinateValue();
							
						DoActions();
							
						HandReadyflag.isCouldExecute = false;
					
						delay_ms(9000); 		//�ȵ���ֹͣ��
						
//						GetCoordinateOffset();
						
						HandReadyflag.isCouldExecute = false;
					}
							
					break;

						
		case MiddlePosition_TZ1:							//�ı�Ŀ��״̬��ʱ�����ǰ׼��
		case GiveWay:
		case MiddlePosition_TZ2:
		case GoBack:
					if(ActionCnt == 0)
					{
						DoActions();
						Next = true;
						delay_ms(9000); 		//�ȵ���ֹͣ��
					}
						
					
					break;
					
		case StartPosition:
					if(ActionCnt == 0)
					{
//						DoActions();
						delay_ms(9000); 		//�ȵ���ֹͣ��
					}
						
					
					break;
					
		default:break;
		
	}			
}

/**
 * @brief  ������ִ���ض�����
 * @param  ACT:��������Ҫִ�е����⶯��
 * @param  Cylinders:ִ�ж���������
 * @retval ��
 */
void CarryOutAction(enum SpecialActions &Act, uint8_t &Cylinders)
{
//	ActionScan(Act, Cylinders);
	SpecialAct(Act, Cylinders);
}

