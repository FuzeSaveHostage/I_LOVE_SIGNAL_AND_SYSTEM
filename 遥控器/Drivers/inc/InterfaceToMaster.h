#ifndef _INTERFACETOMASTER_H_
#define _INTERFACETOMASTER_H_

#include <stdint.h>

typedef union {

struct CMD_Buffer
{
	uint8_t Data_head1;				//固定数据头1(0xaa)
	uint8_t Data_head2;				//固定数据头2(0xbb)
	
	uint8_t CMD;					//指令标志
	uint8_t CMD2;					//扩充指令标志
	uint8_t MovePoint;				//运动点类型
	uint8_t Cylinders;				//气缸控制(执行动作的气缸标号)
	uint8_t Action;					//动作控制
	uint8_t CastBallflag;			//抛球成功标志
	
	int16_t Aim_X;					//目标X坐标
	int16_t Aim_Y;					//目标Y坐标
	int16_t Aim_t;					//预期时间
	
	int16_t LineSpeed;				//移动速度
	int16_t Ward;					//移动方向
	int16_t Pos_Angle;				//姿态角
	
	int16_t Current_P;				//电流环参数——P
	int16_t Current_I;				//电流环参数——I
	int16_t Current_D;				//电流环参数——D
	
	int16_t Angle_P;				//角度环参数——P
	int16_t Angle_I;				//角度环参数——I
	int16_t Angle_D;				//角度环参数——D
	
	int16_t Location_P;				//位置环参数——P
	int16_t Location_I;				//位置环参数——I
	int16_t Location_D;				//位置环参数——D
	
	int16_t Time_P;					//时间环参数——P
	int16_t Time_I;					//时间环参数——I
	int16_t Time_D;					//时间环参数——D
	
	int16_t END_P;					//终点环参数——P
	int16_t END_I;					//终点环参数——I
	int16_t END_D;					//终点环参数——D
	
	uint32_t verify;					//校验（求和）
	
}buffer;

uint8_t Bytes[sizeof(struct CMD_Buffer)];

}CMD_BufferType;

typedef union {

struct TxBuf_ToMaster
{
	uint8_t Data_head1;				//固定数据头1（0xaa）
	uint8_t Data_head2;				//固定数据头2(0xbb)
	
	int16_t Aim_Angle;				//目标姿态角
	int16_t Real_Angle;				//实时姿态角
	int16_t Aim_Displace;			//目标位移
	int16_t Real_Displace;			//实际位移
	int16_t Pos_Error;				//与预期坐标的位移误差
	
	int16_t Wheel_Speed1;			//轮子1转速
	int16_t Wheel_Speed2;			//轮子1转速
	int16_t Wheel_Speed3;			//轮子1转速
	
	int16_t OutSpeed;				//输出速度
	
	int16_t Real_X;					//实际坐标x
	int16_t Real_Y;					//实际坐标y
	
	uint16_t Real_t;				//计算获得的实际运动时间
	
	struct Mode						//模式标识
	{
		unsigned CtrlMode:1;		//控制模式
		unsigned Endflag:1;			//终点闭环标志
		unsigned MotionOverflow:1;	//运动指令溢出标志（给的指令是否超出运动能力，1为超出）
		unsigned reverse:5;			//预留
		
	}flags;
	
	uint32_t verify;					//校验（求和）
}TxBuffer;

uint8_t Bytes[sizeof(struct TxBuf_ToMaster)];

}Msg_Tx_BufferType;

typedef struct 
{
	uint16_t p;
	uint16_t i;
	uint16_t d;
}PID_Value;


struct PIDPara
{
	PID_Value Current;
	PID_Value Angle;
	PID_Value Location;
	PID_Value Time;
	PID_Value End;
	
};

/* Public  variables ----------------------*/
extern unsigned char 	Rcv_Ready;
extern struct PIDPara 	PIDSetting;				//PID数据内容

extern CMD_BufferType		CMDBuffer[2];
extern Msg_Tx_BufferType	Tx_Buffer;
extern uint8_t 				BufferFlag;
extern uint8_t 				PID_flag;
extern int16_t 				DataHeadCorrect;

/**		数据接收		**/
void ReceiveDataFromMaster_Buffer(const CMD_BufferType CMDData);

/**		数据反馈		**/
void SendDataToMaster(void);



#endif

