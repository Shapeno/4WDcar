/********************************************************************************
* @file    driver.h
* @author  DengXY
* @version V1.1
* @creatdate    2018.10.14
* @brief   
* @note    ³µÂÖÇý¶¯
*******************************************************************************/
#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <stdint.h>

#define PWM_Freq 1000

typedef enum{
   Wheel_1,
   Wheel_2,
   Wheel_3,
   Wheel_4,
}Wheel_e;

void Driver_Init(void);
void Wheel_Speed_Ctrl(Wheel_e Num ,int speed);
void Speed_Set(int16_t base,int16_t diff);

#endif