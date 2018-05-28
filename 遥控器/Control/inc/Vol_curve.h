#ifndef __Vol_curve__
#define __Vol_curve__

#include <stdint.h>
#include "Macros.h"
#include "PID.h"

#define Trapezoid 0
#define Triangle  1

#define GET  1
#define NGET 0

#define ARRIVE  1
#define NARRIVE 0

typedef struct {
	
	int32_t v;
	int32_t x;
	
}X_V;

typedef struct
{
	uint8_t Move_flag;			//标记进入何种运算的标志	
	uint8_t New_flag;			//新目标坐标接收标志
	uint8_t Arrive_flag;		//目标点到达标志
	bool 	MotionOverflowflag;	//运动指令溢出标志（指令是否超出运动能力）
	
	double a_max;				//加速度最大值
	double vol_max;  			//速度最大值
	
	uint32_t x_setted;			//规划将行进的总位移
	uint32_t x_stan,t_stan;		//变速总时间、总位移
	uint32_t t;					//预期运动的总时间
	double a_cacul;				//计算所获得的加速度
	
//	int32_t distance;			//与目标点的距离
	int32_t ExpectedDisplace;	//期望位移
	int32_t RealDisplace;		//实际位移
	
	int32_t CurveSpeed;			//速度曲线的输出速度
	
}Vol_CurveType;

extern Vol_CurveType Vol_Curve;

void CurveParamInit(void);
void CreatCurve(double aimx, double aimy, uint32_t aimt);
void CurveLoop(double Distance);

#endif
