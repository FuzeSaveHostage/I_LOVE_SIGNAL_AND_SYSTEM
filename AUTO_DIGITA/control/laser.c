#include "laser.h"
#include "stdarg.h"	 	 
#include "stdio.h"
#include "string.h"
#include "math.h"

/*                     串口接收标志位                  */
uint16_t USART1_RX_STA=0;
uint16_t USART3_RX_STA=0;
uint16_t UART4_RX_STA=0;
/*                     串口发送缓存                    */
__align(8) uint8_t USART1_TX_BUF[20];
__align(8) uint8_t USART3_TX_BUF[20];
__align(8) uint8_t UART4_TX_BUF[20];
/*                     串口接收缓存                    */
uint8_t USART1_RX_BUF[20];
uint8_t USART3_RX_BUF[20];
uint8_t UART4_RX_BUF[20];
/*                     串口标志字节                    */
uint8_t USART1_FLAG=0;
uint8_t USART3_FLAG=0;
uint8_t UART4_FLAG=0;


struct DISTANT Distant;
Position Pos;

/**
  * @brief  串口1(激光1)发送数据函数(这段我也不懂为啥,就是照开发板例程抄的)
  * @param  形参是想要发送的数据,可以直接是字符串也可以是数组名
  * @retval 无
  */
void u1_printf(char* fmt,...)
{
	uint16_t i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);
	for(j=0;j<i;j++)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
		USART_SendData(USART1,(uint8_t)USART1_TX_BUF[j]);
	}
}
/**
  * @brief  串口3(激光2)发送数据函数(这段我也不懂为啥,就是照开发板例程抄的)
  * @param  形参是想要发送的数据,可以直接是字符串也可以是数组名
  * @retval 无
  */
void u3_printf(char* fmt,...)
{
	uint16_t i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);
	for(j=0;j<i;j++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
		USART_SendData(USART3,(uint8_t)USART3_TX_BUF[j]);
	}
}
/**
  * @brief  串口4(激光3)发送数据函数(这段我也不懂为啥,就是照开发板例程抄的)
  * @param  形参是想要发送的数据,可以直接是字符串也可以是数组名
  * @retval 无
  */
void u4_printf(char* fmt,...)
{
	uint16_t i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)UART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART4_TX_BUF);
	for(j=0;j<i;j++)
	{
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
		USART_SendData(UART4,(uint8_t)UART4_TX_BUF[j]);
	}
}

/**
  * @brief  将字符串转化为int型变量,单位mm
  * @param  无
* @retval dist:测量的距离
  */
int32_t Trans(uint8_t* str)
{
	int32_t dist=0;
	uint8_t numb[7]={0};
	uint8_t i;
	for(i=0;i<7;i++)
	{
		numb[i]=str[i+3]-0x30;
	}
	dist=numb[0]*100000+numb[1]*10000+numb[2]*1000+numb[4]*100+numb[5]*10+numb[6];
	return dist;
}
/**
  * @brief  开启激光
  * @param  无
  * @retval 无
  */
void open_laser(void)
{
	uint8_t str1[]={0x80,0x06,0x05,0x01,0x74};//控制激光开机指令
	uint8_t str2[]={0xFA,0x04,0x09,0x0A,0xEF};//设置激光为10米指令
/*              开启激光之前清除一下接收标志位           */
	USART1_RX_STA=0;
	USART3_RX_STA=0;
	UART4_RX_STA=0;
/*              发送开启激光指令               */
	u1_printf((char*)str1);
	u3_printf((char*)str1);
	u4_printf((char*)str1);
/*              循环保证所有激光都已经开启             */
	while((USART1_FLAG&USART3_FLAG&UART4_FLAG)!=Laser_Open)
	{
		if(USART1_FLAG!=Laser_Open)
			u1_printf((char*)str1);
		if(USART3_FLAG!=Laser_Open)
			u3_printf((char*)str1);
		if(UART4_FLAG!=Laser_Open)
			u4_printf((char*)str1);
	}
/*              清除串口接收标志位             */
	USART1_RX_STA=0;           
	USART3_RX_STA=0;
	UART4_RX_STA=0;
/*              设置激光量程               */
	u1_printf((char*)str2);
	u3_printf((char*)str2);
	u4_printf((char*)str2);
/*               循环保证所有激光量程设置完成                */
	while((USART1_FLAG&USART3_FLAG&UART4_FLAG)!=Config_Scale)
	{
		if(USART1_FLAG!=Config_Scale)
			u1_printf((char*)str2);
		if(USART3_FLAG!=Config_Scale)
			u3_printf((char*)str2);
		if(UART4_FLAG!=Config_Scale)
			u4_printf((char*)str2);
	}
/*              清除串口接收标志位              */ 
	USART1_RX_STA=0;    
  USART3_RX_STA=0; 
	UART4_RX_STA=0; 
}
/**
  * @brief  单次测量
  * @param  无
  * @retval 无
  */
void Measure(void)
{
	uint8_t str[]={0x80,0x06,0x02,0x78};//激光模块单次测量指令
/*               接收数据之前先清除一下接收标志位吧                 */
	USART1_RX_STA=0;
	USART3_RX_STA=0;
	UART4_RX_STA=0;
/*               发送单次测量指令                */
	u1_printf((char*)str);
	u3_printf((char*)str);
	u4_printf((char*)str);
/*               循环保证所有激光都获得正确数据              */
	while(!(USART1_RX_STA&USART3_RX_STA&UART4_RX_STA&0x8000))
	{
		if(!USART1_RX_STA)
			u1_printf((char*)str);
		if(!USART3_RX_STA)
			u3_printf((char*)str);
		if(!UART4_RX_STA)
			u4_printf((char*)str);
	}
/*              接收完成也清除一下接收标志位吧              */
	USART1_RX_STA=0;
	USART3_RX_STA=0;
	UART4_RX_STA=0;
/*              将字符串转化为int型                 */
	Distant.distant1=Trans(USART1_RX_BUF)+Bias1;
	Distant.distant2=Trans(USART3_RX_BUF)+Bias2;
	Distant.distant3=Trans(UART4_RX_BUF)+Bias3;
}
/**
  * @brief  计算位姿
  * @param  无
  * @retval 无
  */
void Calculate_Position(void)
{
	float length=0;
	float	degree=0;
	Measure();
	length=sqrt((Distant.distant2)*(Distant.distant2)+(Distant.distant3)*(Distant.distant3)-2*(Distant.distant2)*(Distant.distant3)*cos(Degree));
	degree=acos((Distant.distant2*Distant.distant2+length*length-Distant.distant3*Distant.distant3)/(2*length*Distant.distant2));
	Pos.buffer.Angle=degree*180/3.1415926+30;          //将弧度转化为角度
	Pos.buffer.Pos_X=-Distant.distant2*sin(degree)+500;   //500为十字到边沿的距离
	Pos.buffer.Pos_Y=Distant.distant1*cos(1.04719753-degree)+3580;
}
