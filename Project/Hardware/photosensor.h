/*******************************************************************************
* @file    photosensor.h
* @author  DengXY
* @version V0.1
* @date    2018.10.16
* @brief   
* @note    光电传感器
*******************************************************************************/
#ifndef __PHOTOSENSOR_H__
#define __PHOTOSENSOR_H__

#include <stdint.h>

void Sensor_Init(uint16_t time_us);
static void Sensor_ISR(void);
void Set_Sensor_State_Bit(uint8_t n);
void Reset_Sensor_State_Bit(uint8_t n);
uint8_t Get_Sensor_State(void);

#endif