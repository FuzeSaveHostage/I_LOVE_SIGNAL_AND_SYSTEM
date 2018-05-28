#include "Cylinder.h"
#include "ActionCtrl.h"

/**
* @brief  ���׳�ʼ��
* @param  ��
* @retval ��
*/
void CylinderInit(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	
	// ��GPIO��ʱ��
	RCC_AHB1PeriphClockCmd(	Left_ClawCylinder_Periph | Middle_ClawCylinder_Periph | Right_ClawCylinder_Periph
							| 	Left_PushCylinder_Periph | Middle_PushCylinder_Periph | Right_PushCylinder_Periph, ENABLE);
	
	//���Ų�������
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;				//����GPIOģʽΪ���
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;			//����Ϊ�Ȳ�����Ҳ������
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;			//����ٶ�
	
	//ѡ������
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD;				//�������ģʽΪ����
	
	GPIOInitStruct.GPIO_Pin = Left_ClawCylinder_PIN;
	GPIO_Init(Left_ClawCylinder_PORT, &GPIOInitStruct);	
	
	GPIOInitStruct.GPIO_Pin = Left_PushCylinder_PIN;
	GPIO_Init(Left_PushCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = Middle_ClawCylinder_PIN;
	GPIO_Init(Middle_ClawCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = Middle_PushCylinder_PIN;
	GPIO_Init(Middle_PushCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;				//�������ģʽΪ��©
	
	GPIOInitStruct.GPIO_Pin = Right_ClawCylinder_PIN;
	GPIO_Init(Right_ClawCylinder_PORT, &GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin = Right_PushCylinder_PIN;
	GPIO_Init(Right_PushCylinder_PORT, &GPIOInitStruct);
	
	//ָ�����׳�ʼ״̬
	Left_ClawCylinderIO = IN;
	Left_PushCylinderIO = IN;
	Middle_ClawCylinderIO = IN;
	Middle_PushCylinderIO = IN;
	Right_ClawCylinderIO = OUT;
	Right_PushCylinderIO = OUT;
}



/************************ (C) COPYRIGHT 2018 YANGYIFAN *****END OF FILE****/

