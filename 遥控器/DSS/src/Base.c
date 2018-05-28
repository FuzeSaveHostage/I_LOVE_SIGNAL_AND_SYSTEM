#include "Motion_Ctrl.h"
#include "ActionCtrl.h"
#include "Vol_curve.h"
#include "Schedule.h"
#include "WatchDog.h"
#include "Process.h"
#include "USART.h"
#include "Base.h"
#include "Beep.h"
#include "Map.h"

struct CMD 	Base_CMD;
Base_flags 	Baseflags = {false, false, true};
enum AirPressureStateType AirPressureState = AirNorm_TZ1;

/**
  * @brief  ��ʼ��������
  * @param  ��
  * @retval ��
  */
void CtrlPara_Init(void)
{
	Base_CMD.R_Ctrl_Flag = OpenLoopMode;	//Ĭ��Ϊ��������
	
	Base_CMD.aimx = 0;
	Base_CMD.aimy = 0;
		
	Base_CMD.line_speed = 0;
	Base_CMD.ward = 0;
	
	Base_CMD.Pos_Angle = 0;
	
	Base_CMD.Angle_cnt = 0;
}

/**
  * @brief  ���̳�ʼ��
  * @param  ��
  * @retval ��
  */
void baseInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	InitCylinderState();
	
	USART_Config();								//���ڳ�ʼ��
	
	PIDInit();									//PID������ʼ��
		
	MovePointsInit(MovePoint);					//��ʼ���˶���
		
	CtrlPara_Init();							//��������ʼ��
	
	CurveParamInit();							//�ٶ����߲�����ʼ��
		
	BeepInit();									//��������ʼ��
	
//	while(false == Baseflags.InitSuccess);
	
	WatchDogInit();								//ģ�⿴�Ź���ʼ��
	
	Schedule_TIM_Init();						//���ȶ�ʱ����ʼ��	
	
//	LightCtrl(0, 0, 255, 100);					//�趨����Ϊ��ɫ
	
//	SpinDeviceCtrl(Spin_GetBall);				//�趨��תװ�ó�ʼλ��

}

/**
  * @brief  ��ʾ��ɨ��
  * @param  None
  * @retval None
  */
void PromptToneScan(void)
{
	
	if(true == Baseflags.RcvPIDSuccess)									//������ʾ��
	{
		Baseflags.RcvPIDSuccess = false;
		InitCompleteFlag(PIDUpdate);
	}
	
	if(true == Baseflags.restart && true == Baseflags.InitSuccess)		//��ʼ�����
	{
		Baseflags.restart = false;
		InitCompleteFlag(Start);
	}
}

/**
  * @brief  ����Ŀ����̬��
  * @param  aimAngle:Ŀ����̬��
  * @retval ��
  */
void AimAngleUpdate(double aimAngle)
{
	Base_CMD.Pos_Angle = aimAngle;
				
	Base_CMD.Angle_cnt = Angle_cnt;	//���·���������ʱ��Ȧ������Ϊ֮����̬�Ǳջ��Ĳο�ֵ
}

/**
  * @brief  ���ֶ�ģʽ��Ŀ����������¼
  * @param  None
  * @retval None
  */
void saveAimPosition(void)
{
	Base_CMD.aimx = Coordinate.point.X;
	Base_CMD.aimy = Coordinate.point.Y;
}

/**
  * @brief  ����
  * @param  None
  * @retval None
  */
void Base_Loop(void)
{
	/*  ��λģʽ  */
	if(Base_CMD.R_Ctrl_Flag == AimMode)
	{
		AimMode_Loop();
	}
	/*  ����ң��ģʽ  */
	else if(Base_CMD.R_Ctrl_Flag == OpenLoopMode)
	{
		OpenMode_Loop();
		saveAimPosition();
	}
}
