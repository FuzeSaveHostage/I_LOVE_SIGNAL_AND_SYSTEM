#include "PositionCtrl.h"
#include "Motion_Ctrl.h"
#include "Vol_curve.h"
#include "Process.h"
#include "Base.h"
#include "Map.h"

#include <Math.h>

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))		//����޷�	
#define ANGLE_TO_RAD(x) (x)/180.0f*3.14159f
#define RAD_TO_ANGLE(x) (x)/3.14159f*180.0f

/* Public  variables ---------------------------------------------------------*/
struct PID pidData_Time;						//ʱ��ջ������ڻ�PID��
struct PID pidData_End;							//�յ�ջ������ڻ�PID��
struct PID pidData_Location;					//�н��е�λ�ñջ������ڻ�PID��

MotionParamType Motion;							//�˶�����

/**
* @brief  �㵽��һ���ֱ�߾����뷽��Ǽ���
* @param  line_posx��Ŀ���x����
* @param  line_posy��Ŀ���y����
* @param  posx��ʵ��ֵ
* @param  posy��ʵ��ֵ
* @retval None
*/
static void distanceCal(double line_posx, double line_posy,
						double posx, double posy, double* distan, double* LineWard)
{
	if(line_posx == posx)				//���Ͱ�ȫ
	{
		*LineWard = 90;
	}
	else
	{	
		if((0 == line_posy-posy) && (line_posx-posx < 0))
		{
			*LineWard = 180;
		}
		else
		{
			*LineWard = RAD_TO_ANGLE(atan((double)(line_posy-posy)/(double)(line_posx-posx)));
			*LineWard = (*LineWard < 0.0)?(*LineWard + 180):*LineWard;	//�����е�Wardȡֵ��Χ��0->180��

		}
		
	}
	*distan = sqrt((line_posx-posx)*(line_posx-posx)+(line_posy-posy)*(line_posy-posy));
}

/**
  * @brief  PID������ʼ��
  * @param  ��
  * @retval ��
  */
void PIDInit(void)
{	
	PIDparamUpdate(&pidData_Time, 200, 0, 0, PID_TIME_I_MAX * 100, PID_TIME_OUT_MAX * 100);
	
	PIDparamUpdate(&pidData_End, 800, 0, 0, PID_END_I_MAX * 100, PID_END_OUT_MAX * 100);
	
	PIDparamUpdate(&pidData_Location, 400, 0, 0, PID_Location_I_MAX * 100, PID_Location_OUT_MAX * 100);
	
}	


/**
  * @brief 	�����˶��滮
  * @param  None
  * @retval None
  */
void AfterGetAim(double aimx, double aimy, uint32_t aimt)
{
	//�����µ�Ŀ�����겢�����ٶ�����
	CreatCurve(aimx, aimy, aimt);
	
	//����ֱ�߹켣
	CreatMoveLine(&MOV_Line, aimx, aimy, &Coordinate);
	
	//��ȡ�յ�ջ�����
	getEndDistance(Vol_Curve.a_cacul, pidData_End);
	
	Motion.isENDLoop = false;
}
	

/**
  * @brief  ���㣺Ŀ��ģʽ
  * @param  None
  * @retval None
  */
void AimMode_Loop(void)
{
	double Line_Ward = 0;

	//��ȡ�˶��������Ŀ����ֱ�߾���
	distanceCal(Base_CMD.aimx, Base_CMD.aimy ,Coordinate.point.X, Coordinate.point.Y, &(Motion.distance), &Line_Ward);
		
	/*  �������յ�ջ�״̬  */
	if(Motion.distance <= End_Distance)
	{
		if(!Motion.isENDLoop)	//���ν��룬���л�״̬
		{
			ResetI(&pidData_End);//�������
			Motion.isENDLoop = true;
			Vol_Curve.Arrive_flag = ARRIVE;
		}
			
		END_Loop(Motion.distance);	
		
		Motion.outSpeed = EndSpeed;
		Motion.Ward = Line_Ward;
			
		if((Base_CMD.aimy - Coordinate.point.Y) < 0)	//��yΪ��ֵ��˵���ӵ�ǰ��ָ��Ŀ���������ڵ���/�����ޣ������ٶ�Ϊ��ֵ
		{
			Motion.outSpeed = -Motion.outSpeed;
		}
	
	}
	//����һ�ִ���״̬�����ǻ�������Ϊ�ٶȹ�����ղ�ס���³���յ�ջ���������
	else if(Motion.distance > End_Distance && ARRIVE == Vol_Curve.Arrive_flag)
	{
		Motion.outSpeed = 300;
		Motion.Ward = Line_Ward;
			
		if((Base_CMD.aimy - Coordinate.point.Y) < 0)	//��yΪ��ֵ��˵���ӵ�ǰ��ָ��Ŀ���������ڵ���/�����ޣ������ٶ�Ϊ��ֵ
		{
			Motion.outSpeed = -Motion.outSpeed;
		}
	}
	/*  ��δ�����յ�ջ�����  */
	else
	{	
		if(Motion.isENDLoop)		//���ν��룬���л�״̬
		{
			ResetI(&pidData_Time); 	//�������
			Motion.isENDLoop = false;
		}
		
		CurveLoop(Motion.distance);
		
		if((Base_CMD.aimy - Coordinate.point.Y)<0)	//��yΪ��ֵ��˵���ӵ�ǰ��ָ��Ŀ���������ڵ���/�����ޣ������ٶ�Ϊ��ֵ
		{
			Vol_Curve.CurveSpeed = -Vol_Curve.CurveSpeed;
		}
							
		MixSpeed((double)(Vol_Curve.CurveSpeed), Line_Ward, AdjSpeed, MOV_Line, Coordinate, &(Motion.outSpeed), &(Motion.Ward));

	}
}

/**
  * @brief  ���㣺ң��ģʽ
  * @param  None
  * @retval None
  */
void OpenMode_Loop(void)
{
	Motion.isENDLoop = false;
	
	Motion.outSpeed = Base_CMD.line_speed;
	Motion.Ward = Base_CMD.ward;
}

/************************ (C) COPYRIGHT 2017 YANGYIFAN *****END OF FILE****/
