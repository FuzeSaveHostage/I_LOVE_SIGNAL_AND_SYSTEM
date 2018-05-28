#include "PositionCtrl.h"
#include "Motion_Ctrl.h"
#include <math.h>

/* Private  macro -------------------------------------------------------------*/
#define LimitOutput(in, low, high) ((in)<(low)?(low):((in)>(high)?(high):(in)))		//����޷�	
#define ANGLE_TO_RAD(x) (x)/180.0f*3.14159f
#define RAD_TO_ANGLE(x) (x)/3.14159f*180.0f

/* Public  variables ---------------------------------------------------------*/
int32_t End_Distance = 50;				//���Ⱦ��뵥λ��mm

struct MoveInLine MOV_Line;				//ֱ�߹켣�ջ�����
int32_t AdjSpeed;						//�����ٶ�
int32_t EndSpeed;						//�յ��ٶ�

/**
  * @brief  Ϊ��ƽ�������յ�ջ���ֱ���˶��յ㡣�����յ�ջ�����,�������ٶ����ߺ����
  * @param  a_calcu:�ٶ���������õļ��ٶ�
  * @param  PIDparam:�յ�ջ���PID�α���
  * @retval ��
  */
void getEndDistance(double a_calcu, struct PID PIDparam)
{
	double temp = PIDparam.kP / 100.0;		//����ʵ��PID���ֵ�������������ϵ��õ�100����ϵ��
	
	End_Distance = (uint32_t)(2 * a_calcu / (temp * temp));
}

/**
  * @brief  ����ֱ�߽���ʽ(kx - y + (y1 - kx1) = 0)
  * @param  Line:ֱ�߹켣�ջ�����ָ��
  * @retval ��
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
	
	Line->LineAngle = (Line->LineAngle<0.0 ? (Line->LineAngle + 180.0) : Line->LineAngle);//������ת����0-180
	
	Line->AdjAngle = Line->LineAngle - 90.0;
	
	Line->AdjAngle = (Line->AdjAngle) < 0.0 ? (Line->AdjAngle  + 180.0) : Line->AdjAngle;//������ת����0-180
	
	Line->line = CreateLine(Line->StartPoint, Line->EndPiont);

}

/**
  * @brief  �˶��е�λ�ñջ�����ֱ��
  * @param  Line:ֱ�߹켣�ջ�����
  * @retval ��
  */
void Line_Location_Loop(const struct MoveInLine Line)
{
	Point temp;
	
	temp.X = Coordinate.point.X;
	temp.Y = Coordinate.point.Y;
	
	//Ŀ����0
	pidData_Location.set = 0;
	
	//���ʵʱ����GetDistanceToLine(temp, Line.line);
	pidData_Location.feedback = (int32_t)GetDistanceToLine(temp, Line.line);
	
	//����PID����
	PIDdataUpdate(&pidData_Location);

	//����PID
	GetPID(&pidData_Location);
	
	//�õ������
	AdjSpeed = -LimitOutput(pidData_Location.out, -PID_Location_OUT_MAX, PID_Location_OUT_MAX);
}

/**
  * @brief  �ٶ��ںϡ���ֱ���˶�λ�ñջ�(��ԭ���н�����Ϊ��׼������зֽ�ͺϳ�)
  * @param  LineSpeed:���ٶ����߻�õ�ֱ���ٶ�
  * @param  LineWard:��ǰ����ָ��Ŀ���ĽǶ�
  * @param  adjSpeed:λ�ò����ٶ�
  * @param  Line:ֱ��λ�ñջ�����
  * @param  Coor:ʵʱ����
  * @param  SpeedOut:ָ������ٶȵ�ָ��
  * @param  WardOut:ָ������ٶȷ���ǵ�ָ��
  * @retval ��
  */
void MixSpeed(double LineSpeed, double LineWard,
				double adjSpeed,struct MoveInLine Line, Coord Coor,
				int32_t* SpeedOut, double* WardOut)
{
	double AngleDiffer = 0;
	double tempSpeed = 0, tempAngle = LineWard;
	
	/*  ����ʽΪ(kx - y + (y1 - kx1) = 0)  */
	if((Line.line.A * Coor.point.X + Line.line.C) < Coor.point.Y)//������ֱ���Ϸ�
	{
		Line.AdjAngle -= 180.0;
	}
	if(LineSpeed < 0)						//������ٶ�Ϊ��
	{
		LineSpeed = -LineSpeed;
		tempAngle -= 180.0;
		LineWard -= 180.0;
	}
	
	AngleDiffer = Line.AdjAngle - tempAngle;	//��ȡ���
	
	/*  �ֽ⡢�����ٶ�  */
 	LineSpeed += adjSpeed * cos(ANGLE_TO_RAD(AngleDiffer));
	tempSpeed = adjSpeed * sin(ANGLE_TO_RAD(AngleDiffer));//��ô�ֱ��ԭ��ֱ���ٶȷ���ķ��ٶ�
	
	/*  �ϳ��ٶȴ�С  */
	*SpeedOut = (int32_t)sqrt(LineSpeed * LineSpeed + tempSpeed * tempSpeed);
	
	/*  ��ȡ��� �ٶȷ���  */
	if(fabs(LineSpeed) < 0.000001)				//���Ͱ�ȫ
	{
		tempAngle= 90;
	}
	else
	{
		tempAngle = RAD_TO_ANGLE(atan(tempSpeed / LineSpeed));
		
		tempAngle = tempAngle < 0 ? (tempAngle + 180.0) : tempAngle;		//�ķ�ΧΪ0->180

	}
	
	if(tempSpeed < 0.0)										//���Ƕȷ������Ϊ-180->180�ķ�Χ
	{
		tempAngle = tempAngle - 180;
	}
	
	/*  �ϳɾ����ٶȷ���  */
	tempAngle = LineWard + tempAngle;									
	
	tempAngle = fmod(tempAngle, 360.0);									//���࣬������ֵ��Χ
	
	if(tempAngle < 0.0)													//���Ƕȷ������Ϊ-180->180�ķ�Χ
	{
		tempAngle += 180;						
		
		*SpeedOut = -*SpeedOut;											//���ٶ��ڵ��������ޣ���Ϊ��ֵ
	}
	
	*WardOut = tempAngle;												//������Ʊ���
}

/**
  * @brief  �յ�PID
  * @param  distan:��ǰλ�õ�ԭ��λ�õ�ƫ�distance��
  * @retval None
  */
void END_Loop(double distan)
{
	//Ŀ����0
	pidData_End.set = 0;
	
	//���ʵʱ����
	pidData_End.feedback = distan;
	
	/* �жϵ�ǰ״̬�Ƿ�����ͣ���� */
	if (distan < END_ERR_ALLOW)
	{
		ResetI(&pidData_End);//�������
		EndSpeed = 0;
	}
	else
	{
		if(distan < END_Tiny_DISTANCE)		//�������С�յ�����
		{
			//����PID����
			PIDdataUpdate(&pidData_End);
		
			//����PID
			GetPID(&pidData_End);
			
			//�õ������
			EndSpeed = -LimitOutput(pidData_End.out, -PID_END_OUT_MAX, PID_END_OUT_MAX);
		}
		else								//����ڴ��յ�����
		{
			//�õ������
			EndSpeed  = 300;
		}
		
		
	}
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
