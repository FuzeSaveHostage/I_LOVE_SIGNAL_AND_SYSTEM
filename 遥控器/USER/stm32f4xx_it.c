/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.8.0
  * @date    04-November-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "InterfaceToMicroPC.h"
#include "InterfaceToMaster.h"
#include "InterfaceToF103.h"
#include "stm32f4xx_it.h"
#include "ActionCtrl.h"
#include "Schedule.h" 
#include "WatchDog.h"
#include "USART.h"
#include "TIMs.h"
#include "Beep.h"
#include "Base.h"
#include "Map.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

#ifdef __cplusplus

extern "C"
{
	
#endif
	
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

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/



/**
  * @brief  This function handles ȫ�ֶ�λ���� interrupt request.
  * @param  None
  * @retval None
  */
void MAP_USART_IRQHandler(void)
{
	static union
  {
	 uint8_t data[24];
	 float ActVal[6];
  }posture;
	
	static uint8_t ch;
	static uint8_t count=0;
	static uint8_t i=0;

	
	if (USART_GetFlagStatus(MAP_USARTx, USART_FLAG_ORE) != RESET)//ע�⣡����ʹ��if(USART_GetITStatus(MAP_USARTx, USART_IT_RXNE) != RESET)���ж�
  {
		USART_ClearFlag(MAP_USARTx, USART_FLAG_ORE);
        USART_ReceiveData(MAP_USARTx);
		count=0;
				
  }	
	
	if(USART_GetITStatus(MAP_USARTx, USART_IT_RXNE)==SET)   
	{
		USART_ClearITPendingBit( MAP_USARTx,USART_IT_RXNE);
		ch=USART_ReceiveData(MAP_USARTx);
		USART_SendData( MAP_USARTx,ch);

		 switch(count)
		 {
			 case 0:
				 if(ch==0x0d)
				 {
					 count++;
				 }
				 else
					 count=0;
				 break;
				 
			 case 1:
				 if(ch==0x0a)
				 {
					 i=0;
					 count++;
				 }
				 else if(ch==0x0d);
				 else
					 count=0;
				 break;
				 
			 case 2:
				 posture.data[i]=ch;
				i++;
			   if(i>=24)
				 {
					 i=0;
					 count++;
				 }
				 break;
				 
			 case 3:
				 if(ch==0x0a)
					 count++;
				 else
					 count=0;
				 break;
				 
			 case 4:
				 if(ch==0x0d)
				 {
					motion.zangle = posture.ActVal[0];
					motion.xangle = posture.ActVal[1];
					motion.yangle = posture.ActVal[2];
					motion.pos_x  = posture.ActVal[3];
					motion.pos_y  = posture.ActVal[4];
					motion.w_z    = posture.ActVal[5];
					 
					FeedDog(Map);
				 }
			   count=0;
				 break;
			 
			 default:
				 count=0;
			   break;	
		 }
	 }

}

/**
  * @brief  This function handles ���ȶ�ʱ�� interrupt request.
  * @param  None
  * @retval None
  */
void Schedule_TIM_IRQHandler(void)
{
	if(TIM_GetFlagStatus(Schedule_TIMx, TIM_FLAG_Update) != RESET)
	{
		scheduler.cnt_1ms++;
		scheduler.cnt_5ms++;
		scheduler.cnt_10ms++;
		scheduler.cnt_20ms++;
		scheduler.cnt_50ms++;
	
		Loops();
	}
	
	TIM_ClearITPendingBit(Schedule_TIMx,TIM_FLAG_Update);
}

/**
  * @brief  This function handles α���Ź���ʱ�� interrupt request.
  * @param  None
  * @retval None
  */
void WatchDog_TIM_IRQHandler(void)
{
	if(TIM_GetFlagStatus(WatchDog_TIMx, TIM_FLAG_Update) != RESET)
	{	
		if(BeepMutexflag != 0)
		{
			Bark();
		}
	}
	
	TIM_ClearITPendingBit(WatchDog_TIMx,TIM_FLAG_Update);
}

/**
  * @brief  This function handles F103��С���Դ���DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void CTRL_USART_Rx_DMA_IRQHandler(void)
{
	if(DMA_GetFlagStatus(CTRL_USART_Rx_DMA_Stream, CTRL_USART_Rx_DMA_TC_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(CTRL_USART_Rx_DMA_Stream, CTRL_USART_Rx_DMA_TC_FLAG);
		
		DMA_Cmd(CTRL_USART_Rx_DMA_Stream, DISABLE);
		
		DMA_ReceiveSet_Change(CTRL_USART_DMAx_BASE, CTRL_USART_Rx_DMA_StreamNum,
							(uint32_t)(&(F1_RxBuffer[(~F1_BufferFlag)&0x01])),
							F1_DataHeadCorrect);//�л����ջ������ͽ�������
		
		DMA_Cmd(CTRL_USART_Rx_DMA_Stream, ENABLE);
		
		ReceiveDataFromF103_Buffer(F1_RxBuffer[F1_BufferFlag]);	//��ȡ����
		
		F1_BufferFlag = (~F1_BufferFlag)&0x01;
		
		FeedDog(F103);
	}
}

/**
  * @brief  This function handles F103����DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void CTRL_USART_Tx_DMA_IRQHandler(void)
{
	
	if(DMA_GetFlagStatus(CTRL_USART_Tx_DMA_Stream, CTRL_USART_Tx_DMA_TC_FLAG) != RESET)
	{		
		DMA_ClearITPendingBit(CTRL_USART_Tx_DMA_Stream,CTRL_USART_Tx_DMA_TC_FLAG);
		
	}
}

/**
  * @brief  This function handlesС���Դ���DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void PC_USART_Rx_DMA_IRQHandler(void)
{
	if(DMA_GetFlagStatus(PC_USART_Rx_DMA_Stream, PC_USART_Rx_DMA_TC_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(PC_USART_Rx_DMA_Stream, PC_USART_Rx_DMA_TC_FLAG);
		
		DMA_Cmd(PC_USART_Rx_DMA_Stream, DISABLE);
		
		DMA_ReceiveSet_Change(PC_USART_DMAx_BASE, PC_USART_Rx_DMA_StreamNum,
							(uint32_t)(&(PC_RxBuffer[(~PC_BufferFlag)&0x01])),
							PC_DataHeadCorrect);//�л����ջ������ͽ�������
		
		DMA_Cmd(PC_USART_Rx_DMA_Stream, ENABLE);
		
		ReceiveDataFromPC_Buffer(PC_RxBuffer[PC_BufferFlag]);	//��ȡ����
		
		PC_BufferFlag = (~PC_BufferFlag)&0x01;
	}
}

/**
  * @brief  This function handles С���Դ���DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void PC_USART_Tx_DMA_IRQHandler(void)
{
	
	if(DMA_GetFlagStatus(PC_USART_Tx_DMA_Stream, PC_USART_Tx_DMA_TC_FLAG) != RESET)
	{		
		DMA_ClearITPendingBit(PC_USART_Tx_DMA_Stream, PC_USART_Tx_DMA_TC_FLAG);
		
	}
}

/**
  * @brief  This function handles ���ƴ���DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void BL_USART_Rx_DMA_IRQHandler(void)
{
	if(DMA_GetFlagStatus(BL_USART_Rx_DMA_Stream, BL_USART_Rx_DMA_TC_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(BL_USART_Rx_DMA_Stream,BL_USART_Rx_DMA_TC_FLAG);
		
		DMA_Cmd(BL_USART_Rx_DMA_Stream, DISABLE);
		
		DMA_ReceiveSet_Change(BL_USART_DMAx_BASE, BL_USART_Rx_DMA_StreamNum,
							(uint32_t)(&(CMDBuffer[(~BufferFlag)&0x01])),
							DataHeadCorrect);//�л����ջ������ͽ�������
		
		DMA_Cmd(BL_USART_Rx_DMA_Stream, ENABLE);
		
		ReceiveDataFromMaster_Buffer(CMDBuffer[BufferFlag]);	//��ȡ����
		
		BufferFlag = (~BufferFlag)&0x01;
	}
	

}


/**
  * @brief  This function handles ���ƴ���DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void BL_USART_Tx_DMA_IRQHandler(void)
{
	
	if(DMA_GetFlagStatus(BL_USART_Tx_DMA_Stream, BL_USART_Tx_DMA_TC_FLAG) != RESET)
	{		
		DMA_ClearITPendingBit(BL_USART_Tx_DMA_Stream, BL_USART_Tx_DMA_TC_FLAG);
		
	}
}

/**
  * @brief  This function handles ���ش���DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void Equips_USART_Rx_DMA_IRQHandler(void)
{
	if(DMA_GetFlagStatus(Equips_USART_Rx_DMA_Stream, Equips_USART_Rx_DMA_TC_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(Equips_USART_Rx_DMA_Stream,Equips_USART_Rx_DMA_TC_FLAG);
		
		DMA_Cmd(Equips_USART_Rx_DMA_Stream, DISABLE);
		
		DMA_ReceiveSet_Change(Equips_USART_DMAx_BASE, Equips_USART_Rx_DMA_StreamNum,
							(uint32_t)(&(Eq_RxBuffer[(~Eq_BufferFlag)&0x01])),
							Eq_DataHeadCorrect);//�л����ջ������ͽ�������
		
		DMA_Cmd(Equips_USART_Rx_DMA_Stream, ENABLE);
		
		ReceiveDataFromEquips_Buffer(Eq_RxBuffer[Eq_BufferFlag]);	//��ȡ����
		
		Eq_BufferFlag = (~Eq_BufferFlag)&0x01;

	}
}


/**
  * @brief  This function handles ���ش���DMA�����жϷ����� interrupt request.
  * @param  None
  * @retval None
  */

void Equips_USART_Tx_DMA_IRQHandler(void)
{
	
	if(DMA_GetFlagStatus(Equips_USART_Tx_DMA_Stream, Equips_USART_Tx_DMA_TC_FLAG) != RESET)
	{		
		DMA_ClearITPendingBit(Equips_USART_Tx_DMA_Stream, Equips_USART_Tx_DMA_TC_FLAG);
		
		Eq_isSendingflag = false;
		
		CleanActionFlags();
	}
}

#ifdef __cplusplus

}

#endif

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
