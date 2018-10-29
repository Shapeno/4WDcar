/*******************************************************************************
* @file    led.c
* @author  DengXY
* @version V1.1
* @creatdate    2018.10.16
* @brief   
* @note    LED
*******************************************************************************/
#include "led.h"
#include "gpio.h"

void LED_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.instance = HW_GPIOA;
    GPIO_InitStruct.mode = kGPIO_Mode_OPP;
    GPIO_InitStruct.pinx = 17;
    GPIO_Init(&GPIO_InitStruct);
}