/********************************************************************************
* @file    photosensor.c
* @author  DengXY
* @version V1.1
* @creatdate    2018.10.16
* @brief   
* @note    光电传感器
*******************************************************************************/
#include "photosensor.h"
#include "gpio.h"
#include "pit.h"

uint8_t sensor_state=0; //传感器状态标志

/**
 * @brief  传感器初始化函数
 */
void Sensor_Init(uint16_t time_us){
    GPIO_QuickInit(HW_GPIOB,0,kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOB,1,kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOB,2,kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOB,3,kGPIO_Mode_IPD);
    
    PIT_QuickInit(HW_PIT_CH1,time_us);
    PIT_CallbackInstall(HW_PIT_CH1,Sensor_ISR);
    PIT_ITDMAConfig(HW_PIT_CH1, kPIT_IT_TOF, true);
}

/**
 * @brief  传感器中断函数
 */
static void Sensor_ISR(void){
    GPIO_ReadBit(HW_GPIOB,0)?Set_Sensor_State_Bit(0):Reset_Sensor_State_Bit(0);
    GPIO_ReadBit(HW_GPIOB,1)?Set_Sensor_State_Bit(1):Reset_Sensor_State_Bit(1);
    GPIO_ReadBit(HW_GPIOB,2)?Set_Sensor_State_Bit(2):Reset_Sensor_State_Bit(2);
    GPIO_ReadBit(HW_GPIOB,3)?Set_Sensor_State_Bit(3):Reset_Sensor_State_Bit(3);
}

/**
 * @brief  置位传感器状态标志
 * @code
 *      //传感器状态标志第0位置1
 *      Set_Sensor_State_Bit(0); 
 * @endcode         
 * @param  n: 传感器标志位
 */
void Set_Sensor_State_Bit(uint8_t n){
    sensor_state|=(1<<n);
}

void Reset_Sensor_State_Bit(uint8_t n){
    sensor_state&=~(1<<n);
}

/**
 * @brief  获取传感器状态标志(0表白底,1表黑底)     
 */
uint8_t Get_Sensor_State(void){
    return sensor_state;
}
/**
 * @brief  获取传感器状态第n未状态标志(0表白底,1表黑底)
 * @code
 *      //获取传感器状态标志第0位状态
 *      state=Get_Sensor_State_Bit(0);
 * @param  n: 传感器标志位     
 */
uint8_t Get_Sensor_State_Bit(uint8_t n){
    return (sensor_state|(1<<n))>>n;
}