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
	uint8_t Data_head1;				//�̶�����ͷ1��0xaa��
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	uint8_t Data_cnt;				//���ݳ���
	
	int16_t Coord_X;				//����X
	int16_t Coord_Y;				//����Y
	int16_t Coord_Angle;			//�����
	uint8_t CoordValid;				//1Ϊ������Ч
	
	uint8_t PressureState;			//��ѹֵ
	uint8_t CylinderState;			//����״̬
	
	uint8_t ACK;					//ָ��Ӧ�� -- ��־����Ϊָ���ѽ��յ�(δ����)
	uint8_t ActFinished;			//��������ɱ�־
	uint8_t HaveBallCnt;			//����Լ����ϻ��м�����
	
	uint32_t verify;				//У�飨��ͣ�
	
}buffer;

uint8_t Bytes[sizeof(struct Buffer)];

}Eq_Rx_BufferType;

typedef union {

struct TxBuf
{
	uint8_t Data_head1;				//�̶�����ͷ1��0xaa��
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	uint8_t Data_cnt;				//���ݳ���
	
	LightType Light;				//��������
	
	uint8_t CylinderGroups;			//������������
	uint8_t CylinderAction;			//���׶���
	
//	uint8_t BasicCylinderGroup;		//�����ȼ�->������
//	uint8_t BasicCylinderType;		//�����ȼ�->��������
//	uint8_t BasicCylinderState;		//�����ȼ�->����״̬
	
	uint8_t SetAirPressure;			//���÷���װ����ѹ
	uint8_t SpinPosition;			//��תλ��
	uint8_t LaserRequest;			//������������
	
	uint8_t PointCnt;				//�Զ����������ָ����ǰ������״̬����Ϊ -- ����ģʽ
	uint8_t DoAction;				//����ָ�� -- ����ģʽ
	
	uint8_t Ack;					//��־Ӧ��
	
	uint32_t verify;				//У�飨��ͣ�
}TxBuffer;

uint8_t Bytes[sizeof(struct TxBuf)];

}Eq_Msg_Tx_BufferType;

/* Public  variables ----------------------*/
extern Eq_Rx_BufferType		Eq_RxBuffer[2];				//˫���ջ�����
extern Eq_Msg_Tx_BufferType	Eq_Tx_Buffer;
extern uint8_t				Eq_BufferFlag;				//Ŀǰʹ�õĻ��������
extern int16_t 				Eq_DataHeadCorrect;			//����ͷλ�ý���ֵ

extern bool Eq_isSendingflag;

void InitCylinderState(void);
void SendDataToEquips(void);
void ReceiveDataFromEquips_Buffer(const Eq_Rx_BufferType CMDData);

#endif

