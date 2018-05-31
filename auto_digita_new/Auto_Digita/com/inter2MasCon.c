#include "inter2MasCon.h"

CMD_BufferType		CMDBuffer[2];									//双接收缓冲区
Msg_Tx_BufferType	Tx_Buffer;
uint8_t 			BufferFlag = 0;									//目前使用的缓冲区标号
int16_t 			DataHeadCorrect = CTRL_UART_Rx_DMA_BufferSize;	//数据头位置矫正值

static uint8_t ack = 0;//接收动作指令的反馈信号

void ReceiveDataFromCtrl_Buffer(const CMD_BufferType* CMDData)
{
	int32_t i, temp = 0;
	uint32_t check = 0;
	static int32_t LastBuffersize = CTRL_UART_Rx_DMA_BufferSize;
	
	if(DataHeadCorrect != CTRL_UART_Rx_DMA_BufferSize)//说明是在进行补偿操作，纠正数据头位置
	{	
		DataHeadCorrect = CTRL_UART_Rx_DMA_BufferSize;
		
		return;
	}
	else
	{
		for(i = 0;i < LastBuffersize - 4;i++)	
		{
			check += CMDData->Bytes[i];					//求和校验
			
			if(0xaa == CMDData->Bytes[i])				//检索数据头位置
			{
				temp = i;
			}
			if((0xbb == CMDData->Bytes[i]) && (temp == (i-1)))	//如果这一位是0xBB,且前一位是0xAA，
			{													//那么数据头在缓冲区的位置就可以确定
				
				DataHeadCorrect = temp;//下一次接收这么多位就可以消除数据错位
			}
			if(DataHeadCorrect == 0)
			{
				DataHeadCorrect = CTRL_UART_Rx_DMA_BufferSize;
			}
		}
		
		if(CMDData->Rxbuffer.verify == check)				//若满足校验条件，则刷入数据
		{
			//灯条，手动车不用
			/*
				lightBarPara = CMDData->Rxbuffer.color
			*/
			
			//fPROCESS.fHANDORAUTO = CMDData->Rxbuffer.hadnOrAuto;

//			if(fPROCESS.fHANDORAUTO == HAND)
//			{
//				cylAct.basicCylGroup = CMDData->Rxbuffer.basicCylGroup;
//				cylAct.basicCylType = CMDData->Rxbuffer.basicCylType;
//				cylAct.basicCylState = CMDData->Rxbuffer.basicCylState;

				assConActSet.ball_serial_num = CMDData->Rxbuffer.Ball_serial_num;
				//assConActSet.roate = CMDData->Rxbuffer.spinPos;

				fPROCESS.cylGroup = CMDData->Rxbuffer.cylGroup;
				fPROCESS.cylAct = CMDData->Rxbuffer.cylAct;
			//}

			fPROCESS.pos = CMDData->Rxbuffer.Current_State;
			fPROCESS.act = CMDData->Rxbuffer.act_flag;

			fLASER = CMDData->Rxbuffer.LaserSignal;

			if(CMDData->Rxbuffer.cylGroup != 0||
				CMDData->Rxbuffer.cylAct != 0||
				CMDData->Rxbuffer.LaserSignal != 0||
				CMDData->Rxbuffer.act_flag != 0)//接收到动作指令，给反馈
				ack = 1;


			if(CMDData->Rxbuffer.ACK_From_Master == 1)//完成动作发送动作完成标志，接收到反馈后清除标志
				fPROCESS.finish = 0;
			
			
			DataHeadCorrect = CTRL_UART_Rx_DMA_BufferSize;
		}
	}

	LastBuffersize = DataHeadCorrect;
}

void SendDataToMasCon(void)
{
	uint8_t i;
	int32_t temp = 0;
	static uint8_t j = 0;
	
	Tx_Buffer.TxBuffer.head1 = 0xaa;
	Tx_Buffer.TxBuffer.head2 = 0xbb;
	
	Tx_Buffer.TxBuffer.cnt = 0x00;
	
	Tx_Buffer.TxBuffer.x = Pos.buffer.Pos_X;
	Tx_Buffer.TxBuffer.y = Pos.buffer.Pos_Y;
	Tx_Buffer.TxBuffer.angle = Pos.buffer.Angle;
	//激光坐标
	if(fLASER == LASER_FINISH)
	{
		Tx_Buffer.TxBuffer.Flag|=(1<<2);   //第2位置一
		//Tx_Buffer.TxBuffer.coordValid = 1;
		fLASER = IDLE;
	}
	else
		Tx_Buffer.TxBuffer.Flag&=~(1<<2);
		//Tx_Buffer.TxBuffer.coordValid = 0;

	//指令应答，收到未动作
	//Tx_Buffer.TxBuffer.ack = ack;
	if(ack)
	{
		Tx_Buffer.TxBuffer.Flag|=1;  //应答为1，第0位置一
		j++;
	}
	else
		Tx_Buffer.TxBuffer.Flag&=~1;  //第0位清零
	
	if(j>3)
	{
		ack = 0;
		j=0;
	}

	//当前动作完成
	if(fPROCESS.finish)
		Tx_Buffer.TxBuffer.Flag|=(1<<1);
	else
		Tx_Buffer.TxBuffer.Flag&=~(1<<1);
	//Tx_Buffer.TxBuffer.finish = fPROCESS.finish;
	Tx_Buffer.TxBuffer.ballNum = fPROCESS.ballNum;
	

	for(i = 0; i<(sizeof(Msg_Tx_BufferType) - 4); i++)	//计算校验()
	{
		temp += Tx_Buffer.Bytes[i];
	}
	
	Tx_Buffer.TxBuffer.verify = temp;// % 0x100;
	
	SendMsg_byDMA();									//发送数据
}
