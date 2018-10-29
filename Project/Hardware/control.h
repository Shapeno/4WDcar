/*******************************************************************************
* @file    control.h
* @author  DengXY
* @version V1.1
* @creatdate    2018.10.22
* @brief   
* @note    过程控制
*******************************************************************************/
#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdint.h>

#define Str_Speed 1600
#define Str_Speed_Diff 3200
#define Trn_Speed 2000
#define Finish_Point_Delay 2000 //2000*100us

void Ctrl_Init(uint16_t time_us);     //定义机器周期控制函数
void Str_Ctrl(void);
void Turn_Ctrl(void);
static void Ctrl_ISR(void);

#endif