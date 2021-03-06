#ifndef _PID_H_
#define _PID_H_

#include "STM32F4xx.h"

struct PID
{
	uint16_t kP;
	uint16_t kI;
	uint16_t kD;
	uint16_t dt;
	int32_t P;
	int32_t I;
	int32_t D;
	int32_t set;
	int32_t feedback;
	int32_t last_error;
	int32_t error;
	int32_t derror;
	int32_t Imax;
	int32_t outmax;
	int32_t out;
};

typedef union
{
	short int value;
	char value_byte[2];
}value;


void PIDparamUpdate(struct PID* pidData,uint16_t kp,uint16_t ki,uint16_t kd,int32_t imax,int32_t outmax);
void PIDdataUpdate(struct PID* pidData);
void ResetI(struct PID* pidData);
void GetP(struct PID* pidData);
void GetI(struct PID* pidData);
void GetD(struct PID* pidData);
void GetPI(struct PID* pidData);
void GetPID(struct PID* pidData);


#endif

