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

//����
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

//��һ���߳�
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
        
        DMA_ReceiveSet_Change((uint32_t)(&(CMDBuffer[(~BufferFlag)&0x01])), DataHeadCorrect);//�л����ջ������ͽ�������
        
        DMA_Cmd(CTRL_UART_Rx_DMA_Channel, ENABLE);
        
        ReceiveDataFromCtrl_Buffer(&CMDBuffer[BufferFlag]);  //��ȡ����
        
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
  * @brief  ����1(����1)�жϷ�����
  * @param  ��
  * @retval ��
  */
void USART1_IRQHandler(void)
{
    uint8_t Res=0;                       //�ݴ����ݱ���
    uint16_t CS=0;                       // ��ͱ���
    uint8_t i=0;
    if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET)          //�鿴���ڽ��������־λ
    {
        USART_ClearFlag(USART1, USART_FLAG_ORE);
        USART_ReceiveData(USART1);
        USART1_RX_STA=0;    
    }
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)             //�鿴�����жϱ�־λ
    {
        Res=USART_ReceiveData(USART1);
        if((USART1_RX_STA&0X8000)==0)              //����δ���
        {
            if((USART1_FLAG&0x0F)==0)
            {
                USART1_RX_BUF[USART1_RX_STA&0X0FFF]=Res;
                USART1_RX_STA++;
                if(USART1_RX_BUF[0]!=0x80)
                    USART1_RX_STA=0;                   //���յĵ�һλ����,���½���
                if((USART1_RX_BUF[1]!=0x06)&&((USART1_RX_STA&0X0FFF)>1))
                    USART1_RX_STA=0;                   //���յĵڶ�λ����,���½���
                if((USART1_RX_BUF[2]!=0x85)&&((USART1_RX_STA&0X0FFF)>2))
                    USART1_RX_STA=0;                   //���յĵ���λ����,���½���
                if((USART1_RX_STA&0X0FFF)>3)
                {
                    if(USART1_RX_BUF[3]==0x01)
                    {
                        USART1_RX_STA=1;
                        USART1_FLAG=Laser_Open;         //����1�Ѿ��ɹ���
                    }
                else
                    USART1_RX_STA=0;                 //����ʧ��,���½��� 
                }               
            }
            else if((USART1_FLAG&0x0F)==Laser_Open)
            {
                USART1_RX_BUF[USART1_RX_STA&0X0FFF]=Res;
                USART1_RX_STA++;
                if(USART1_RX_BUF[0]!=0xFA)
                    USART1_RX_STA=0;                   //���յĵ�һλ����,���½���
                if((USART1_RX_BUF[1]!=0x04)&&((USART1_RX_STA&0X0FFF)>1))
                    USART1_RX_STA=0;                   //���յĵڶ�λ����,���½���
                if((USART1_RX_BUF[2]!=0x89)&&((USART1_RX_STA&0X0FFF)>2))
                    USART1_RX_STA=0;                   //���յĵ���λ����,���½���
                if((USART1_RX_STA&0X0FFF)>3)
                {
                    if(USART1_RX_BUF[3]==0x79)
                    {
                        USART1_RX_STA=1;
                        USART1_FLAG=Config_Scale;         //����1�Ѿ��ɹ���������
                    }
                    else
                        USART1_RX_STA=0;            //��������ʧ��,���½���
                }
            }
            else if((USART1_FLAG&0x0F)==Config_Scale)
            {
                USART1_RX_BUF[USART1_RX_STA&0X0FFF]=Res;
                USART1_RX_STA++;
                if(USART1_RX_BUF[0]!=0x80)
                {
                    USART1_RX_STA=0;                   //���յĵ�һλ����,���½���
                }
                if((USART1_RX_BUF[1]!=0x06)&&((USART1_RX_STA&0X0FFF)>1))
                {
                    USART1_RX_STA=0;                   //���յĵڶ�λ����,���½���
                }
                if((USART1_RX_BUF[2]!=0x82)&&((USART1_RX_STA&0X0FFF)>2))
                {
                    USART1_RX_STA=0;                   //���յĵ���λ����,���½���
                }
                if(((USART1_RX_STA&0X0FFF)>8)&&(USART1_RX_BUF[3]==0x45)&&(USART1_RX_BUF[4]==0x52)&&(USART1_RX_BUF[5]==0x52))
                    USART1_RX_STA=0;                   //����ERROR���²���
                if((USART1_RX_STA&0X0FFF)>=11)
                {
                    for(i=0;i<10;i++)                  //���У��
                        CS+=USART1_RX_BUF[i];
                    CS=~CS+1;
                    if((CS&0x00FF)==USART1_RX_BUF[10])
                    {
                        USART1_RX_STA|=0x8000;           //�������  
                    }
                    else
                    {
                        USART1_RX_STA=0;                 //���մ���,���½���
                    }
                }
            }           
        }
    }   
}

