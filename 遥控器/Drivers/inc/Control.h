#ifndef __CONTROL_H
#define __CONTROL_H

/********************键值宏*******************************/
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

/******************指令宏************************************/
#define NORM_STEP1   0x07  //普通球交接1对应的动作组
#define R_PUSH   0x03  //右伸对应的动作组
#define R_FETCH   0x04  //右收对应的动作组
#define BOTH_PUSH    0x03  //全取对应的动作组
#define BOTH_FETCH  0x04  //全收对应的动作组
#define L_FETCH   0x04  //左收对应的动作组
#define L_PUSH   0x03  //左伸对应的动作组
#define NORM_STEP2   0x08  //普通球交接2对应的动作组
#define GOLDEN_HANDOVER   0x09  //金色球交接对应的动作组
/************************************************************/

/***********************动作组宏******************************/
#define LEFT        0x01
#define RIGHT       0x02
#define BOTH        0x03
#define SPECIAL     0x05   //组合动作组
/************************************************************/

void Controler_handle(void);


#endif
