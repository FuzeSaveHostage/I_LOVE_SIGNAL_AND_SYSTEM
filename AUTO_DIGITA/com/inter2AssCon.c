#include "inter2AssCon.h"


/**********************************************************/
/*               ˵��                                     */
/* ball_serial_num��                                      */
/* ����ʱ���ñ��������������һ�����Ա�ȷ������            */
/* ������δ���ʱ���ñ����ش�Ϊ��һ����ı�ţ���ģ�⸨����ɲ�  */
/* ��ʱ���ñ�����Ϊ���ָ��ط��͵�ֵ                          */
/**********************************************************/
void Send2AssCon(__ASS_CON_ACT_t* act)
{
	uint8_t msg[4];

	msg[0] = act->left_roate;
	msg[1] = 0;
	msg[2] = act->right_roate;
	msg[3] = act->ball_serial_num;

	CAN_SendMassage(0x008, 1, msg);
}


void ReceiveFromAssBuffer(CanRxMsg* rx)
{
	assConActGet.left_roate = rx->Data[0];
	assConActGet.right_roate = rx->Data[2];
	assConActGet.pressure_signal = rx->Data[3];
	assConActGet.ball_serial_num = rx->Data[4];
}
