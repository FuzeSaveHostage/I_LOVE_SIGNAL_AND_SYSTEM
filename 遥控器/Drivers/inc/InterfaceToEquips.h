#ifndef _INTERFACETOEQUIP_H_
#define _INTERFACETOEQUIP_H_

#include "Map.h"
#include <stdint.h>

/** @defgroup CylinderGroups Type
  * @{
  */

#define Null_finished 	0x00
#define Left_finished 	0x01
#define Right_finished 	0x02
#define Spin_finished 	0x04

/**
  * @}
  */

typedef struct
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	
	uint8_t Light;
	
}LightType;

typedef union {

struct Buffer
{
	uint8_t Data_head1;				//固定数据头1（0xaa）
	uint8_t Data_head2;				//固定数据头2(0xbb)
	
	uint8_t Data_cnt;				//数据长度
	
	int16_t Coord_X;				//坐标X
	int16_t Coord_Y;				//坐标Y
	int16_t Coord_Angle;			//朝向角
	uint8_t CoordValid;				//1为坐标有效
	
	uint8_t PressureState;			//气压值
	uint8_t CylinderState;			//气缸状态
	
	uint8_t ACK;					//指令应答 -- 标志着行为指令已接收到(未动作)
	uint8_t ActFinished;			//动作已完成标志
	uint8_t HaveBallCnt;			//标记自己手上还有几个球
	
	uint32_t verify;				//校验（求和）
	
}buffer;

uint8_t Bytes[sizeof(struct Buffer)];

}Eq_Rx_BufferType;

typedef union {

struct TxBuf
{
	uint8_t Data_head1;				//固定数据头1（0xaa）
	uint8_t Data_head2;				//固定数据头2(0xbb)
	
	uint8_t Data_cnt;				//数据长度
	
	LightType Light;				//灯条控制
	
	uint8_t CylinderGroups;			//动作的气缸组
	uint8_t CylinderAction;			//气缸动作
	
//	uint8_t BasicCylinderGroup;		//高优先级->气缸组
//	uint8_t BasicCylinderType;		//高优先级->气缸类型
//	uint8_t BasicCylinderState;		//高优先级->气缸状态
	
	uint8_t SetAirPressure;			//设置发射装置气压
	uint8_t SpinPosition;			//旋转位置
	uint8_t LaserRequest;			//激光数据请求
	
	uint8_t PointCnt;				//自动点计数器，指定当前机器人状态和行为 -- 流程模式
	uint8_t DoAction;				//动作指令 -- 流程模式
	
	uint8_t Ack;					//标志应答
	
	uint32_t verify;				//校验（求和）
}TxBuffer;

uint8_t Bytes[sizeof(struct TxBuf)];

}Eq_Msg_Tx_BufferType;

/* Public  variables ----------------------*/
extern Eq_Rx_BufferType		Eq_RxBuffer[2];				//双接收缓冲区
extern Eq_Msg_Tx_BufferType	Eq_Tx_Buffer;
extern uint8_t				Eq_BufferFlag;				//目前使用的缓冲区标号
extern int16_t 				Eq_DataHeadCorrect;			//数据头位置矫正值

extern bool Eq_isSendingflag;

void InitCylinderState(void);
void SendDataToEquips(void);
void ReceiveDataFromEquips_Buffer(const Eq_Rx_BufferType CMDData);

#endif

