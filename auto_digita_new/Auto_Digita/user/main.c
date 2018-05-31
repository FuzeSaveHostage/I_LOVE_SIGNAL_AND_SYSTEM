#include "bsp.h"
#include "act.h"
#include "process.h"


int main()
{
	SysInit();

	BEEP_InitSucceed();

	while(1)
	{
		if(fCOM == LOST)
		{
			BEEP_ConnectBreak();
		}
		else
		{
			if(fPROCESS.fHANDORAUTO == HAND)
			{
				if(fPROCESS.cylAct)
				{
					fPROCESS.pHandAct();//执行动作，函数指针在调度器更新
					HandActFinish();
				}
			}
			else if(fPROCESS.fHANDORAUTO == AUTO)
			{
				if(fPROCESS.act)
				{
					fPROCESS.pAutoAct();
					AutoActFinish();
				}
			}

			//可添加新任务，在未收到开始动作指令时执行，执行动作时不执行
		}
	}
}
