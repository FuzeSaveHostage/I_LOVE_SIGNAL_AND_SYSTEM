#ifndef _IO_ACT_H_
#define _IO_ACT_H_

/*
	本头文件提供一些宏，为IO位带操作提供方便（直接对寄存器某一位赋值）。
	注意：地址偏移以手册为准！
*/

#include "STM32F4xx.h"

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE + 0x14) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE + 0x14) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE + 0x14) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE + 0x14) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE + 0x14) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE + 0x14) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE + 0x14) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE + 0x14) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE + 0x14) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE + 0x10) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE + 0x10) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE + 0x10) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE + 0x10) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE + 0x10) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE + 0x10) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE + 0x10) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE + 0x10) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE + 0x10) //0x40022010  

#define BIT_ADDR(addr, bitnum) *(unsigned int*)((addr & 0xF0000000) + 0x02000000 	\
									+ ((addr & 0x00FFFFFF) << 5) + (bitnum << 2)) 

//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //输出 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //输入

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //输出 
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //输入


#endif 
