/**
  ******************************************************************************
  * @file    rtc.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  ******************************************************************************
  */
#ifndef __CH_LIB_RTC_H__
#define __CH_LIB_RTC_H__

#include <stdint.h>
#include "common.h"


typedef struct 
{
   uint16_t year;    /*!< ��Range from 200 to 2099.*/
   uint16_t month;   /*!< ��Range from 1 to 12.*/
   uint16_t day;     /*!< ��Range from 1 to 31 (depending on month).*/
   uint16_t hour;    /*!< ʱRange from 0 to 23.*/
   uint16_t minute;  /*!< ��Range from 0 to 59.*/
   uint8_t second;   /*!< ��Range from 0 to 59.*/
}RTC_DateTime_Type;


typedef enum
{
    kRTC_OScLoad_0PF,   /* �������� */
    kRTC_OScLoad_2PF,
    kRTC_OScLoad_4PF,
    kRTC_OScLoad_8PF,
    kRTC_OScLoad_16PF,
}RTC_OscLoad_Type;


typedef struct
{
    RTC_OscLoad_Type        oscLoad;            // OSC ���ݸ���
}RTC_InitTypeDef;

//!< interrupt and DMA select
typedef enum
{
    kRTC_IT_TimeAlarm,             /* ���������ж� */
    kRTC_IT_TimeOverflow,          /* ����ʱ������ж� */    
}RTC_ITDMAConfig_Type;

//!< CallbackType
typedef void (*RTC_CallBackType)(void);

//!<API functions
void RTC_QuickInit(void);
void RTC_GetDateTime(RTC_DateTime_Type * datetime);
void RTC_SetDateTime(RTC_DateTime_Type * datetime);
void RTC_ITDMAConfig(RTC_ITDMAConfig_Type config, bool status);
uint32_t RTC_IsTimeValid(void);
void RTC_CallbackInstall(RTC_CallBackType AppCBFun);
//!< function which may not be used
void RTC_Init(RTC_InitTypeDef * RTC_InitStruct);
int  RTC_GetWeekFromYMD(int year, int month, int days);
uint32_t RTC_GetTSR(void);
void RTC_SetTSR(uint32_t val);
void RTC_SetAlarm(RTC_DateTime_Type * datetime);
void RTC_SetCompensation(uint32_t compensationInterval, uint32_t timeCompensation);



#endif
