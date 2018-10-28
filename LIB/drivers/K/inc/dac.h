/**
  ******************************************************************************
  * @file    dac.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  ******************************************************************************
  */
  
#ifndef __CH_LIB_DAC_H__
#define __CH_LIB_DAC_H__

#include <stdint.h>


#define HW_DAC0     (0x00)  //DAC0ģ��
#define HW_DAC1     (0x01)  //DAC1ģ��


//!< DAC �ص���������
typedef void (*DAC_CallBackType)(void);


typedef enum
{
    kDAC_TriggerSoftware,  //�������
    kDAC_TriggerHardware,  //Ӳ������
}DAC_TriggerSelect_Type;

/*DAC ���ͻ���ģʽ ѡ�� */
typedef enum
{
    kDAC_Buffer_Disable,    //�ر�DAC����
    kDAC_Buffer_Normal,     //����DAC����
    kDAC_Buffer_Swing,      //dacҡ��ģʽ
    kDAC_Buffer_OneTimeScan,//DACһ�����
}DAC_BufferMode_Type;

typedef enum
{
    kDAC_Reference_1,       /* �ο���ѹԴ VREF_OUT */
    kDAC_Reference_2,       /* �ο���ѹԴ VDDA */
}DAC_ReferenceSelect_Type;

typedef struct
{
    uint32_t                    instance;      //DACģ���0~1
    DAC_TriggerSelect_Type      triggerMode;   //DAC����ģʽ
    DAC_BufferMode_Type         bufferMode;    //��������
    DAC_ReferenceSelect_Type    referenceMode; //�ο�Դѡ��
}DAC_InitTypeDef;


//!< DAC�жϼ�DMA����ѡ��
typedef enum
{
    kDAC_DMA_Disable,
    kDAC_IT_Disable,
    kDAC_IT_BufferPointer_WaterMark,
    kDAC_IT_BufferPointer_TopFlag,      /* when read pointer reach 0 */
    kDAC_IT_BufferPointer_BottomFlag,   /* when read pointer reach upper limit */
    kDAC_DMA_BufferPointer_WaterMark,
    kDAC_DMA_BufferPointer_TopFlag,    
    kDAC_DMA_BufferPointer_BottomFlag, 
}DAC_ITDMAConfig_Type;
 

typedef enum
{
    kDAC_WaterMark_1Word,
    kDAC_WaterMark_2Word,
    kDAC_WaterMark_3Word,
    kDAC_WaterMark_4Word,
}DAC_WaterMarkMode_Type;

 //!< API functions
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct);
void DAC_CallbackInstall(uint8_t instance, DAC_CallBackType AppCBFun);
void DAC_ITDMAConfig(uint32_t instance, DAC_ITDMAConfig_Type config);
uint32_t DAC_GetBufferReadPointer(uint32_t instance);
void DAC_SetBufferReadPointer(uint32_t instance, uint32_t value);
void DAC_SetBufferUpperLimit(uint32_t instance, uint32_t value);
void DAC_ITDMAConfig(uint32_t instance, DAC_ITDMAConfig_Type config);
void DAC_SetBufferValue(uint32_t instance, uint16_t* buf, uint8_t len);
void DAC_SetWaterMark(uint32_t instance, DAC_WaterMarkMode_Type value);
void DAC_SoftwareStartConversion(uint32_t instance);

#endif



