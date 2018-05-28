#ifndef _INTERFACETOMCROPC_H_
#define _INTERFACETOMCROPC_H_

#include "Macros.h"
#include <stdint.h>


///********************��ֵ��*******************************/
///*    key1    */
//#define BACK              0x40
//#define START             0x80
//#define X_BUTTON                 0x04
//#define Y_BUTTON               0x08
//#define B_BUTTON                 0x02
//#define A_BUTTON               0x01
//#define LB                0x10
//#define RB                0x20
///*     key2    */
//#define HOME              0x01
//#define L3                0x02
//#define R3                0x04
///***********************************************************/

///******************ָ���************************************/
//#define NORM_STEP1   0x07  //��ͨ�򽻽�1��Ӧ�Ķ�����
//#define R_PUSH   0x03  //�����Ӧ�Ķ�����
//#define R_FETCH   0x04  //���ն�Ӧ�Ķ�����
//#define BOTH_PUSH    0x03  //ȫȡ��Ӧ�Ķ�����
//#define BOTH_FETCH  0x04  //ȫ�ն�Ӧ�Ķ�����
//#define L_FETCH   0x04  //���ն�Ӧ�Ķ�����
//#define L_PUSH   0x03  //�����Ӧ�Ķ�����
//#define NORM_STEP2   0x08  //��ͨ�򽻽�2��Ӧ�Ķ�����
//#define GOLDEN_HANDOVER   0x09  //��ɫ�򽻽Ӷ�Ӧ�Ķ�����
///************************************************************/

///***********************�������******************************/
//#define LEFT        0x01
//#define RIGHT       0x02
//#define BOTH        0x03
//#define SPECIAL     0x05   //��϶�����
///************************************************************/

typedef union {

struct PC_Buffer
{
	uint8_t Data_head1;				//�̶�����ͷ1(0xaa)
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	uint8_t flag;					//��־
	uint8_t place_holder;
	float dockDModule;
	float dockArgument;
	float dockRAngle;
	
	float xAngle;
	float yAngle;
	float xDis;
	float yDis;
		 
	uint8_t key_flag;        //������־
	uint8_t key_flag2;
	int8_t rotate;
	uint8_t ward;
	uint32_t verify;				//У�飨��ͣ�
	
}buffer;

uint8_t Bytes[sizeof(struct PC_Buffer)];

}PC_Rx_BufferType;

typedef union {

struct PC_TxBuf
{
	uint8_t Data_head1;				//�̶�����ͷ1��0xaa��
	uint8_t Data_head2;				//�̶�����ͷ2(0xbb)
	
	int16_t positionx;
	int16_t positiony;
	int16_t angle;
	
	uint8_t flag[4];
	
	uint32_t verify;				//У�飨��ͣ�
}TxBuffer;

uint8_t Bytes[sizeof(struct PC_TxBuf)];

}PC_Msg_Tx_BufferType;


//��ӽṹ��
typedef struct
{
	int16_t RobotSpeed;    
	int16_t RobotWard;   
	int16_t RockerPosAngel;  
}SendBuffer;




/* Public  variables ----------------------*/
extern PC_Rx_BufferType		PC_RxBuffer[2];
extern PC_Msg_Tx_BufferType	PC_Tx_Buffer;
extern uint8_t 				PC_BufferFlag;
extern int16_t 				PC_DataHeadCorrect;
extern uint8_t KEY1_value;
extern uint8_t KEY2_value;
extern uint8_t Rotate_value;
extern uint8_t caculed_ward;

void SendDataToPC(void);
void ReceiveDataFromPC_Buffer(const PC_Rx_BufferType CMDData);
void Robot_State(uint8_t SpeedUp, int8_t ADC_Gather[3]);
uint32_t T_cal();




#endif
