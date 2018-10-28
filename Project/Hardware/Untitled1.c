/*******************************************************************************
* @file    uart0.c
* @author  DengXY
* @version V0.1
* @date    2018.10.22
* @brief   
* @note    ´®¿ÚÍ¨ÐÅ
*******************************************************************************/

void Uart0_Init(void){
    uint8_t uart_0;
    GPIO_QuickInit(HW_GPIOA,1,kGPIO_Mode_OPP);
    uart_0=UART_QuickInit(UART0_RX_PA15_TX_PA14,115200);
}