/**
  * @brief  ����3(����2)�жϷ�����
  * @param  ��
  * @retval ��
  */
void USART3_IRQHandler(void)
{
    uint8_t Res=0;                       //�ݴ����ݱ���
    uint16_t CS=0;                       // ��ͱ���,��ʼ����ֵΪ0,����Ϊ0,��ͱ�Ȼ����
    uint8_t i=0;
    if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)!=RESET)          //�鿴���ڽ��������־λ
    {
        USART_ClearFlag(USART3, USART_FLAG_ORE);
        USART_ReceiveData(USART3);
        USART3_RX_STA=0;    
    }
    if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)             //�鿴�����жϱ�־λ
    {
        Res=USART_ReceiveData(USART3);
        if((USART3_RX_STA&0X8000)==0)              //����δ���
        {
            if((USART3_FLAG&0x0F)==0)
            {
                USART3_RX_BUF[USART3_RX_STA&0X0FFF]=Res;
                USART3_RX_STA++;
                if(USART3_RX_BUF[0]!=0x80)
                    USART3_RX_STA=0;                   //���յĵ�һλ����,���½���
                if((USART3_RX_BUF[1]!=0x06)&&((USART3_RX_STA&0X0FFF)>1))
                    USART3_RX_STA=0;                   //���յĵڶ�λ����,���½���
                if((USART3_RX_BUF[2]!=0x85)&&((USART3_RX_STA&0X0FFF)>2))
                    USART3_RX_STA=0;                   //���յĵ���λ����,���½���
                if((USART3_RX_STA&0X0FFF)>3)
                {
                    if(USART3_RX_BUF[3]==0x01)
                    {
                        USART3_RX_STA=1;
                        USART3_FLAG=Laser_Open;         //����1�Ѿ��ɹ���
                    }
                else
                    USART3_RX_STA=0;                 //����ʧ��,���½��� 
                }               
            }
            else if((USART3_FLAG&0x0F)==Laser_Open)
            {
                USART3_RX_BUF[USART3_RX_STA&0X0FFF]=Res;
                USART3_RX_STA++;
                if(USART3_RX_BUF[0]!=0xFA)
                    USART3_RX_STA=0;                   //���յĵ�һλ����,���½���
                if((USART3_RX_BUF[1]!=0x04)&&((USART3_RX_STA&0X0FFF)>1))
                    USART3_RX_STA=0;                   //���յĵڶ�λ����,���½���
                if((USART3_RX_BUF[2]!=0x89)&&((USART3_RX_STA&0X0FFF)>2))
                    USART3_RX_STA=0;                   //���յĵ���λ����,���½���
                if((USART3_RX_STA&0X0FFF)>3)
                {
                    if(USART3_RX_BUF[3]==0x79)
                    {
                        USART3_RX_STA=1;
                        USART3_FLAG=Config_Scale;         //����1�Ѿ��ɹ���������
                    }
                    else
                        USART3_RX_STA=0;            //��������ʧ��,���½���
                }
            }
            else if((USART3_FLAG&0x0F)==Config_Scale)
            {
                USART3_RX_BUF[USART3_RX_STA&0X0FFF]=Res;
                USART3_RX_STA++;
                if(USART3_RX_BUF[0]!=0x80)
                {
                    USART3_RX_STA=0;                   //���յĵ�һλ����,���½���
                }
                if((USART3_RX_BUF[1]!=0x06)&&((USART3_RX_STA&0X0FFF)>1))
                {
                    USART3_RX_STA=0;                   //���յĵڶ�λ����,���½���
                }
                if((USART3_RX_BUF[2]!=0x82)&&((USART3_RX_STA&0X0FFF)>2))
                {
                    USART3_RX_STA=0;                   //���յĵ���λ����,���½���
                }
                if(((USART3_RX_STA&0X0FFF)>8)&&(USART3_RX_BUF[3]==0x45)&&(USART3_RX_BUF[4]==0x52)&&(USART3_RX_BUF[5]==0x52))
                    USART3_RX_STA=0;                   //����ERROR���²���
                if((USART3_RX_STA&0X0FFF)>=11)
                {
                    for(i=0;i<10;i++)                  //���У��
                        CS+=USART3_RX_BUF[i];
                    CS=~CS+1;
                    if((CS&0x00FF)==USART3_RX_BUF[10])
                    {
                        USART3_RX_STA|=0x8000;           //�������  
                    }
                    else
                    {
                        USART3_RX_STA=0;                 //���մ���,���½���
                    }
                }
            }           
        }
    }   
}

