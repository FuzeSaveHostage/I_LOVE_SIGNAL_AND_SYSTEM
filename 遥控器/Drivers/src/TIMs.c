#include "STM32F4xx.h"
#include "TIMs.h"

 /**
  * @brief  ������ �ж���������
  * @param  ��
  * @retval ��
  */
static void ScheduleTIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
	/* ����TIM7Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = Schedule_TIM_IRQ;
	
	/* ��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	
	/* ��Ӧ���ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  ���ȶ�ʱ������,������������
  * @param  ��
  * @retval ��
  */
void ScheduleTIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	
	// ������ʱ��
	Schedule_TIM_ClkCmd(Schedule_TIM_CLK, ENABLE);
	
	//���ü���ģʽ
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	
	//���ü�������
	TIM_InitStructure.TIM_Period = 1000 - 1;				//��������Ϊ1ms
	
	//����Ԥ��Ƶϵ��
	TIM_InitStructure.TIM_Prescaler = 90 - 1;				//������APB1�����ϣ���Ƶ��Ƶ��90MHz
	
	//��ʼ����ʱ��
	TIM_TimeBaseInit(Schedule_TIMx, &TIM_InitStructure);
	
	// ʹ�ܶ�ʱ���ж�
	TIM_ITConfig(Schedule_TIMx, TIM_IT_Update, ENABLE);	
	 
	//��ʼ����ʱ���ж�����
	ScheduleTIM_NVIC_Config();
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(Schedule_TIMx, ENABLE);	    
}

/**
  * @brief  ģ�⿴�Ź���ʱ���ж���������
  * @param  ��
  * @retval ��
  */
static void WatchDogTIM_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	
	/* ����TIM6Ϊ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = WatchDog_TIM_IRQ;
	
	/* ��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
	
	/* ��Ӧ���ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  ģ�⿴�Ź���ʱ������,�����������ã�400ms��
  * @param  ��
  * @retval ��
  */
void WatchDogTIM_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	
	// ������ʱ��
	WatchDog_TIM_ClkCmd(WatchDog_TIM_CLK, ENABLE);
	
	//���ü���ģʽ
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	
	//���ü�������
	TIM_InitStructure.TIM_Period = 50000 - 1;				//��������Ϊ50 * 8ms
	
	//����Ԥ��Ƶϵ��
	TIM_InitStructure.TIM_Prescaler = 720 - 1;				//������APB1�����ϣ���Ƶ��Ƶ��90MHz
	
	//��ʼ����ʱ��
	TIM_TimeBaseInit(WatchDog_TIMx, &TIM_InitStructure);
	
	// ʹ�ܶ�ʱ���ж�
	TIM_ITConfig(WatchDog_TIMx, TIM_IT_Update, ENABLE);	
	
	//��ʼ����ʱ���ж�����
	WatchDogTIM_NVIC_Config();
	 
	// ʹ�ܶ�ʱ��
	TIM_Cmd(WatchDog_TIMx, ENABLE);	    
}

/**
  * @brief  ������������ʱ������,������������
  * @param  ��
  * @retval ��
  */
void Beep_TIM_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_OCInitTypeDef OC_InitStructure;
	
	// ������ʱ��
	Beep_TIM_ClkCmd(Beep_TIM_CLK, ENABLE);
	
	// ��PWM���GPIO��ʱ��
	Beep_GPIO_ClkCmd(Beep_GPIO_CLK, ENABLE);
	
	//ʹ�ܸ��ã�����GPIO���ӵ�Ŀ������
	GPIO_PinAFConfig(Beep_GPIO_PORT, Beep_GPIO_PinSource, Beep_GPIO_AF);
	
	/*  ����PWM���GPIO  */
	GPIO_InitStructure.GPIO_Pin = Beep_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(Beep_GPIO_PORT, &GPIO_InitStructure);
	
	/*  ����ʱ����Ԫ  */
	//���ü���ģʽ
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	
	//���ü�������
	TIM_InitStructure.TIM_Period = 1000 - 1;				//��������Ϊ1ms
	
	//����Ԥ��Ƶϵ��
	TIM_InitStructure.TIM_Prescaler = 90 - 1;				//������APB1�����ϣ���Ƶ��90MHz
	
	//�����ظ���������
	TIM_InitStructure.TIM_RepetitionCounter = 0;			//���ظ�����
	
	//���������������������˲����Զ�ʱ��ʱ��Ƶ�ʵķ�Ƶ
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//����Ƶ
	
	//��ʼ����ʱ��
	TIM_TimeBaseInit(Beep_TIMx, &TIM_InitStructure);
	
	/*  �������ͨ��  */
	//ѡ���������ģʽ
	OC_InitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//ѡ��ΪPWM1ģʽ����ģʽ�µ�CNT��ֵС��Ԥ��ֵʱ��Ϊ�����Ч״̬
	
	//����״̬�µ����ŵ�ƽ
	OC_InitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//����Ϊ�͵�ƽ
	
	//���������Ч��ƽ�ļ���
	OC_InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//�ߵ�ƽ��Ч
	
	//ʹ�ܸ�ͨ��
	OC_InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	//��ʼ����
	OC_InitStructure.TIM_Pulse = 500;
	
	//�رջ������ʹ��
	OC_InitStructure.TIM_OutputNState = TIM_OutputState_Disable;
	
	//�ߵ�ƽ��Ч
	OC_InitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	//�������ſ���ʱ�ĵ�ƽ״̬
	OC_InitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	
	BeepOCInit(Beep_TIMx, &OC_InitStructure);
	
	/* ʹ��ͨ������ */
	Beep_OCPreloadConfig(Beep_TIMx, TIM_OCPreload_Enable);
	
	TIM_CCxCmd(Beep_TIMx, Beep_Channel, TIM_CCx_Disable);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(Beep_TIMx, ENABLE);	
	
}

/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/
