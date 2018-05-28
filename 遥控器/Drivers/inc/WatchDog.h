#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include <stdint.h>

typedef enum				
{
	Blocked = 0,
	Unblocked = 1,	
	
}BlockTypes;

typedef enum				
{
	Master = 0,
	F103,
//	MicroPC,
//	Equips,
	Map,
	FlagsNum
	
}FlagsTypes;

typedef struct 
{
	uint8_t State;								//当前状态
	uint8_t LastState;							//上次状态
	
}Flags;

void WatchDogInit(void);
void FeedDog(FlagsTypes flag);
void ShutUp(void);
void Bark(void);

#endif
