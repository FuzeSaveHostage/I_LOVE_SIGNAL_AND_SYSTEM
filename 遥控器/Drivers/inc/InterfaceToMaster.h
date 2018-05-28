#ifndef _INTERFACETOMASTER_H_
#define _INTERFACETOMASTER_H_

#include <stdint.h>

typedef union {

struct CMD_Buffer
{
	uint8_t Data_head1;				//�̶�����ͷ1(0xaa)
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	uint8_t CMD;					//ָ���־
	uint8_t CMD2;					//����ָ���־
	uint8_t MovePoint;				//�˶�������
	uint8_t Cylinders;				//���׿���(ִ�ж��������ױ��)
	uint8_t Action;					//��������
	uint8_t CastBallflag;			//����ɹ���־
	
	int16_t Aim_X;					//Ŀ��X����
	int16_t Aim_Y;					//Ŀ��Y����
	int16_t Aim_t;					//Ԥ��ʱ��
	
	int16_t LineSpeed;				//�ƶ��ٶ�
	int16_t Ward;					//�ƶ�����
	int16_t Pos_Angle;				//��̬��
	
	int16_t Current_P;				//��������������P
	int16_t Current_I;				//��������������I
	int16_t Current_D;				//��������������D
	
	int16_t Angle_P;				//�ǶȻ���������P
	int16_t Angle_I;				//�ǶȻ���������I
	int16_t Angle_D;				//�ǶȻ���������D
	
	int16_t Location_P;				//λ�û���������P
	int16_t Location_I;				//λ�û���������I
	int16_t Location_D;				//λ�û���������D
	
	int16_t Time_P;					//ʱ�价��������P
	int16_t Time_I;					//ʱ�价��������I
	int16_t Time_D;					//ʱ�价��������D
	
	int16_t END_P;					//�յ㻷��������P
	int16_t END_I;					//�յ㻷��������I
	int16_t END_D;					//�յ㻷��������D
	
	uint32_t verify;					//У�飨��ͣ�
	
}buffer;

uint8_t Bytes[sizeof(struct CMD_Buffer)];

}CMD_BufferType;

typedef union {

struct TxBuf_ToMaster
{
	uint8_t Data_head1;				//�̶�����ͷ1��0xaa��
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	int16_t Aim_Angle;				//Ŀ����̬��
	int16_t Real_Angle;				//ʵʱ��̬��
	int16_t Aim_Displace;			//Ŀ��λ��
	int16_t Real_Displace;			//ʵ��λ��
	int16_t Pos_Error;				//��Ԥ�������λ�����
	
	int16_t Wheel_Speed1;			//����1ת��
	int16_t Wheel_Speed2;			//����1ת��
	int16_t Wheel_Speed3;			//����1ת��
	
	int16_t OutSpeed;				//����ٶ�
	
	int16_t Real_X;					//ʵ������x
	int16_t Real_Y;					//ʵ������y
	
	uint16_t Real_t;				//�����õ�ʵ���˶�ʱ��
	
	struct Mode						//ģʽ��ʶ
	{
		unsigned CtrlMode:1;		//����ģʽ
		unsigned Endflag:1;			//�յ�ջ���־
		unsigned MotionOverflow:1;	//�˶�ָ�������־������ָ���Ƿ񳬳��˶�������1Ϊ������
		unsigned reverse:5;			//Ԥ��
		
	}flags;
	
	uint32_t verify;					//У�飨��ͣ�
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
extern struct PIDPara 	PIDSetting;				//PID��������

extern CMD_BufferType		CMDBuffer[2];
extern Msg_Tx_BufferType	Tx_Buffer;
extern uint8_t 				BufferFlag;
extern uint8_t 				PID_flag;
extern int16_t 				DataHeadCorrect;

/**		���ݽ���		**/
void ReceiveDataFromMaster_Buffer(const CMD_BufferType CMDData);

/**		���ݷ���		**/
void SendDataToMaster(void);



#endif

