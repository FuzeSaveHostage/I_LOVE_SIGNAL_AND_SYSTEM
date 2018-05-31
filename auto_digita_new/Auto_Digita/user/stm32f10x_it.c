/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "bsp.h"

#include "laser.h"
#include "inter2MasCon.h"
#include "schedule.h"
#include "act.h"
    
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg canRxMsg;
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
    {
        FeedDog(ASS_CON);

        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
            
		    if(canRxMsg.StdId == 0x009&&canRxMsg.DLC == 2)
        {
            CAN_Receive(CAN1, CAN_FIFO0, &canRxMsg);
            ReceiveFromAssBuffer(&canRxMsg);
        }

        fSTART |= 0XF0;
    }
}

//调度
void TIM2_IRQHandler()
{
    static uint32_t i = 0;
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET ) 
    {
        i++;
        Schedule(i);
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
    }
}

//另一个线程
void TIM3_IRQHandler()
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) 
    {
        LASER_StartMesure();
        TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
    }    
}

void CTRL_USART_Rx_DMA_IRQHandler(void)
{
    if(DMA_GetITStatus(CTRL_UART_Rx_DMA_TC_FLAG) != RESET)
    {
        DMA_ClearITPendingBit(CTRL_UART_Rx_DMA_GL_FLAG);
        DMA_ClearITPendingBit(CTRL_UART_Rx_DMA_TC_FLAG);
        
        DMA_Cmd(CTRL_UART_Rx_DMA_Channel, DISABLE);
        
        DMA_ReceiveSet_Change((uint32_t)(&(CMDBuffer[(~BufferFlag)&0x01])), DataHeadCorrect);//切换接收缓冲区和接收数量
        
        DMA_Cmd(CTRL_UART_Rx_DMA_Channel, ENABLE);
        
        ReceiveDataFromCtrl_Buffer(&CMDBuffer[BufferFlag]);  //读取数据
        
        BufferFlag = (~BufferFlag)&0x01;

        FeedDog(MAS_CON);

        fSTART |= 0X0F;
    }
    
}

void CTRL_USART_Tx_DMA_IRQHandler(void)
{ 
    if(DMA_GetITStatus(CTRL_UART_Tx_DMA_TC_FLAG) != RESET)
    {
        DMA_ClearITPendingBit(CTRL_UART_Tx_DMA_TC_FLAG);
    }
}

/**
  * @brief  串口1(激光1)中断服务函数
  * @param  无
  * @retval 无
  */
