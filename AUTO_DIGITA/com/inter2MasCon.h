#ifndef __INTER2MAS__H
#define __INTER2MAS__H

#include "uartdma.h"
#include "lightbeep.h"
#include "act.h"
#include "process.h"


typedef union 
{
	struct Buffer
	{
		uint8_t head1;				//固定数据头1（0xaa）
		uint8_t head2;				//固定数据头2(0xbb)
		
		uint8_t cnt;				//数据长度
		uint8_t cylGroup;			//气缸组
		uint8_t cylAct;       		//气缸动作
		
		uint8_t LaserSignal;		//关于激光的相关信号
		uint8_t Ball_serial_num;  //绣球编号
		uint8_t Current_State;				//手动车位置	//状态
		uint8_t act_flag;				//遥控指令
		uint8_t ACK_From_Master;				//指令反馈
		
		uint32_t verify;			//校验（求和）
	}Rxbuffer;

	uint8_t Bytes[sizeof(struct Buffer)];

}CMD_BufferType;

typedef union 
{
	struct TxBuf
	{
		uint8_t head1;					//固定数据头1（0xaa）
		uint8_t head2;					//固定数据头2(0xbb)	
		uint8_t cnt;					//数据长度		
		uint16_t x;
		uint16_t y;
		uint16_t angle;
		uint8_t Flag;  //各种标志位，包含激光信号有效，应答，动作完成标志
		uint8_t ballNum;				//车上球数	
		uint32_t verify;				//校验（求和）
	}TxBuffer;

	uint8_t Bytes[sizeof(struct TxBuf)];

}Msg_Tx_BufferType;

extern CMD_BufferType		CMDBuffer[2];
extern Msg_Tx_BufferType	Tx_Buffer;
extern uint8_t 				BufferFlag;
extern int16_t 				DataHeadCorrect;

void ReceiveDataFromCtrl_Buffer(const CMD_BufferType* CMDData);
void SendDataToMasCon(void);

#endif
