#include "InterfaceToMicroPC.h"
#include "InterfaceToMaster.h"
#include "Motion_Ctrl.h"
#include "ActionCtrl.h"
#include "STM32F4xx.h"
#include "Vol_curve.h"
#include "WatchDog.h"
#include "Process.h"
#include "USART.h"
#include "Base.h"
#include "math.h"
#include "Map.h"
#include "Control.h"

uint8_t spin_flag = 0;
uint8_t NEXT_FLAG = 0;
void Controler_handle()
{
		static uint8_t key = 0,key2 = 0;   //��¼�ϴ�����У��ʱ����������ֵ
		static uint8_t speed_flag = 0;  //0Ĭ��Ϊ����
		//static uint8_t manual_auto = 0;  //�ֶ����Զ��л���־��0Ϊ�ֶ� 
		//static uint8_t start_flag = 0;   //������һ���ı�־λ
		static uint8_t R_PUSH_flag = 0;  //����ı�־λ
		static uint8_t L_PUSH_flag = 0;  //����ı�־λ
		static uint8_t BOTH_HANDOVER_flag = 0;
		static uint8_t X_flag = 0;
		static int8_t plus_minus;
		static uint8_t speed_ratio;
	
			/************************************����Ϊ�ֶ�����***************************************************/

				//SpinState = Spin_Null;
		   //��������
			//-------------------------ȡ�����ָ������ж�Ϊ�жϰ�������----------------------------------------//
			 if((KEY1_value & A_BUTTON) && (!(key & A_BUTTON))   //����KEY��Ϊ���ж��ϴ�û�а�����ΰ����ˣ���ֹ�����ظ�ִ��
					&& (KEY2_value & RT) && (KEY2_value & LT))   //��ΰ��¶��ϴ�û�а���
			 {
				 if(!BOTH_HANDOVER_flag)
				 {
				   Act_flag = (enum SpecialActions)BOTH_PUSH;
				   ActCylinders = BOTH;
					 spin_flag = Spin_Null; 
				   ActACK = false;
					 BOTH_HANDOVER_flag = 1;
				 }
         else
				 {
           Act_flag = (enum SpecialActions)BOTH_FETCH;
				   ActCylinders = BOTH;
					 spin_flag = Spin_Null; 
				   ActACK = false;
					 BOTH_HANDOVER_flag = 0;
				 }					 
			 }
			 else if((KEY1_value & A_BUTTON) && (!(key & A_BUTTON)) 
								&& (KEY2_value & LT ))   //��ΰ��¶��ϴ�û�а���
			 {
				 if(!L_PUSH_flag)
				 {
				   Act_flag = (enum SpecialActions)L_PUSH;
				   ActCylinders = LEFT;
					 spin_flag = Spin_Null; 
				   ActACK = false;
					 L_PUSH_flag = 1;
				 }
         else
				 {
           Act_flag = (enum SpecialActions)L_FETCH;
				   ActCylinders = LEFT;
					 spin_flag = Spin_Null; 
				   ActACK = false;
					 L_PUSH_flag = 0;
				 }					 
			 }			 
			 else if((KEY1_value & A_BUTTON) && (!(key & A_BUTTON)) 
								&& (KEY2_value & RT))   //��ΰ��¶��ϴ�û�а���
			 {
				 if(!R_PUSH_flag)
				 {
				   Act_flag = (enum SpecialActions)R_PUSH;
				   ActCylinders = RIGHT;	
					 spin_flag = Spin_Null; 
				   ActACK = false;
					 R_PUSH_flag = 1;
				 }
         else
				 {
           Act_flag = (enum SpecialActions)R_FETCH;
				   ActCylinders = RIGHT;
					 spin_flag = Spin_Null; 
				   ActACK = false;
					 R_PUSH_flag = 0;
				 }					 
			 }
			 
			 //------------------------------------------------------------------------//
			 //---------------------NEXT����δ�����ԣ�����-------------------------------//
			 if((KEY1_value & B_BUTTON) && (!(key & B_BUTTON)))   
			 {
					NEXT_FLAG=1;           //������һ���ֶ�����
			 }
			 //-------------------------------------------------------------------------//
			 
			 
			 if((KEY1_value & X_BUTTON) && (!(key & X_BUTTON)))  
			 {
				 if(!X_flag)
				 {
				   Act_flag = (enum SpecialActions)NORM_STEP1;
				   ActCylinders = SPECIAL;
				   spin_flag = Spin_Null;
				   ActACK = false;		
					 
					 X_flag = 1;
         }	
         else
				 {
           Act_flag = (enum SpecialActions)NORM_STEP2;
				   ActCylinders = SPECIAL;
				   spin_flag = Spin_Null;
				   ActACK = false;	
					 
					 X_flag = 0;
         }					 
			 }
			 
			 if((KEY1_value & Y_BUTTON) && (!(key & Y_BUTTON)))   
			 {
				 Act_flag = (enum SpecialActions)GOLDEN_HANDOVER;
				 ActCylinders = SPECIAL;
				 spin_flag = Spin_Null;
				 ActACK = false;		 
			 }
			 
			 //---------------------------��תָ��-----------------------------------//
			 if(((KEY1_value & RB) && (!(key & RB)))
						||((KEY1_value & LB) && (!(key & LB))))  //----����RB��LB��һ��������
			 {
				 if((KEY1_value & RB) && (!(key & RB)))
			 {
				 
				 if(spin_flag <= 2)
				 {
				   spin_flag++;		
				 }
			 }
			 else if((KEY1_value & LB) && (!(key & LB)))
			 {
				 
				 if(spin_flag >= 2)
				 {
				   spin_flag--;
				 }
			 }
			 
			 if(spin_flag>=3) spin_flag=3;
			 if(spin_flag<=1) spin_flag=1;
			 
			 SpinState=(SpinPositionType)spin_flag;
			
			  ActACK = false;
		 }
			 
		 
				 
			 if((KEY2_value & L3) && (!(key2 & L3)))
			 {
				 speed_flag = !speed_flag;
			 }
			 
			 /*--------------------ҡ�˿��Ƶ����ƶ�--------------------------*/
			 /*�ٶȽ���*/  //--------�ÿ���������y�����������x�������  --------------------//��Ҫ����
			 
			 if((Rotate_value&0x10))   
						plus_minus=-1;  //�������λΪ1������Ϊ��
			 else 
						plus_minus=1;
			 
			 if(((Rotate_value&0x08)==0)&&((Rotate_value&0x04))) 
				 speed_ratio=1;  //�������λΪ0������λΪ1����һ���ٶ�
			 else if(((Rotate_value&0x08))&&((Rotate_value&0x04)==0)) 
				 speed_ratio=5;  //�ڶ����ٶ�
			 else if(((Rotate_value&0x08)==0)&&((Rotate_value&0x04)==0)) 
				 speed_ratio=0;
			 
			 Base_CMD.ward=plus_minus*caculed_ward;
			 Base_CMD.line_speed=speed_ratio*200;
			 /****************��������********************************************/
			 switch(Rotate_value&0x03)
			 {
				 case 0x01:
					 Base_CMD.Pos_Angle++;
					 break;
				 case 0x02:
					  Base_CMD.Pos_Angle--;
				   break;
				 default: 
					 break;
			 }
			 
			AimAngleUpdate(Base_CMD.Pos_Angle);
//			 //��ҡ�˱������и�ֵ
//			 Axis[0] = CMDData.buffer.axis_value0;
//			 Axis[1] = -CMDData.buffer.axis_value1;     //�����ǽ��з���任
//			 Axis[2] = CMDData.buffer.axis_value3;
//			 
//			 Robot_State(speed_flag,Axis);
//			 
//			 
			 
			 
//			 if(sendbuffer.RobotSpeed !=0)
//			 {
//			   Base_CMD.R_Ctrl_Flag = 0x01;
//			 }
//			   Base_CMD.ward = sendbuffer.RobotWard;
//			   Base_CMD.line_speed = sendbuffer.RobotSpeed;
//			   Base_CMD.Pos_Angle = sendbuffer.RockerPosAngel;
//	       Base_CMD.Angle_cnt = Angle_cnt;
			 
		/*-------------------������λ�ù���------------------------------------*/
			 
			 if((KEY1_value & BACK) && (!(key & BACK)))
			 {
				   Base_CMD.R_Ctrl_Flag = AimMode;
				   
				   Base_CMD.aimx = 0;
				   Base_CMD.aimy = -2400;
				 
				   SpinState = Spin_GetBall;
				 
				   Base_CMD.aimt = T_cal();
				 
				   AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
			 }
			 
			 if((KEY2_value & HOME) && (!(key2 & HOME)))
			 {
				 Base_CMD.R_Ctrl_Flag = AimMode;
				 
  			 Base_CMD.aimx = -500;;
				 Base_CMD.aimy = 5851;
				 
				 SpinState = Spin_GiveNormBall1;
				 	 
				 Base_CMD.aimt = 7000;
				 
				 AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);
			 }
			 
			 
			 if((KEY1_value & START) && (!(key & START)) )
			 {
				 Base_CMD.R_Ctrl_Flag = AimMode;
				 
				 Base_CMD.aimx = -350;   //����Ҫ����
				 Base_CMD.aimy = 8300;
				 
				 SpinState = Spin_GiveGodenBall;
				 
				 Base_CMD.aimt = T_cal();
				 
				 AfterGetAim(Base_CMD.aimx, Base_CMD.aimy, Base_CMD.aimt);				 
			 }
			 
			 //���������У��ʱ�İ���ֵ��¼���������´���
       key = KEY1_value;
			 key2 = KEY2_value;
}

uint32_t T_cal()
{
	uint32_t cal;
	
	cal = (uint32_t)sqrtf((Base_CMD.aimx - Coordinate.point.X) * (Base_CMD.aimx - Coordinate.point.X)
	                     + (Base_CMD.aimy - Coordinate.point.Y) * (Base_CMD.aimy - Coordinate.point.Y));
  
	if(cal < 2000)
	return (1.6 * cal);
	if(cal < 5000)
	return (1.3 * cal);
	if(cal < 10000)
	return (0.9 * cal);
  else
  return 0.7 * cal;		
}
