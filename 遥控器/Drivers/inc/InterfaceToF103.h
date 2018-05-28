#ifndef _INTERFACEF103_H_
#define _INTERFACEF103_H_

#include <stdint.h>


typedef union {

struct Buffer
{
	uint8_t Data_head1;				//固定数据头1（0xaa）
	uint8_t Data_head2;				//固定数据头2(0xbb)
	
	uint8_t CMD_type;				//指令类型（标识后面的PID参数是否有效）
	uint8_t Data_cnt;				//数据长度
	
	int16_t max_vol;				//最大速度
	float max_a;					//最大加速度
	
	int16_t AimSpeed1;				//1号轮转速
	int16_t AimSpeed2;				//2号轮转速
	int16_t AimSpeed3;				//3号轮转速
	
	int16_t RealSpeed1;				//1号轮转速
	int16_t RealSpeed2;				//2号轮转速
	int16_t RealSpeed3;				//3号轮转速
	
	int16_t PID_Current_P;			//电流环参数——P
	int16_t PID_Current_I;			//电流环参数——I
	int16_t PID_Current_D;			//电流环参数——D
	
	int16_t PID_Angle_P;			//角度环参数——P
	int16_t PID_Angle_I;			//角度环参数——I
	int16_t PID_Angle_D;			//角度环参数——D
	
	uint32_t verify;				//校验（求和）
	
}buffer;

uint8_t Bytes[sizeof(struct Buffer)];

}F1_Rx_BufferType;

typedef union {

struct TxBuf
{
	uint8_t Data_head1;				//固定数据头1（0xaa）
	uint8_t Data_head2;				//固定数据头2(0xbb)
	
	uint8_t CMD_type;				//指令类型（标识后面的PID参数是否有效）
	uint8_t Data_cnt;				//数据长度
	
	int16_t lineSpeed;				//移动速度
	int16_t Ward;					//移动方向
	int16_t Aim_PosAngle;			//目标姿态角
	int16_t AngleCnt;				//圈数计数器
	int16_t PosAngle;				//实际姿态角
	
	int16_t PID_Current_P;			//电流环参数——P
	int16_t PID_Current_I;			//电流环参数——I
	int16_t PID_Current_D;			//电流环参数——D
	
	int16_t PID_Angle_P;			//角度环参数——P
	int16_t PID_Angle_I;			//角度环参数——I
	int16_t PID_Angle_D;			//角度环参数——D
	
	uint32_t verify;				//校验（求和）
}TxBuffer;

uint8_t Bytes[sizeof(struct TxBuf)];

}F1_Msg_Tx_BufferType;


typedef struct				//接收到的轮子转速结构体
{
	int16_t v1;
	int16_t v2;
	int16_t v3;

}wheel_speed;

/* Public  variables ----------------------*/
extern wheel_speed 			AimSpeed;
extern wheel_speed 			RealSpeed;
extern F1_Rx_BufferType		F1_RxBuffer[2];
extern F1_Msg_Tx_BufferType	F1_Tx_Buffer;
extern uint8_t 				F1_BufferFlag;
extern uint8_t 				F1_PID_flag;
extern int16_t 				F1_DataHeadCorrect;

void SendDataToF103(uint8_t PIDflag);
void ReceiveDataFromF103_Buffer(const F1_Rx_BufferType CMDData);

#endif

