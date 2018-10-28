/*******************************************************************************
* @file    uart0.c
* @author  DengXY
* @version V0.1
* @date    2018.10.22
* @brief   
* @note    ´®¿ÚÍ¨ÐÅ
*******************************************************************************/
#include "uart.h"
#include "gpio.h"
#include "uart0.h"

uint8_t Uart0_Init(void){
    uint8_t uart;
    uart=UART_QuickInit(UART0_RX_PA15_TX_PA14,115200);
    return uart;
}