#ifndef _INTERFACEF103_H_
#define _INTERFACEF103_H_

#include <stdint.h>


typedef union {

struct Buffer
{
	uint8_t Data_head1;				//�̶�����ͷ1��0xaa��
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	uint8_t CMD_type;				//ָ�����ͣ���ʶ�����PID�����Ƿ���Ч��
	uint8_t Data_cnt;				//���ݳ���
	
	int16_t max_vol;				//����ٶ�
	float max_a;					//�����ٶ�
	
	int16_t AimSpeed1;				//1����ת��
	int16_t AimSpeed2;				//2����ת��
	int16_t AimSpeed3;				//3����ת��
	
	int16_t RealSpeed1;				//1����ת��
	int16_t RealSpeed2;				//2����ת��
	int16_t RealSpeed3;				//3����ת��
	
	int16_t PID_Current_P;			//��������������P
	int16_t PID_Current_I;			//��������������I
	int16_t PID_Current_D;			//��������������D
	
	int16_t PID_Angle_P;			//�ǶȻ���������P
	int16_t PID_Angle_I;			//�ǶȻ���������I
	int16_t PID_Angle_D;			//�ǶȻ���������D
	
	uint32_t verify;				//У�飨��ͣ�
	
}buffer;

uint8_t Bytes[sizeof(struct Buffer)];

}F1_Rx_BufferType;

typedef union {

struct TxBuf
{
	uint8_t Data_head1;				//�̶�����ͷ1��0xaa��
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	uint8_t CMD_type;				//ָ�����ͣ���ʶ�����PID�����Ƿ���Ч��
	uint8_t Data_cnt;				//���ݳ���
	
	int16_t lineSpeed;				//�ƶ��ٶ�
	int16_t Ward;					//�ƶ�����
	int16_t Aim_PosAngle;			//Ŀ����̬��
	int16_t AngleCnt;				//Ȧ��������
	int16_t PosAngle;				//ʵ����̬��
	
	int16_t PID_Current_P;			//��������������P
	int16_t PID_Current_I;			//��������������I
	int16_t PID_Current_D;			//��������������D
	
	int16_t PID_Angle_P;			//�ǶȻ���������P
	int16_t PID_Angle_I;			//�ǶȻ���������I
	int16_t PID_Angle_D;			//�ǶȻ���������D
	
	uint32_t verify;				//У�飨��ͣ�
}TxBuffer;

uint8_t Bytes[sizeof(struct TxBuf)];

}F1_Msg_Tx_BufferType;


typedef struct				//���յ�������ת�ٽṹ��
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

