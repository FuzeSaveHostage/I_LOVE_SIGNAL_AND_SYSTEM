#include "schedule.h"

inline void Schedule(uint32_t i)
{
	if(!(i%10))
	{
		Task10Ms();
	}
	if(!(i%20))
	{
		Task20Ms();
	}
	if(!(i%30))
	{
		Task30Ms();
	}
}

inline static void Task10Ms()
{
	CheckCommunication(10);

	if(fPROCESS.fHANDORAUTO == AUTO)
		AutoProcess();
	else if(fPROCESS.fHANDORAUTO == HAND)
		HandProcess();
}

inline static void Task20Ms()
{
	//LightAct(&lightBarPara);
	SendDataToMasCon();
}

inline static void Task30Ms()
{
	Send2AssCon(&assConActSet);
}
