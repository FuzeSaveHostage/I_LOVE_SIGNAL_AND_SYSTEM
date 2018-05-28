#include "inter2AssCon.h"


/**********************************************************/
/*               说明                                     */
/* ball_serial_num：                                      */
/* 发送时，该变量代表发射的是哪一个球，以便确定参数            */
/* 当发射未完成时，该变量回传为上一个球的编号，当模拟辅控完成操  */
/* 作时，该变量置为数字辅控发送的值                          */
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
