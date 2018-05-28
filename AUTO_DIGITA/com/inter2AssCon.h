#ifndef __INTER2ASS__H
#define __INTER2ASS__H

#include "can.h"
#include "act.h"

void Send2AssCon(__ASS_CON_ACT_t* act);
void ReceiveFromAssBuffer(CanRxMsg* rx);


#endif
