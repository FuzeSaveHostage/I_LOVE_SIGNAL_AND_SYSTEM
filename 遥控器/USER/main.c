#include "InterfaceToMaster.h"
#include "Process.h"
#include "Base.h"

void delay_ms(unsigned int t);
 
/**
  * @brief  主函数入口
  * @param  None
  * @retval None
  */
int main(void)
{
	baseInit();									//初始化底盘
	
//	CarryOutAction((enum SpecialActions)3, 1);
	
	while(1)
	{
		PromptToneScan();						//提示音扫描
		
		if(NullAction != Act_flag)				//执行特殊动作
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