void USART1_IRQHandler(void)
{
    uint8_t Res=0;                       //暂存数据变量
    uint16_t CS=0;                       // 求和变量
    uint8_t i=0;
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET)          //查看串口接收溢出标志位
    {
        USART_ClearFlag(USART1, USART_FLAG_ORE);
        USART_ReceiveData(USART1);
        USART1_RX_STA=0;    
    }
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)             //查看接收中断标志位
    {
        Res=USART_ReceiveData(USART1);
        if((USART1_RX_STA&0X8000)==0)              //接收未完成
        {
            if((USART1_FLAG&0x0F)==0)
            {
                USART1_RX_BUF[USART1_RX_STA&0X0FFF]=Res;
                USART1_RX_STA++;
                if(USART1_RX_BUF[0]!=0x80)
                    USART1_RX_STA=0;                   //接收的第一位错误,重新接收
                if((USART1_RX_BUF[1]!=0x06)&&((USART1_RX_STA&0X0FFF)>1))
                    USART1_RX_STA=0;                   //接收的第二位错误,重新接收
                if((USART1_RX_BUF[2]!=0x85)&&((USART1_RX_STA&0X0FFF)>2))
                    USART1_RX_STA=0;                   //接收的第三位错误,重新接收
                if((USART1_RX_STA&0X0FFF)>3)
                {
                    if(USART1_RX_BUF[3]==0x01)
                    {
                        USART1_RX_STA=1;
                        USART1_FLAG=Laser_Open;         //激光1已经成功打开
                    }
                else
                    USART1_RX_STA=0;                 //接收失败,重新接收 
                }               
            }
            else if((USART1_FLAG&0x0F)==Laser_Open)
            {
                USART1_RX_BUF[USART1_RX_STA&0X0FFF]=Res;
                USART1_RX_STA++;
                if(USART1_RX_BUF[0]!=0xFA)
                    USART1_RX_STA=0;                   //接收的第一位错误,重新接收
                if((USART1_RX_BUF[1]!=0x04)&&((USART1_RX_STA&0X0FFF)>1))
                    USART1_RX_STA=0;                   //接收的第二位错误,重新接收
                if((USART1_RX_BUF[2]!=0x89)&&((USART1_RX_STA&0X0FFF)>2))
                    USART1_RX_STA=0;                   //接收的第三位错误,重新接收
                if((USART1_RX_STA&0X0FFF)>3)
                {
                    if(USART1_RX_BUF[3]==0x79)
                    {
                        USART1_RX_STA=1;
                        USART1_FLAG=Config_Scale;         //激光1已经成功设置量程
                    }
                    else
                        USART1_RX_STA=0;            //量程设置失败,重新接收
                }
            }
            else if((USART1_FLAG&0x0F)==Config_Scale)
            {
                USART1_RX_BUF[USART1_RX_STA&0X0FFF]=Res;
                USART1_RX_STA++;
                if(USART1_RX_BUF[0]!=0x80)
                {
                    USART1_RX_STA=0;                   //接收的第一位错误,重新接收
                }
                if((USART1_RX_BUF[1]!=0x06)&&((USART1_RX_STA&0X0FFF)>1))
                {
                    USART1_RX_STA=0;                   //接收的第二位错误,重新接收
                }
                if((USART1_RX_BUF[2]!=0x82)&&((USART1_RX_STA&0X0FFF)>2))
                {
                    USART1_RX_STA=0;                   //接收的第三位错误,重新接收
                }
                if(((USART1_RX_STA&0X0FFF)>8)&&(USART1_RX_BUF[3]==0x45)&&(USART1_RX_BUF[4]==0x52)&&(USART1_RX_BUF[5]==0x52))
                    USART1_RX_STA=0;                   //测量ERROR重新测量
                if((USART1_RX_STA&0X0FFF)>=11)
                {
                    for(i=0;i<10;i++)                  //求和校验
                        CS+=USART1_RX_BUF[i];
                    CS=~CS+1;
                    if((CS&0x00FF)==USART1_RX_BUF[10])
                    {
                        USART1_RX_STA|=0x8000;           //接收完成  
                    }
                    else
                    {
                        USART1_RX_STA=0;                 //接收错误,重新接收
                    }
                }
            }           
        }
    }   
}

/**
  * @brief  串口3(激光2)中断服务函数
  * @param  无
  * @retval 无
  */
