#ifndef _POSITIONCTRL_H_
#define _POSITIONCTRL_H_


#include "PlaneAnalyticGeometry.h"
#include "Map.h"
#include "PID.h"


struct MoveInLine
{
	Point StartPoint;			//起点坐标
	Point EndPiont;				//终点坐标
	
	double LineAngle;			//起点指向终点的向量与绝对坐标X轴正向的夹角
	double AdjAngle;			//位置矫正速度方向所在直线的方向角（与上一个角度相差90°）
	
	Line line;					//直线轨迹解析式
	
};

/* Public  macro --------------------------*/
#define END_DISTANCE		50				//进入终点闭环的距离范围（单位:mm）
#define END_Tiny_DISTANCE	20				//进入小终点闭环的距离范围（单位:mm）
#define END_ERR_ALLOW       0				//允许的终点误差范围（认为进入这个范围就允许停车了,单位：mm）

/* Public  variables ----------------------*/
extern struct MoveInLine  MOV_Line;				//直线轨迹闭环参量
extern int32_t AdjSpeed;						//调整速度
extern int32_t EndSpeed;						//稳定速度
extern int32_t End_Distance;					//动态终点闭环区域

/*  参量变动  */
void CreatMoveLine(struct MoveInLine* Line, double aimx, double aimy, Coord *coord);
void getEndDistance(double a_calcu, struct PID PIDparam);
void MixSpeed(double LineSpeed, double LineWard,
				double adjSpeed,struct MoveInLine Line, Coord Coor,
				int32_t* SpeedOut, double* WardOut);

/*  定时调用的运算  */
void Line_Location_Loop(const struct MoveInLine Line);
void END_Loop(double distance);

#endif
