/*******************************************************************************
* @file    driver.c
* @author  DengXY
* @version V1.1
* @creatdate    2018.10.14
* @brief   
* @note    车轮驱动
*******************************************************************************/
#include "driver.h"
#include "gpio.h"
#include "ftm.h"

/**
 * @brief  轮子初始化函数
 */
void Driver_Init(void){
   GPIO_QuickInit(HW_GPIOA, 4, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA, 5, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA, 6, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA, 7, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA, 8, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA, 9, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA, 10, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA, 11, kGPIO_Mode_OPP);
   
   FTM_PWM_QuickInit(FTM0_CH1_PA04, kPWM_EdgeAligned, PWM_Freq);
   FTM_PWM_QuickInit(FTM0_CH2_PA05, kPWM_EdgeAligned, PWM_Freq);
   FTM_PWM_QuickInit(FTM0_CH3_PA06, kPWM_EdgeAligned, PWM_Freq);
   FTM_PWM_QuickInit(FTM0_CH4_PA07, kPWM_EdgeAligned, PWM_Freq);
   FTM_PWM_QuickInit(FTM1_CH0_PA08, kPWM_EdgeAligned, PWM_Freq);
   FTM_PWM_QuickInit(FTM1_CH1_PA09, kPWM_EdgeAligned, PWM_Freq);
   FTM_PWM_QuickInit(FTM2_CH0_PA10, kPWM_EdgeAligned, PWM_Freq);
   FTM_PWM_QuickInit(FTM2_CH1_PA11, kPWM_EdgeAligned, PWM_Freq);
}

/**
 * @brief  轮子的速度控制函数
 * @code
 *      //控制1号车轮的速度为50%
 *      Wheel_Speed_Ctrl(Wheel_1,5000); 
 * @endcode         
 * @param  Num: 轮子编号
 * @param  speed:PWM输出占空比(越大速度越快)
 */
void Wheel_Speed_Ctrl(Wheel_e Num ,int speed){
   
   if(speed>=-10000&&speed<=10000)
      switch(Num){
         
          case Wheel_1:{
             if(speed>=0){
                FTM_PWM_ChangeDuty(HW_FTM0,2,speed);
                FTM_PWM_ChangeDuty(HW_FTM0,4,0);
             }
             else{
                FTM_PWM_ChangeDuty(HW_FTM0,2,0);
                FTM_PWM_ChangeDuty(HW_FTM0,4,-speed);
             }
          };break;
          
          case Wheel_2:{
             if(speed>=0){
                FTM_PWM_ChangeDuty(HW_FTM1,1,speed);
                FTM_PWM_ChangeDuty(HW_FTM2,1,0);
             }
             else{
                FTM_PWM_ChangeDuty(HW_FTM1,1,0);
                FTM_PWM_ChangeDuty(HW_FTM2,1,-speed);
             }
          };break;
          
          case Wheel_3:{
             if(speed>=0){
                FTM_PWM_ChangeDuty(HW_FTM0,3,0);
                FTM_PWM_ChangeDuty(HW_FTM0,1,speed);
             }
             else{
                FTM_PWM_ChangeDuty(HW_FTM0,3,-speed);
                FTM_PWM_ChangeDuty(HW_FTM0,1,0);
             }
          };break;
          
          case Wheel_4:{
             if(speed>=0){
                FTM_PWM_ChangeDuty(HW_FTM1,0,speed);
                FTM_PWM_ChangeDuty(HW_FTM2,0,0);
             }
             else{
                FTM_PWM_ChangeDuty(HW_FTM1,0,0);
                FTM_PWM_ChangeDuty(HW_FTM2,0,-speed);
             }
          };break;
      }
   else return ;
}

/**
 * @brief  轮子的速度设定函数
 * @code
 *      //设定直线速度2000，无转向
 *      Speed_Set(2000, 0); 
 * @endcode         
 * @param  base: 基速度，+向前
 * param   diff:速度差，+向右
 */
void Speed_Set(int16_t base,int16_t diff){
    Wheel_Speed_Ctrl(Wheel_1,base-diff);
    Wheel_Speed_Ctrl(Wheel_2,base+diff);
    Wheel_Speed_Ctrl(Wheel_3,base+diff);
    Wheel_Speed_Ctrl(Wheel_4,base-diff);
}