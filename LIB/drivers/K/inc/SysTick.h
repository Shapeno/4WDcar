/**
  ******************************************************************************
  * @file    systick.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬARM�ں��е�SysTickģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */
#ifndef __CH_LIB_SYSTICK_H__
#define __CH_LIB_SYSTICK_H__

#include <stdint.h>
#include <stdbool.h>

//API functions
void SYSTICK_DelayInit(void);
void SYSTICK_Init(uint32_t timeInUs);
void SYSTICK_Cmd(bool NewState);
void SYSTICK_ITConfig(bool NewState);
void SYSTICK_DelayUs(uint32_t us);
void SYSTICK_DelayMs(uint32_t ms);
uint32_t SYSTICK_GetVal(void);

#endif

