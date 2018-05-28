#include "STM32F4xx.h"
#include "USART.h"
#include "Map.h"

 /**
  * @brief  USART �ж���������
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
	
	/* ����F103�ӿ�USARTΪ�ж�Դ */
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = CTRL_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = CTRL_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* ���ø��ؽӿ�USARTΪ�ж�Դ */
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = Equips_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = Equips_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* ����С���Խӿ�USARTΪ�ж�Դ */
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = PC_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = PC_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* ������������USARTΪ�ж�Դ */
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = BL_USART_Tx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	//DMA�����ж�����  
    NVIC_InitStructure.NVIC_IRQChannel = BL_USART_Rx_DMA_IRQ;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
	
	/* ����ȫ�ֶ�λUSARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = MAP_USART_IRQ;
	/* ��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* ��Ӧ���ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief ���F103��USART ����DMA ����
  * @param  ��
  * @retval ��
  */
static void Ctrl_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(CTRL_USART_Tx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = CTRL_USART_Tx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = CTRL_USART_Tx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = CTRL_USART_Tx_DMA_BufferSize;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = CTRL_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//�����ڴ��ַָ�������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(CTRL_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(CTRL_USART_Tx_DMA_Stream, CTRL_USART_Tx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(CTRL_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//�Ȳ�ʹ�ܽ���DMAͨ�����ȵ�Ҫ���͵�ʱ����ʹ�ܣ���Ȼ�Ļ���һֱ��
	DMA_Cmd(CTRL_USART_Tx_DMA_Stream, DISABLE); 
	
	//����DMA��ʽ����  
    USART_DMACmd(CTRL_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  ���F103��USART ����DMA ����
  * @param  RxBase:�����ڴ滺������ַ
  * @param  HeadCorrectValue:���ݽ���У׼ֵ
  * @retval ��
  */
static void Ctrl_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(CTRL_USART_Rx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = CTRL_USART_Rx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = CTRL_USART_Rx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = CTRL_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = CTRL_USART_Rx_DMA_BufferSize;
	//������������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(CTRL_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(CTRL_USART_Rx_DMA_Stream, CTRL_USART_Rx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(CTRL_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//ʹ��DMAͨ��
	DMA_Cmd(CTRL_USART_Rx_DMA_Stream, ENABLE); 
	
	//����DMA��ʽ����
    USART_DMACmd(CTRL_USARTx, USART_DMAReq_Rx, ENABLE);  
}

/**
  * @brief  ��F103ͨ�ŵ�USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
static void Ctrl_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	CTRL_USART_GPIO_ClkCmd(CTRL_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	CTRL_USART_ClkCmd(CTRL_USART_CLK, ENABLE);
	
	//����DMAʱ��  
    CTRL_USART_DMAClkCmd(CTRL_USART_DMA_CLK, ENABLE);
	
	//ʹ�ܸ��ã�����GPIO���ӵ�Ŀ������
	GPIO_PinAFConfig(CTRL_USART_TX_GPIO_PORT,CTRL_USART_TX_GPIO_PinSource,CTRL_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(CTRL_USART_RX_GPIO_PORT,CTRL_USART_RX_GPIO_PinSource,CTRL_USART_RX_GPIO_AF);
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = CTRL_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(CTRL_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = CTRL_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(CTRL_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = CTRL_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(CTRL_USARTx, &USART_InitStructure);
	
	//���÷���DMA
	Ctrl_USART_Tx_DMA_Config();
	
	//���ý���DMA
	Ctrl_USART_Rx_DMA_Config((uint32_t)(&(F1_RxBuffer[0])), F1_DataHeadCorrect);
	
	// ʹ�ܴ���
	USART_Cmd(CTRL_USARTx, ENABLE);	    
}

/**
  * @brief ��Ը��ص�USART ����DMA ����
  * @param  ��
  * @retval ��
  */
static void Equips_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(Equips_USART_Tx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = Equips_USART_Tx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = Equips_USART_Tx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = Equips_USART_Tx_DMA_BufferSize;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = Equips_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//�����ڴ��ַָ�������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(Equips_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(Equips_USART_Tx_DMA_Stream, Equips_USART_Tx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(Equips_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//�Ȳ�ʹ�ܽ���DMAͨ�����ȵ�Ҫ���͵�ʱ����ʹ�ܣ���Ȼ�Ļ���һֱ��
	DMA_Cmd(Equips_USART_Tx_DMA_Stream, DISABLE); 
	
	//����DMA��ʽ����  
    USART_DMACmd(Equips_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  ��Ը��ص�USART ����DMA ����
  * @param  RxBase:�����ڴ滺������ַ
  * @param  HeadCorrectValue:���ݽ���У׼ֵ
  * @retval ��
  */
static void Equips_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(Equips_USART_Rx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = Equips_USART_Rx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = Equips_USART_Rx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = Equips_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = Equips_USART_Rx_DMA_BufferSize;
	//������������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(Equips_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(Equips_USART_Rx_DMA_Stream, Equips_USART_Rx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(Equips_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//ʹ��DMAͨ��
	DMA_Cmd(Equips_USART_Rx_DMA_Stream, ENABLE); 
	
	//����DMA��ʽ����
    USART_DMACmd(Equips_USARTx, USART_DMAReq_Rx, ENABLE);  
}

/**
  * @brief  �븨��ͨ�ŵ�USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
static void Equips_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	Equips_USART_GPIO_ClkCmd(Equips_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	Equips_USART_ClkCmd(Equips_USART_CLK, ENABLE);
	
	//����DMAʱ��  
    Equips_USART_DMAClkCmd(Equips_USART_DMA_CLK, ENABLE);
	
	//ʹ�ܸ��ã�����GPIO���ӵ�Ŀ������
	GPIO_PinAFConfig(Equips_USART_TX_GPIO_PORT,Equips_USART_TX_GPIO_PinSource,Equips_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(Equips_USART_RX_GPIO_PORT,Equips_USART_RX_GPIO_PinSource,Equips_USART_RX_GPIO_AF);
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = Equips_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(Equips_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = Equips_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(Equips_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = Equips_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(Equips_USARTx, &USART_InitStructure);
	
	//���÷���DMA
	Equips_USART_Tx_DMA_Config();
	
	//���ý���DMA
	Equips_USART_Rx_DMA_Config((uint32_t)(&(Eq_RxBuffer[0])), Eq_DataHeadCorrect);
	
	// ʹ�ܴ���
	USART_Cmd(Equips_USARTx, ENABLE);	    
}

/**
  * @brief ���С���Ե�USART ����DMA ����
  * @param  ��
  * @retval ��
  */
static void PC_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(PC_USART_Tx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = PC_USART_Tx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = PC_USART_Tx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = PC_USART_Tx_DMA_BufferSize;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = PC_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//�����ڴ��ַָ�������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(PC_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(PC_USART_Tx_DMA_Stream, PC_USART_Tx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(PC_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//�Ȳ�ʹ�ܽ���DMAͨ�����ȵ�Ҫ���͵�ʱ����ʹ�ܣ���Ȼ�Ļ���һֱ��
	DMA_Cmd(PC_USART_Tx_DMA_Stream, DISABLE); 
	
	//����DMA��ʽ����  
    USART_DMACmd(PC_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  ���С���Ե�USART ����DMA ����
  * @param  RxBase:�����ڴ滺������ַ
  * @param  HeadCorrectValue:���ݽ���У׼ֵ
  * @retval ��
  */
static void PC_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(PC_USART_Rx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = PC_USART_Rx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = PC_USART_Rx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = PC_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = PC_USART_Rx_DMA_BufferSize;
	//������������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(PC_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(PC_USART_Rx_DMA_Stream, PC_USART_Rx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(PC_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//ʹ��DMAͨ��
	DMA_Cmd(PC_USART_Rx_DMA_Stream, ENABLE); 
	
	//����DMA��ʽ����
    USART_DMACmd(PC_USARTx, USART_DMAReq_Rx, ENABLE);  
}



/**
  * @brief  ��С����ͨ�ŵ�USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
static void PC_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	PC_USART_GPIO_ClkCmd(PC_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	PC_USART_ClkCmd(PC_USART_CLK, ENABLE);
	
	//����DMAʱ��  
    PC_USART_DMAClkCmd(PC_USART_DMA_CLK, ENABLE);
	
	//ʹ�ܸ��ã�����GPIO���ӵ�Ŀ������
	GPIO_PinAFConfig(PC_USART_TX_GPIO_PORT,PC_USART_TX_GPIO_PinSource,PC_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(PC_USART_RX_GPIO_PORT,PC_USART_RX_GPIO_PinSource,PC_USART_RX_GPIO_AF);
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = PC_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PC_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = PC_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(PC_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = PC_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(PC_USARTx, &USART_InitStructure);
	
	//���÷���DMA
	PC_USART_Tx_DMA_Config();
	
	//���ý���DMA
	PC_USART_Rx_DMA_Config((uint32_t)(&(PC_RxBuffer[0])), PC_DataHeadCorrect);
	
	// ʹ�ܴ���
	USART_Cmd(PC_USARTx, ENABLE);	    
}

/**
  * @brief ����������ƶ˵�USART ����DMA ����
  * @param  ��
  * @retval ��
  */
static void BL_USART_Tx_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(BL_USART_Tx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = BL_USART_Tx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = BL_USART_Tx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = BL_USART_Tx_DMA_BufferSize;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = BL_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//�����ڴ��ַָ�������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(BL_USART_Tx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(BL_USART_Tx_DMA_Stream, BL_USART_Tx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(BL_USART_Tx_DMA_Stream,DMA_IT_TC,ENABLE); 
	
	//�Ȳ�ʹ�ܽ���DMAͨ�����ȵ�Ҫ���͵�ʱ����ʹ�ܣ���Ȼ�Ļ���һֱ��
	DMA_Cmd(BL_USART_Tx_DMA_Stream, DISABLE); 
	
	//����DMA��ʽ����  
    USART_DMACmd(BL_USARTx,USART_DMAReq_Tx,ENABLE);  
}

 /**
  * @brief  ����������ƶ˵�USART ����DMA ���ã���Ϊһ��ʼ�õ����������Ծ���дΪBL��
  * @param  RxBase:�����ڴ滺������ַ
  * @param  HeadCorrectValue:���ݽ���У׼ֵ
  * @retval ��
  */
static void BL_USART_Rx_DMA_Config(uint32_t RxBase, uint32_t HeadCorrectValue)
{
	DMA_InitTypeDef  DMA_InitStructre;
	
	//��λ����DMAͨ��
    DMA_DeInit(BL_USART_Rx_DMA_Stream); 
	
	//ѡ��DMAͨ��
	DMA_InitStructre.DMA_Channel = BL_USART_Rx_DMA_Channel;
	
	//�����ڴ滺������ַ
	DMA_InitStructre.DMA_Memory0BaseAddr = BL_USART_Rx_Memory_Base;
	//�ڴ����ݵ�Ԫ����Ϊһ���ֽڣ�8b��
	DMA_InitStructre.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//ʹ���ڴ��ַ����
	DMA_InitStructre.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//�ڴ�ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	//���������ַ
	DMA_InitStructre.DMA_PeripheralBaseAddr = BL_USART_DR_Base;
	//�����ַ��Ӧ�����ݵ�Ԫ����Ϊһ���ֽ�
	DMA_InitStructre.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//��ֹ�����ַ����
	DMA_InitStructre.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����ͻ������Ϊ���ֽ�
	DMA_InitStructre.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	//���ݵ����ݳ���
	DMA_InitStructre.DMA_BufferSize = BL_USART_Rx_DMA_BufferSize;
	//������������Ϊ����Դ
	DMA_InitStructre.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//��ѭ������ͨ��ģʽ
	DMA_InitStructre.DMA_Mode  = DMA_Mode_Normal;
	
	//��FIFOģʽ
	DMA_InitStructre.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//����FIFO��ֵ����Ϊû��ʹ��FIFO���������ѡ��һ����Ա<1/4�ּ�һ���ֽ�>������䣩
	DMA_InitStructre.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	
	//�������ȼ�Ϊ��
	DMA_InitStructre.DMA_Priority = DMA_Priority_High;
	
	DMA_Init(BL_USART_Rx_DMA_Stream, &DMA_InitStructre);
	
	//���DMA������������ɱ�־λ
    DMA_ClearFlag(BL_USART_Rx_DMA_Stream, BL_USART_Rx_DMA_TC_FLAG);
	
	//ʹ�ܴ�������ж�
	DMA_ITConfig(BL_USART_Rx_DMA_Stream, DMA_IT_TC, ENABLE); 
	
	//ʹ��DMAͨ��
	DMA_Cmd(BL_USART_Rx_DMA_Stream, ENABLE); 
	
	//����DMA��ʽ����
    USART_DMACmd(BL_USARTx, USART_DMAReq_Rx, ENABLE);  
}

/**
  * @brief  ���������ƶ˵�USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
static void BlueTooth_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	BL_USART_GPIO_ClkCmd(BL_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	BL_USART_ClkCmd(BL_USART_CLK, ENABLE);
	
	//����DMAʱ��  
    BL_USART_DMAClkCmd(BL_USART_DMA_CLK, ENABLE);
	
	//ʹ�ܸ��ã�����GPIO���ӵ�Ŀ������
	GPIO_PinAFConfig(BL_USART_TX_GPIO_PORT,BL_USART_TX_GPIO_PinSource,BL_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(BL_USART_RX_GPIO_PORT,BL_USART_RX_GPIO_PinSource,BL_USART_RX_GPIO_AF);
	
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = BL_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(BL_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = BL_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(BL_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = BL_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(BL_USARTx, &USART_InitStructure);
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(BL_USARTx, USART_IT_RXNE, ENABLE);

	//���÷���DMA
	BL_USART_Tx_DMA_Config();
	
	//���ý���DMA
	BL_USART_Rx_DMA_Config((uint32_t)(&(CMDBuffer[0])), DataHeadCorrect);
	 
	// ʹ�ܴ���
	USART_Cmd(BL_USARTx, ENABLE);	    
}

/**
  * @brief  ��ȫ�ֶ�λ�����Ĵ�������
  * @param  ��
  * @retval ��
  */
static void Map_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	MAP_USART_GPIO_APBxClkCmd(MAP_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	MAP_USART_APBxClkCmd(MAP_USART_CLK, ENABLE);
	
	//ʹ�ܸ��ã�����GPIO���ӵ�Ŀ������
	GPIO_PinAFConfig(MAP_USART_TX_GPIO_PORT,MAP_USART_TX_GPIO_PinSource,MAP_USART_TX_GPIO_AF);
	GPIO_PinAFConfig(MAP_USART_RX_GPIO_PORT,MAP_USART_RX_GPIO_PinSource,MAP_USART_RX_GPIO_AF);


	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = MAP_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MAP_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = MAP_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(MAP_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = MAP_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(MAP_USARTx, &USART_InitStructure);
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(MAP_USARTx, USART_IT_RXNE, ENABLE);	

	// ʹ�ܴ���
	USART_Cmd(MAP_USARTx, ENABLE);	
	
}

/**
* @brief  ��ʼ����λϵͳ
* @param  ��
* @retval ��
*/
static void Map_Config(void)
{
	Map_USART_Config();
	MapParamsInit();							//ȫ�ֶ�λ������ʼ��}
}

/**
  * @brief  ��ʼ��ͨ�Ŵ���
  * @param  ��
  * @retval ��
  */
void USART_Config(void)
{
	//��ʼ���ж�����
	NVIC_Configuration();
	
	Map_Config();
	Ctrl_USART_Config();		//��ʼ������F103�Ĵ���
	Equips_USART_Config();		//��ʼ�����Ƹ��صĴ���
	PC_USART_Config();			//��ʼ����С���ԵĽӿ�
	BlueTooth_USART_Config();	//��ʼ������ң�ض˿�
}

/**
  * @brief  ��DMA���з���
  * @param  Tx_DMA_Stream���е������������
  * @param  BufferSize�����ͻ������Ĵ�С
  * @retval ��
  */
void SendMsg_byDMA(DMA_Stream_TypeDef * Tx_DMA_Stream, uint32_t BufferSize)
{
	DMA_Cmd(Tx_DMA_Stream, DISABLE );  				//�ر�Tx_DMA_Channel ��ָʾ��ͨ��        
    DMA_SetCurrDataCounter(Tx_DMA_Stream, BufferSize);//DMAͨ����DMA����Ĵ�С  
    DMA_Cmd(Tx_DMA_Stream, ENABLE);  				//ʹ��Tx_DMA_Channel ��ָʾ��ͨ��   
}

 /**
  * @brief  ���Ľ��ջ�������ַ�Լ�����
  * @param  DMAx_BASE:DMA�������ַ��x����Ϊ1��2
  * @param  ChannelNum:����ͨ����
  * @param  HeadCorrectValue:���ݽ���У׼ֵ
  * @param  RxBase:�����ڴ滺������ַ
  * @retval ��
  */
void DMA_ReceiveSet_Change(uint32_t DMAx_BASE, uint32_t ChannelNum, uint32_t RxBase, uint32_t HeadCorrectValue)
{
	*(uint32_t*)(DMAx_BASE + 0x1C + 0x18 * ChannelNum) =  RxBase;
	
	*(uint32_t*)(DMAx_BASE + 0x14 + 0x18 * ChannelNum) |=  HeadCorrectValue % 0x10000;
}



/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

