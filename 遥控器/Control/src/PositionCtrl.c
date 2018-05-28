#include "PositionCtrl.h"
#include "Motion_Ctrl.h"
#include <math.h>

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))		//输出限幅	
#define ANGLE_TO_RAD(x) (x)/180.0f*3.14159f
#define RAD_TO_ANGLE(x) (x)/3.14159f*180.0f

/* Public  variables ---------------------------------------------------------*/
int32_t End_Distance = 50;				//长度距离单位：mm

struct MoveInLine MOV_Line;				//直线轨迹闭环参量
int32_t AdjSpeed;						//调整速度
int32_t EndSpeed;						//终点速度

/**
  * @brief  为了平滑连接终点闭环与直线运动终点。计算终点闭环区域,在生成速度曲线后调用
  * @param  a_calcu:速度曲线所获得的加速度
  * @param  PIDparam:终点闭环的PID参变量
  * @retval 无
  */
void getEndDistance(double a_calcu, struct PID PIDparam)
{
	double temp = PIDparam.kP / 100.0;		//根据实际PID输出值与参数的数量关系获得的100被关系。
	
	End_Distance = (uint32_t)(2 * a_calcu / (temp * temp));
}

/**
  * @brief  生成直线解析式(kx - y + (y1 - kx1) = 0)
  * @param  Line:直线轨迹闭环参量指针
  * @retval 无
  */
void CreatMoveLine(struct MoveInLine* Line, double aimx, double aimy, Coord *coord)
{
	double temp = 0;
	
	Line->StartPoint.X = coord->point.X;
	Line->StartPoint.Y = coord->point.Y;
	
	Line->EndPiont.X = aimx;
	Line->EndPiont.Y = aimy;
	
	temp = (Line->EndPiont.Y - Line->StartPoint.Y) / (Line->EndPiont.X - Line->StartPoint.X);
	
	Line->LineAngle = RAD_TO_ANGLE(atan(temp));
	
	Line->LineAngle = (Line->LineAngle<0.0 ? (Line->LineAngle + 180.0) : Line->LineAngle);//将区间转换到0-180
	
	Line->AdjAngle = Line->LineAngle - 90.0;
	
	Line->AdjAngle = (Line->AdjAngle) < 0.0 ? (Line->AdjAngle  + 180.0) : Line->AdjAngle;//将区间转换到0-180
	
	Line->line = CreateLine(Line->StartPoint, Line->EndPiont);

}

/**
  * @brief  运动中的位置闭环——直线
  * @param  Line:直线轨迹闭环参量
  * @retval 无
  */
void Line_Location_Loop(const struct MoveInLine Line)
{
	Point temp;
	
	temp.X = Coordinate.point.X;
	temp.Y = Coordinate.point.Y;
	
	//目标是0
	pidData_Location.set = 0;
	
	//获得实时数据GetDistanceToLine(temp, Line.line);
	pidData_Location.feedback = (int32_t)GetDistanceToLine(temp, Line.line);
	
	//更新PID数据
	PIDdataUpdate(&pidData_Location);

	//计算PID
	GetPID(&pidData_Location);
	
	//得到输出量
	AdjSpeed = -LimitOutput(pidData_Location.out, -PID_Location_OUT_MAX, PID_Location_OUT_MAX);
}

/**
  * @brief  速度融合——直线运动位置闭环(以原定行进方向为基准方向进行分解和合成)
  * @param  LineSpeed:由速度曲线获得的直线速度
  * @param  LineWard:当前坐标指向目标点的角度
  * @param  adjSpeed:位置补偿速度
  * @param  Line:直线位置闭环参量
  * @param  Coor:实时坐标
  * @param  SpeedOut:指向输出速度的指针
  * @param  WardOut:指向输出速度方向角的指针
  * @retval 无
  */
void MixSpeed(double LineSpeed, double LineWard,
				double adjSpeed,struct MoveInLine Line, Coord Coor,
				int32_t* SpeedOut, double* WardOut)
{
	double AngleDiffer = 0;
	double tempSpeed = 0, tempAngle = LineWard;
	
	/*  解析式为(kx - y + (y1 - kx1) = 0)  */
	if((Line.line.A * Coor.point.X + Line.line.C) < Coor.point.Y)//若点在直线上方
	{
		Line.AdjAngle -= 180.0;
	}
	if(LineSpeed < 0)						//若输出速度为负
	{
		LineSpeed = -LineSpeed;
		tempAngle -= 180.0;
		LineWard -= 180.0;
	}
	
	AngleDiffer = Line.AdjAngle - tempAngle;	//获取差角
	
	/*  分解、叠加速度  */
 	LineSpeed += adjSpeed * cos(ANGLE_TO_RAD(AngleDiffer));
	tempSpeed = adjSpeed * sin(ANGLE_TO_RAD(AngleDiffer));//获得垂直于原定直线速度方向的分速度
	
	/*  合成速度大小  */
	*SpeedOut = (int32_t)sqrt(LineSpeed * LineSpeed + tempSpeed * tempSpeed);
	
	/*  获取相对 速度方向  */
	if(fabs(LineSpeed) < 0.000001)				//类型安全
	{
		tempAngle= 90;
	}
	else
	{
		tempAngle = RAD_TO_ANGLE(atan(tempSpeed / LineSpeed));
		
		tempAngle = tempAngle < 0 ? (tempAngle + 180.0) : tempAngle;		//改范围为0->180

	}
	
	if(tempSpeed < 0.0)										//将角度方向更正为-180->180的范围
	{
		tempAngle = tempAngle - 180;
	}
	
	/*  合成绝对速度方向  */
	tempAngle = LineWard + tempAngle;									
	
	tempAngle = fmod(tempAngle, 360.0);									//求余，修正数值范围
	
	if(tempAngle < 0.0)													//将角度方向更正为-180->180的范围
	{
		tempAngle += 180;						
		
		*SpeedOut = -*SpeedOut;											//若速度在第三四象限，则为负值
	}
	
	*WardOut = tempAngle;												//赋入控制变量
}

/**
  * @brief  终点PID
  * @param  distan:当前位置到原定位置的偏差（distance）
  * @retval None
  */
void END_Loop(double distan)
{
	//目标是0
	pidData_End.set = 0;
	
	//获得实时数据
	pidData_End.feedback = distan;
	
	/* 判断当前状态是否满足停下来 */
	if (distan < END_ERR_ALLOW)
	{
		ResetI(&pidData_End);//清除积分
		EndSpeed = 0;
	}
	else
	{
		if(distan < END_Tiny_DISTANCE)		//如果进入小终点区域
		{
			//更新PID数据
			PIDdataUpdate(&pidData_End);
		
			//计算PID
			GetPID(&pidData_End);
			
			//得到输出量
			EndSpeed = -LimitOutput(pidData_End.out, -PID_END_OUT_MAX, PID_END_OUT_MAX);
		}
		else								//如果在大终点区域
		{
			//得到输出量
			EndSpeed  = 300;
		}
		
		
	}
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
