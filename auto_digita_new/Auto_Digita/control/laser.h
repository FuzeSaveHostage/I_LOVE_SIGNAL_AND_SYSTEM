#ifndef __LASER__H
#define __LASER__H

#include "stm32f10x.h"
#include "uartdma.h"

#define		Laser_Open			0x01
#define 	Config_Scale   		0x02
#define		Bias1          		505
#define		Bias2				355
#define		Bias3				411
#define 	Degree				0.52359877         //激光2与激光3夹角30°
/*                 三个激光模块测得距离的结构体                 */
struct DISTANT
{
	int32_t distant1;
	int32_t distant2;
	int32_t distant3;
};

/*                 位姿共用体                  */
typedef union{
	struct POSITION
	{
		int16_t Pos_X;                //x方向坐标
		int16_t Pos_Y;								//y方向坐标
		int16_t Angle;								//自旋角
	}buffer;
	uint8_t Byte[6];
}Position;

extern Position Pos;

extern uint16_t USART1_RX_STA;
extern uint16_t USART3_RX_STA;
extern uint16_t UART4_RX_STA;
extern __align(8) uint8_t USART1_TX_BUF[20];
extern __align(8) uint8_t USART3_TX_BUF[20];
extern __align(8) uint8_t UART4_TX_BUF[20];
extern uint8_t USART1_RX_BUF[20];
extern uint8_t USART3_RX_BUF[20];
extern uint8_t UART4_RX_BUF[20];
extern uint8_t USART1_FLAG;
extern uint8_t USART3_FLAG;
extern uint8_t UART4_FLAG;

extern struct DISTANT Distant;

void u1_printf(char* fmt,...);
void u3_printf(char* fmt,...);
void u4_printf(char* fmt,...);
void open_laser(void);
void Measure(void);
void Calculate_Position(void);

#endif