/**
  * @brief  ����4(����3)�жϷ�����
  * @param  ��
  * @retval ��
  */
void UART4_IRQHandler(void)
{
    uint8_t Res=0;                       //�ݴ����ݱ���
    uint16_t CS=0;                       // ��ͱ���,��ʼ����ֵΪ0,����Ϊ0,��ͱ�Ȼ����
    uint8_t i=0;
    if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)!=RESET)          //�鿴���ڽ��������־λ
    {
        USART_ClearFlag(UART4, USART_FLAG_ORE);
        USART_ReceiveData(UART4);
        UART4_RX_STA=0; 
    }
    if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)             //�鿴�����жϱ�־λ
    {
        Res=USART_ReceiveData(UART4);
        if((UART4_RX_STA&0X8000)==0)              //����δ���
        {
            if((UART4_FLAG&0x0F)==0)
            {
                UART4_RX_BUF[UART4_RX_STA&0X0FFF]=Res;
                UART4_RX_STA++;
                if(UART4_RX_BUF[0]!=0x80)
                    UART4_RX_STA=0;                   //���յĵ�һλ����,���½���
                if((UART4_RX_BUF[1]!=0x06)&&((UART4_RX_STA&0X0FFF)>1))
                    UART4_RX_STA=0;                   //���յĵڶ�λ����,���½���
                if((UART4_RX_BUF[2]!=0x85)&&((UART4_RX_STA&0X0FFF)>2))
                    UART4_RX_STA=0;                   //���յĵ���λ����,���½���
                if((UART4_RX_STA&0X0FFF)>3)
                {
                    if(UART4_RX_BUF[3]==0x01)
                    {
                        UART4_RX_STA=1;
                        UART4_FLAG=Laser_Open;         //����1�Ѿ��ɹ���
                    }
                else
                    UART4_RX_STA=0;                 //����ʧ��,���½��� 
                }               
            }
            else if((UART4_FLAG&0x0F)==Laser_Open)
            {
                UART4_RX_BUF[UART4_RX_STA&0X0FFF]=Res;
                UART4_RX_STA++;
                if(UART4_RX_BUF[0]!=0xFA)
                    UART4_RX_STA=0;                   //���յĵ�һλ����,���½���
                if((UART4_RX_BUF[1]!=0x04)&&((UART4_RX_STA&0X0FFF)>1))
                    UART4_RX_STA=0;                   //���յĵڶ�λ����,���½���
                if((UART4_RX_BUF[2]!=0x89)&&((UART4_RX_STA&0X0FFF)>2))
                    UART4_RX_STA=0;                   //���յĵ���λ����,���½���
                if((UART4_RX_STA&0X0FFF)>3)
                {
                    if(UART4_RX_BUF[3]==0x79)
                    {
                        UART4_RX_STA=1;
                        UART4_FLAG=Config_Scale;         //����1�Ѿ��ɹ���������
                    }
                    else
                        UART4_RX_STA=0;            //��������ʧ��,���½���
                }
            }
            else if((UART4_FLAG&0x0F)==Config_Scale)
            {
                UART4_RX_BUF[UART4_RX_STA&0X0FFF]=Res;
                UART4_RX_STA++;
                if(UART4_RX_BUF[0]!=0x80)
                {
                    UART4_RX_STA=0;                   //���յĵ�һλ����,���½���
                }
                if((UART4_RX_BUF[1]!=0x06)&&((UART4_RX_STA&0X0FFF)>1))
                {
                    UART4_RX_STA=0;                   //���յĵڶ�λ����,���½���
                }
                if((UART4_RX_BUF[2]!=0x82)&&((UART4_RX_STA&0X0FFF)>2))
                {
                    UART4_RX_STA=0;                   //���յĵ���λ����,���½���
                }
                if(((UART4_RX_STA&0X0FFF)>8)&&(UART4_RX_BUF[3]==0x45)&&(UART4_RX_BUF[4]==0x52)&&(UART4_RX_BUF[5]==0x52))
                    UART4_RX_STA=0;                   //����ERROR���²���
                if((UART4_RX_STA&0X0FFF)>=11)
                {
                    for(i=0;i<10;i++)                  //���У��
                        CS+=UART4_RX_BUF[i];
                    CS=~CS+1;
                    if((CS&0x00FF)==UART4_RX_BUF[10])
                    {
                        UART4_RX_STA|=0x8000;           //�������  
                    }
                    else
                    {
                        UART4_RX_STA=0;                 //���մ���,���½���
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
