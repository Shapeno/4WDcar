/*******************************************************************************
* @file    main.c
* @author  DengXY
* @date    2018.10.28
* @brief   
* @note    ����С����ѭ�������Ĵ�������
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
    Sensor_Init(100);   //��ʼ����������ѯ��������ѯ����us
    Ctrl_Init(100);     //��ʼ�����ƺ��������û�������us
    while(1)
    {
        GPIO_SetBit(HW_GPIOA,17);
        DelayMs(500);
        GPIO_ResetBit(HW_GPIOA,17);
        DelayMs(500);
        printf("state:%d%d%d%d\n\t\t",Get_Sensor_State()/8,Get_Sensor_State()%8/4,Get_Sensor_State()%4/2,Get_Sensor_State()%2); //���ڴ�ӡ��������Ϣ
    }
}