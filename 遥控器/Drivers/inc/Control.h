#ifndef __CONTROL_H
#define __CONTROL_H

/********************��ֵ��*******************************/
/*    key1    */
#define BACK              0x40
#define START             0x80
#define X_BUTTON                 0x04
#define Y_BUTTON               0x08
#define B_BUTTON                 0x02
#define A_BUTTON               0x01
#define LB                0x10
#define RB                0x20
/*     key2    */
#define HOME              0x01
#define L3                0x02
#define R3                0x04
#define LT                0x08
#define RT                0x10
#define ward_button_x     0x20
#define ward_button_y     0x40
/***********************************************************/

/******************ָ���************************************/
#define NORM_STEP1   0x07  //��ͨ�򽻽�1��Ӧ�Ķ�����
#define R_PUSH   0x03  //�����Ӧ�Ķ�����
#define R_FETCH   0x04  //���ն�Ӧ�Ķ�����
#define BOTH_PUSH    0x03  //ȫȡ��Ӧ�Ķ�����
#define BOTH_FETCH  0x04  //ȫ�ն�Ӧ�Ķ�����
#define L_FETCH   0x04  //���ն�Ӧ�Ķ�����
#define L_PUSH   0x03  //�����Ӧ�Ķ�����
#define NORM_STEP2   0x08  //��ͨ�򽻽�2��Ӧ�Ķ�����
#define GOLDEN_HANDOVER   0x09  //��ɫ�򽻽Ӷ�Ӧ�Ķ�����
/************************************************************/

/***********************�������******************************/
#define LEFT        0x01
#define RIGHT       0x02
#define BOTH        0x03
#define SPECIAL     0x05   //��϶�����
/************************************************************/

void Controler_handle(void);


#endif
