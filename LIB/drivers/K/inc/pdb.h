/**
  ******************************************************************************
  * @file    pdb.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief  ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  ******************************************************************************
  */
#ifndef __CH_LIB_PDB_H__
#define __CH_LIB_PDB_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "common.h"
#include <stdint.h>
     
    
#define HW_PDB_CH0
#define HW_PDB_CH1
     
#define HW_PDB_TRIG_CH0
#define HW_PDB_TRIG_CH1
     
//!< PIT CallBack Type
typedef void (*PDB_CallBackType)(void);

     
/* PDB ����Դ �ο�Chip configuration�½� */
typedef enum
{
    kPDB_TriggerSrc0,
    kPDB_TriggerSrc1,
    kPDB_TriggerSrc2,
    kPDB_TriggerSrc3,
    kPDB_TriggerSrc4,
    kPDB_TriggerSrc5,
    kPDB_TriggerSrc6,
    kPDB_TriggerSrc7,
    kPDB_TriggerSrc8,
    kPDB_TriggerSrc9, 
    kPDB_TriggerSrc10,
    kPDB_TriggerSrc11,
    kPDB_TriggerSrc12,
    kPDB_TriggerSrc13,
    kPDB_TriggerSrc14,
    kPDB_SoftwareTrigger, 
}PDB_TriggerSrc_Type;

typedef enum
{
	kPWM_EdgeAligned,           //���ض��� ���
	kPWM_Combine,               //���ģʽ
    kPWM_Complementary,         //����ģʽ �������ģʽ ����Chl(n) �� Chl(n+1) �ǻ������
}FTM_PWM_Mode_Type;
     
typedef struct
{
    uint32_t                srcClock;           // ʱ��ԴƵ��
    PDB_TriggerSrc_Type     inputTrigSource;    // �������봥��Դ
    bool                    isContinuesMode;    //�Ƿ�����������ģʽ
    uint32_t                timeInUs;
}PDB_InitTypeDef;

/*!< �жϼ�DMA���� */
typedef enum
{
    kPDB_IT_CF,
    kPDB_DMA_CF,
}PDB_ITDMAConfig_Type;


//!< API functions
void PDB_QuickInit(PDB_TriggerSrc_Type triggerSrc, uint32_t timeInUs);
void PDB_Init(PDB_InitTypeDef * PDB_InitStruct);
void PDB_SoftwareTrigger(void);
void PDB_ITDMAConfig(PDB_ITDMAConfig_Type config, bool status);
void PDB_CallbackInstall(PDB_CallBackType AppCBFun);
uint32_t PDB_GetMODValue(void);
//!< ADC trigger function
void PDB_SetADCPreTrigger(uint32_t adcInstance, uint32_t adcMux, uint32_t dlyValue, bool status);
void PDB_SetBackToBackMode(uint32_t adcInstance, uint32_t adcMux, bool status);


#ifdef __cplusplus
}
#endif

#endif

