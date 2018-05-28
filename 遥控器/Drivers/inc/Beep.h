#ifndef _BEEP_H_
#define _BEEP_H_

#include <stdint.h>
#include "Macros.H"

typedef enum
{
	Start,
	PIDUpdate,
	
}InitTypes;

extern bool BeepMutexflag;

void BeepInit(void);
void BeepingON(uint16_t frequency);
void BeepingOFF(void);
void InitCompleteFlag(InitTypes Type);

#endif
