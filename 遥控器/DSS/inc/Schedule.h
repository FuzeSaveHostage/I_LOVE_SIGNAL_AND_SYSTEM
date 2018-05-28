#ifndef 	__SCHEDULE_H__
#define 	__SCHEDULE_H__

#include <stdint.h>

struct Scheduler
{
	uint16_t cnt_1ms;
	uint16_t cnt_5ms;
	uint16_t cnt_10ms;
	uint16_t cnt_20ms;
	uint16_t cnt_50ms;
	
};

extern struct Scheduler scheduler;

void Schedule_TIM_Init(void);
void Loops(void);

#endif

