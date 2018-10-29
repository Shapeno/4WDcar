/********************************************************************************
* @file    photosensor.c
* @author  DengXY
* @version V1.1
* @creatdate    2018.10.16
* @brief   
* @note    ��紫����
*******************************************************************************/
#include "photosensor.h"
#include "gpio.h"
#include "pit.h"

uint8_t sensor_state=0; //������״̬��־

/**
 * @brief  ��������ʼ������
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
 * @brief  �������жϺ���
 */
static void Sensor_ISR(void){
    GPIO_ReadBit(HW_GPIOB,0)?Set_Sensor_State_Bit(0):Reset_Sensor_State_Bit(0);
    GPIO_ReadBit(HW_GPIOB,1)?Set_Sensor_State_Bit(1):Reset_Sensor_State_Bit(1);
    GPIO_ReadBit(HW_GPIOB,2)?Set_Sensor_State_Bit(2):Reset_Sensor_State_Bit(2);
    GPIO_ReadBit(HW_GPIOB,3)?Set_Sensor_State_Bit(3):Reset_Sensor_State_Bit(3);
}

/**
 * @brief  ��λ������״̬��־
 * @code
 *      //������״̬��־��0λ��1
 *      Set_Sensor_State_Bit(0); 
 * @endcode         
 * @param  n: ��������־λ
 */
void Set_Sensor_State_Bit(uint8_t n){
    sensor_state|=(1<<n);
}

void Reset_Sensor_State_Bit(uint8_t n){
    sensor_state&=~(1<<n);
}

/**
 * @brief  ��ȡ������״̬��־(0��׵�,1��ڵ�)     
 */
uint8_t Get_Sensor_State(void){
    return sensor_state;
}
/**
 * @brief  ��ȡ������״̬��nδ״̬��־(0��׵�,1��ڵ�)
 * @code
 *      //��ȡ������״̬��־��0λ״̬
 *      state=Get_Sensor_State_Bit(0);
 * @param  n: ��������־λ     
 */
uint8_t Get_Sensor_State_Bit(uint8_t n){
    return (sensor_state|(1<<n))>>n;
}