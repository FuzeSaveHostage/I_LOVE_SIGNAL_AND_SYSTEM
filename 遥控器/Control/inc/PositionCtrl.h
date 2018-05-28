#ifndef _POSITIONCTRL_H_
#define _POSITIONCTRL_H_


#include "PlaneAnalyticGeometry.h"
#include "Map.h"
#include "PID.h"


struct MoveInLine
{
	Point StartPoint;			//�������
	Point EndPiont;				//�յ�����
	
	double LineAngle;			//���ָ���յ���������������X������ļн�
	double AdjAngle;			//λ�ý����ٶȷ�������ֱ�ߵķ���ǣ�����һ���Ƕ����90�㣩
	
	Line line;					//ֱ�߹켣����ʽ
	
};

/* Public  macro --------------------------*/
#define END_DISTANCE		50				//�����յ�ջ��ľ��뷶Χ����λ:mm��
#define END_Tiny_DISTANCE	20				//����С�յ�ջ��ľ��뷶Χ����λ:mm��
#define END_ERR_ALLOW       0				//������յ���Χ����Ϊ���������Χ������ͣ����,��λ��mm��

/* Public  variables ----------------------*/
extern struct MoveInLine  MOV_Line;				//ֱ�߹켣�ջ�����
extern int32_t AdjSpeed;						//�����ٶ�
extern int32_t EndSpeed;						//�ȶ��ٶ�
extern int32_t End_Distance;					//��̬�յ�ջ�����

/*  �����䶯  */
void CreatMoveLine(struct MoveInLine* Line, double aimx, double aimy, Coord *coord);
void getEndDistance(double a_calcu, struct PID PIDparam);
void MixSpeed(double LineSpeed, double LineWard,
				double adjSpeed,struct MoveInLine Line, Coord Coor,
				int32_t* SpeedOut, double* WardOut);

/*  ��ʱ���õ�����  */
void Line_Location_Loop(const struct MoveInLine Line);
void END_Loop(double distance);

#endif