void USART3_IRQHandler(void)
{
    uint8_t Res=0;                       //暂存数据变量
    uint16_t CS=0;                       // 求和变量,初始化赋值为0,若不为0,求和必然出错
    uint8_t i=0;
    if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)!=RESET)          //查看串口接收溢出标志位
    {
        USART_ClearFlag(USART3, USART_FLAG_ORE);
        USART_ReceiveData(USART3);
        USART3_RX_STA=0;    
    }
    if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)             //查看接收中断标志位
    {
        Res=USART_ReceiveData(USART3);
        if((USART3_RX_STA&0X8000)==0)              //接收未完成
        {
            if((USART3_FLAG&0x0F)==0)
            {
                USART3_RX_BUF[USART3_RX_STA&0X0FFF]=Res;
                USART3_RX_STA++;
                if(USART3_RX_BUF[0]!=0x80)
                    USART3_RX_STA=0;                   //接收的第一位错误,重新接收
                if((USART3_RX_BUF[1]!=0x06)&&((USART3_RX_STA&0X0FFF)>1))
                    USART3_RX_STA=0;                   //接收的第二位错误,重新接收
                if((USART3_RX_BUF[2]!=0x85)&&((USART3_RX_STA&0X0FFF)>2))
                    USART3_RX_STA=0;                   //接收的第三位错误,重新接收
                if((USART3_RX_STA&0X0FFF)>3)
                {
                    if(USART3_RX_BUF[3]==0x01)
                    {
                        USART3_RX_STA=1;
                        USART3_FLAG=Laser_Open;         //激光1已经成功打开
                    }
                else
                    USART3_RX_STA=0;                 //接收失败,重新接收 
                }               
            }
            else if((USART3_FLAG&0x0F)==Laser_Open)
            {
                USART3_RX_BUF[USART3_RX_STA&0X0FFF]=Res;
                USART3_RX_STA++;
                if(USART3_RX_BUF[0]!=0xFA)
                    USART3_RX_STA=0;                   //接收的第一位错误,重新接收
                if((USART3_RX_BUF[1]!=0x04)&&((USART3_RX_STA&0X0FFF)>1))
                    USART3_RX_STA=0;                   //接收的第二位错误,重新接收
                if((USART3_RX_BUF[2]!=0x89)&&((USART3_RX_STA&0X0FFF)>2))
                    USART3_RX_STA=0;                   //接收的第三位错误,重新接收
                if((USART3_RX_STA&0X0FFF)>3)
                {
                    if(USART3_RX_BUF[3]==0x79)
                    {
                        USART3_RX_STA=1;
                        USART3_FLAG=Config_Scale;         //激光1已经成功设置量程
                    }
                    else
                        USART3_RX_STA=0;            //量程设置失败,重新接收
                }
            }
            else if((USART3_FLAG&0x0F)==Config_Scale)
            {
                USART3_RX_BUF[USART3_RX_STA&0X0FFF]=Res;
                USART3_RX_STA++;
                if(USART3_RX_BUF[0]!=0x80)
                {
                    USART3_RX_STA=0;                   //接收的第一位错误,重新接收
                }
                if((USART3_RX_BUF[1]!=0x06)&&((USART3_RX_STA&0X0FFF)>1))
                {
                    USART3_RX_STA=0;                   //接收的第二位错误,重新接收
                }
                if((USART3_RX_BUF[2]!=0x82)&&((USART3_RX_STA&0X0FFF)>2))
                {
                    USART3_RX_STA=0;                   //接收的第三位错误,重新接收
                }
                if(((USART3_RX_STA&0X0FFF)>8)&&(USART3_RX_BUF[3]==0x45)&&(USART3_RX_BUF[4]==0x52)&&(USART3_RX_BUF[5]==0x52))
                    USART3_RX_STA=0;                   //测量ERROR重新测量
                if((USART3_RX_STA&0X0FFF)>=11)
                {
                    for(i=0;i<10;i++)                  //求和校验
                        CS+=USART3_RX_BUF[i];
                    CS=~CS+1;
                    if((CS&0x00FF)==USART3_RX_BUF[10])
                    {
                        USART3_RX_STA|=0x8000;           //接收完成  
                    }
                    else
                    {
                        USART3_RX_STA=0;                 //接收错误,重新接收
                    }
                }
            }           
        }
    }   
}

/**
  * @brief  串口4(激光3)中断服务函数
  * @param  无
  * @retval 无
  */
