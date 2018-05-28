#include "InterfaceToMaster.h"
#include "Process.h"
#include "Base.h"

void delay_ms(unsigned int t);
 
/**
  * @brief  ���������
  * @param  None
  * @retval None
  */
int main(void)
{
	baseInit();									//��ʼ������
	
//	CarryOutAction((enum SpecialActions)3, 1);
	
	while(1)
	{
		PromptToneScan();						//��ʾ��ɨ��
		
		if(NullAction != Act_flag)				//ִ�����⶯��
		{
			CarryOutAction(Act_flag, ActCylinders);
		}
		
		SpinDeviceCtrl(SpinState);
				
		if(AimMode == Base_CMD.R_Ctrl_Flag)
		{
			GoToSpecialPoints(MovePoint, &MovePointCnt, GotoThePoint);
			//MoveDecision(MovePoint, &MovePointCnt);
		}
	}
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
