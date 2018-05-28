#include "Map.h"
#include "Base.h"
#include "Process.h"
#include "Motion_Ctrl.h"
#include "InterfaceToMaster.h"
#include "InterfaceToMicroPC.h"

#include <math.h>

#define AverageVol 1.0f

//const uint32_t ClawInterval = 150;								//爪子间距（单位mm）

bool Next = false;
bool ChangeFlag = false;										//指令更改互斥标志，若被置一，则先不进行任何运算，也不发送
bool CastNormBallSuccessed_TZ1 = false;
bool CastNormBallSuccessed_TZ2 = false;
bool CastGoldenBallSuccessed = false;
enum MovePointsMarkType MovePointCnt = StartPosition;			//自动点计数器，标识机器人的状态和位置
MovePointsType MovePoint[PointNum] = {0};						//自动运行到的点集

HandflagType HandReadyflag = {true, false};						//交取球动作准备完毕标志
uint8_t HaveBallNumber = 0;										//机器人上剩余球的个数

enum MovePointsMarkType GotoThePoint = StartPosition;

/**
  * @brief  初始化运动过程目标点
  * @param  Points:指向目标点的数组指针
  * @param  PointsNum:目标点数量
  * @retval 无
  */
void MovePointsInit(MovePointsType* Points)
{
	Points[GetNormball].Aim_X = 0;						//取球
	Points[GetNormball].Aim_Y = -2400;
	Points[GetNormball].POS_Angle = 0;
	
	Points[GetGoldenball].Aim_X = 0;						//取金色球
	Points[GetGoldenball].Aim_Y = -2400;
	Points[GetGoldenball].POS_Angle = 0;
	
	Points[MiddlePosition_TZ1].Aim_X = -550;				//中间位置1
	Points[MiddlePosition_TZ1].Aim_Y = 5703;
	Points[MiddlePosition_TZ1].POS_Angle = 0;
	
	Points[HandOverPosition_TZ1].Aim_X = -500;//-550		//TZ1交接
	Points[HandOverPosition_TZ1].Aim_Y = 5851;
	Points[HandOverPosition_TZ1].POS_Angle = 0;
	
	Points[GiveWay].Aim_X = -217;							//给自动车让路
	Points[GiveWay].Aim_Y = 8300;
	Points[GiveWay].POS_Angle = 0;
	
	Points[MiddlePosition_TZ2].Aim_X = -350;				//中间位置2
	Points[MiddlePosition_TZ2].Aim_Y = 8300;
	Points[MiddlePosition_TZ2].POS_Angle = 0;
	
	Points[HandOverNormPosition_TZ2].Aim_X = -1000;				//TZ2交接
	Points[HandOverNormPosition_TZ2].Aim_Y = 8300;
	Points[HandOverNormPosition_TZ2].POS_Angle = 0;
	
	Points[HandOverGodenPosition_TZ2].Aim_X = -700;				//TZ2交接
	Points[HandOverGodenPosition_TZ2].Aim_Y = 8441;
	Points[HandOverGodenPosition_TZ2].POS_Angle = 0;
	
	Points[GoBack].Aim_X = 0;								//回原点
	Points[GoBack].Aim_Y = 0;
	Points[GoBack].POS_Angle = 0;
	
}

void delay_ms(unsigned int t);
void delay_us(unsigned int t);


/**
 * @brief  更新交接标志
 * @param  OutSpeed:输出的速度
 * @param  isArrived:是否到达了目标位置附近
 * @retval 无
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
  * @brief  获取自动运动的时间
  * @param  aimx:目标点坐标x
  * @param  aimy:目标点坐标y
  * @param  thisx:当前点坐标x
  * @param  thisy:当前点坐标y
  * @param  ave_Vol:预期的平均速度(单位：mm/s)
  * @retval 无
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
	if(true == Next && Motion.isENDLoop)		//若要进入下一点
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
		
		//设置新的目标坐标并生成速度曲线
		AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
			
		ChangeFlag = false;
	}
	else if(false == Next && Motion.isENDLoop)	//若走到了某一点的终点
	{
		
	}
	else				//其余情况下一步指令无效
	{
		Next = false;
	}
	
}

/**
  * @brief  自动运动决策
  * @param  Points:自动点集合
  * @param  PointCnt:自动点计数器
  * @retval 无
  */
void MoveDecision(MovePointsType* Points, enum MovePointsMarkType* PointCnt)
{	
	static uint8_t ActionCnt = 0;
	
	if(true == Next && Motion.isENDLoop)		//若进入某一点的终点闭环区域
	{
		Next = false;
		ActionCnt = 0;
		
		if(*PointCnt < PointNum)						//若行程还未走完
		{
			if(0 == HaveBallNumber && !((*PointCnt) == GetNormball || (*PointCnt) == GetGoldenball))	//如果身在交接区且手上没球了
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
			
			//设置新的目标坐标并生成速度曲线
			AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
				
			ChangeFlag = false;
		}
		else
		{
			(*PointCnt) = StartPosition;							//重新走一遍流程
		}
	}
	else
	{
		Next = false;									//未到目标点的“下一步”指令无效
	}
	
	if(Motion.isENDLoop)
	{			
		if(HandReadyflag.isCouldExecute)			//若到了抛普通球的位置且已经准备好动作
		{	
			delay_ms(2000);	//等车停稳
				
	//		KeepCoordinateValue();
								
			DoActions();
								
			HandReadyflag.isCouldExecute = false;
						
			delay_ms(9000); 		//等底盘停止振动
							
	//		GetCoordinateOffset();
							
			HandReadyflag.isCouldExecute = false;
		}			
	}
	
	UpdateHandOverflags(Motion.outSpeed, Motion.isENDLoop);
	
	switch(*PointCnt)
	{
		case GetNormball:							//等停稳后再运动
		case GetGoldenball:
//		case HandOverPosition_TZ1:
//		case HandOverNormPosition_TZ2:
//		case HandOverGodenPosition_TZ2:
				
					if(HandReadyflag.isCouldExecute)			//若到了抛普通球的位置且已经准备好动作
					{	
						delay_ms(2000);	//等车停稳
						
//						KeepCoordinateValue();
							
						DoActions();
							
						HandReadyflag.isCouldExecute = false;
					
						delay_ms(9000); 		//等底盘停止振动
						
//						GetCoordinateOffset();
						
						HandReadyflag.isCouldExecute = false;
					}
							
					break;

						
		case MiddlePosition_TZ1:							//改变目标状态的时候就提前准备
		case GiveWay:
		case MiddlePosition_TZ2:
		case GoBack:
					if(ActionCnt == 0)
					{
						DoActions();
						Next = true;
						delay_ms(9000); 		//等底盘停止振动
					}
						
					
					break;
					
		case StartPosition:
					if(ActionCnt == 0)
					{
//						DoActions();
						delay_ms(9000); 		//等底盘停止振动
					}
						
					
					break;
					
		default:break;
		
	}			
}

/**
 * @brief  机器人执行特定动作
 * @param  ACT:机器人所要执行的特殊动作
 * @param  Cylinders:执行动作的气缸
 * @retval 无
 */
void CarryOutAction(enum SpecialActions &Act, uint8_t &Cylinders)
{
//	ActionScan(Act, Cylinders);
	SpecialAct(Act, Cylinders);
}