void UART4_IRQHandler(void)
{
    uint8_t Res=0;                       //暂存数据变量
    uint16_t CS=0;                       // 求和变量,初始化赋值为0,若不为0,求和必然出错
    uint8_t i=0;
    if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)!=RESET)          //查看串口接收溢出标志位
    {
        USART_ClearFlag(UART4, USART_FLAG_ORE);
        USART_ReceiveData(UART4);
        UART4_RX_STA=0; 
    }
    if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)             //查看接收中断标志位
    {
        Res=USART_ReceiveData(UART4);
        if((UART4_RX_STA&0X8000)==0)              //接收未完成
        {
            if((UART4_FLAG&0x0F)==0)
            {
                UART4_RX_BUF[UART4_RX_STA&0X0FFF]=Res;
                UART4_RX_STA++;
                if(UART4_RX_BUF[0]!=0x80)
                    UART4_RX_STA=0;                   //接收的第一位错误,重新接收
                if((UART4_RX_BUF[1]!=0x06)&&((UART4_RX_STA&0X0FFF)>1))
                    UART4_RX_STA=0;                   //接收的第二位错误,重新接收
                if((UART4_RX_BUF[2]!=0x85)&&((UART4_RX_STA&0X0FFF)>2))
                    UART4_RX_STA=0;                   //接收的第三位错误,重新接收
                if((UART4_RX_STA&0X0FFF)>3)
                {
                    if(UART4_RX_BUF[3]==0x01)
                    {
                        UART4_RX_STA=1;
                        UART4_FLAG=Laser_Open;         //激光1已经成功打开
                    }
                else
                    UART4_RX_STA=0;                 //接收失败,重新接收 
                }               
            }
            else if((UART4_FLAG&0x0F)==Laser_Open)
            {
                UART4_RX_BUF[UART4_RX_STA&0X0FFF]=Res;
                UART4_RX_STA++;
                if(UART4_RX_BUF[0]!=0xFA)
                    UART4_RX_STA=0;                   //接收的第一位错误,重新接收
                if((UART4_RX_BUF[1]!=0x04)&&((UART4_RX_STA&0X0FFF)>1))
                    UART4_RX_STA=0;                   //接收的第二位错误,重新接收
                if((UART4_RX_BUF[2]!=0x89)&&((UART4_RX_STA&0X0FFF)>2))
                    UART4_RX_STA=0;                   //接收的第三位错误,重新接收
                if((UART4_RX_STA&0X0FFF)>3)
                {
                    if(UART4_RX_BUF[3]==0x79)
                    {
                        UART4_RX_STA=1;
                        UART4_FLAG=Config_Scale;         //激光1已经成功设置量程
                    }
                    else
                        UART4_RX_STA=0;            //量程设置失败,重新接收
                }
            }
            else if((UART4_FLAG&0x0F)==Config_Scale)
            {
                UART4_RX_BUF[UART4_RX_STA&0X0FFF]=Res;
                UART4_RX_STA++;
                if(UART4_RX_BUF[0]!=0x80)
                {
                    UART4_RX_STA=0;                   //接收的第一位错误,重新接收
                }
                if((UART4_RX_BUF[1]!=0x06)&&((UART4_RX_STA&0X0FFF)>1))
                {
                    UART4_RX_STA=0;                   //接收的第二位错误,重新接收
                }
                if((UART4_RX_BUF[2]!=0x82)&&((UART4_RX_STA&0X0FFF)>2))
                {
                    UART4_RX_STA=0;                   //接收的第三位错误,重新接收
                }
                if(((UART4_RX_STA&0X0FFF)>8)&&(UART4_RX_BUF[3]==0x45)&&(UART4_RX_BUF[4]==0x52)&&(UART4_RX_BUF[5]==0x52))
                    UART4_RX_STA=0;                   //测量ERROR重新测量
                if((UART4_RX_STA&0X0FFF)>=11)
                {
                    for(i=0;i<10;i++)                  //求和校验
                        CS+=UART4_RX_BUF[i];
                    CS=~CS+1;
                    if((CS&0x00FF)==UART4_RX_BUF[10])
                    {
                        UART4_RX_STA|=0x8000;           //接收完成  
                    }
                    else
                    {
                        UART4_RX_STA=0;                 //接收错误,重新接收
                    }
                }
            }           
        }
    }   
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
