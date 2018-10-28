/*******************************************************************************
* @file    control.c
* @author  DengXY
* @version V0.1
* @date    2018.10.22
* @brief   
* @note    过程控制
*******************************************************************************/
#include "control.h"
#include "driver.h"
#include "pit.h"
#include "photosensor.h"

volatile uint8_t trn_state=0;                   //转向状态标志1:右转 2：中央传感器离开赛道 3：左转
volatile uint16_t counter=Finish_Point_Delay;   //中央传感器离开赛道计时


/**
 * @brief  初始化控制函数
 */
void Ctrl_Init(uint16_t time_us){
    PIT_QuickInit(HW_PIT_CH0,time_us);
    PIT_CallbackInstall(HW_PIT_CH0,Ctrl_ISR);
    PIT_ITDMAConfig(HW_PIT_CH0, kPIT_IT_TOF, true);
}

/**
 * @brief  控制中断函数
 */
static void Ctrl_ISR(void){
    if(trn_state&(1<<0)^(1<<0)){
        Str_Ctrl();
    }
    Turn_Ctrl();
}

/**
 * @brief  直线路径控制函数
 */
void Str_Ctrl(void){
    if(Get_Sensor_State()&(1<<1)&&Get_Sensor_State()&(1<<2)){
        Speed_Set(Str_Speed,0);
        counter=Finish_Point_Delay;
        trn_state=0;
    }
    else if(Get_Sensor_State()&(1<<1)^(1<<1)&&Get_Sensor_State()&(1<<2)){
        Speed_Set(0,-Str_Speed_Diff);
    }
    else if(Get_Sensor_State()&(1<<1)&&Get_Sensor_State()&(1<<2)^(1<<2)){
        Speed_Set(0,Str_Speed_Diff);
    }
}

/**
 * @brief  转直角弯控制函数
 */
void Turn_Ctrl(void){
    if(Get_Sensor_State()&(1<<1)^(1<<1)&&Get_Sensor_State()&(1<<2)^(1<<2)&&trn_state==0){
        trn_state=2;
    }
    switch(trn_state){
    case 2:
        Speed_Set(Str_Speed,0);
        counter--;
        if(counter==0){
            trn_state=1;
            counter=Finish_Point_Delay;
        }
        if(Get_Sensor_State()&(1<<0)){
            trn_state=1;
            }
        else if(Get_Sensor_State()&(1<<3)){
            trn_state=3;
        }break;
    case 1:
        Speed_Set(0,Trn_Speed);
        if(Get_Sensor_State()&(1<<1)&&Get_Sensor_State()&(1<<2)){
            Speed_Set(Str_Speed,0);
            trn_state=0;
        }break;
    case 3:
        Speed_Set(0,-Trn_Speed);
        if(Get_Sensor_State()&(1<<1)&&Get_Sensor_State()&(1<<2)){
            Speed_Set(Str_Speed,0);
            trn_state=0;
        }break;
    }
            
}