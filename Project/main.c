/*******************************************************************************
* @file    main.c
* @author  DengXY
* @date    2018.10.28
* @brief   
* @note    四驱小车的循迹程序（四传感器）
*******************************************************************************/
#include "gpio.h"
#include "common.h"
#include "driver.h"
#include "led.h"
#include "photosensor.h"
#include "control.h"
#include "uart.h"

 
int main(void)
{
    UART_QuickInit(UART0_RX_PA15_TX_PA14,115200);
    DelayInit();
    Driver_Init();
    LED_Init();
    Sensor_Init(100);   //初始化传感器轮询，设置轮询周期us
    Ctrl_Init(100);     //初始化控制函数，设置机器周期us
    while(1)
    {
        GPIO_SetBit(HW_GPIOA,17);
        DelayMs(500);
        GPIO_ResetBit(HW_GPIOA,17);
        DelayMs(500);
        printf("state:%d%d%d%d\n\t\t",Get_Sensor_State()/8,Get_Sensor_State()%8/4,Get_Sensor_State()%4/2,Get_Sensor_State()%2); //串口打印传感器信息
    }
}