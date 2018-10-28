/**
  ******************************************************************************
  * @file    gpio.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬGPIOģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */
#ifndef __CH_LIB_GPIO_H__
#define __CH_LIB_GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "common.h"
#include <stdint.h>

/* GPIO�˿ڶ��� */
#define HW_GPIOA  (0x00U)   /* hardware GPIOA */
#define HW_GPIOB  (0x01U)
#define HW_GPIOC  (0x02U)
#define HW_GPIOD  (0x03U)
#define HW_GPIOE  (0x04U)
#define HW_GPIOF  (0x05U)


/* �˿ڸ���ѡ�� �����ֲ� Signal Multiplexing and Signal Descriptions �½�ѡ���� */
typedef enum
{
    kPinAlt0,  //0���ܸ���
    kPinAlt1,  //1���ܸ���
    kPinAlt2,  //2���ܸ���
    kPinAlt3,  //3���ܸ���
    kPinAlt4,  //4���ܸ���
    kPinAlt5,  //5���ܸ���
    kPinAlt6,  //6���ܸ���
    kPinAlt7,  //7���ܸ���
}PORT_PinMux_Type;

/* �˿����������� ��������ֵԼΪ20K */
typedef enum
{
    kPullDisabled,  //�ر����������蹦��
    kPullUp,        //�����������蹦��
    kPullDown,      //�����������蹦��
}PORT_Pull_Type;

/* GPIO�˿�ģʽ���� */
typedef enum
{
    kGPIO_Mode_IFT = 0x00,       /* �������� */
    kGPIO_Mode_IPD = 0x01,       /* �������� */
    kGPIO_Mode_IPU = 0x02,       /* �������� */
    kGPIO_Mode_OOD = 0x03,       /* ��©��� */
    kGPIO_Mode_OPP = 0x04,       /* ������� */
}GPIO_Mode_Type;

/* �˿��������ģʽѡ�� */
typedef enum
{
    kInput,                  /* ��������ģʽ */
    kOutput,                 /* �������ģʽ */
}GPIO_PinConfig_Type;

/* �˿��жϼ�DMA����ѡ�� */
typedef enum
{
    kGPIO_DMA_RisingEdge,	      //�����ش���DMA
    kGPIO_DMA_FallingEdge,        //�½��ش���DMA
    kGPIO_DMA_RisingFallingEdge,  //�����غ��½��ش���DMA
    kGPIO_IT_Low,                 //�͵�ƽ�����ж�
    kGPIO_IT_RisingEdge,          //�����ش����ж�
    kGPIO_IT_FallingEdge,         //�½��ش����ж�
    kGPIO_IT_RisingFallingEdge,   //�����غ��½��ش����ж�
    kGPIO_IT_High,                //�ߵ�ƽ�����ж�
}GPIO_ITDMAConfig_Type;

/* �˿ڳ�ʼ���ṹ�� */
typedef struct
{
    uint8_t                instance;    //���Ŷ˿�HW_GPIOA~HW_GPIOF
    GPIO_Mode_Type         mode;        //����ģʽ
    uint32_t               pinx;        //���ź�0~31
}GPIO_InitTypeDef;

/* �˿��жϻص��������� */
typedef void (*GPIO_CallBackType)(uint32_t pinxArray);


/* λ������ �ڴ�ƫ�Ƽ��� ��� Cortex-M4 Generic User Guide 2.25 */
/* CM4����2��bitband���� 0x2000_0000-0x200F_FFFF ӳ���� 0x2200_0000-0x23FF_FFFF
                         0x4000_0000-0x4000_FFFF ӳ���� 0x4200_0000-0x43FF_FFFF
*/
#define PAout(n)   BITBAND_REG(PTA->PDOR, n)
#define PAin(n)    BITBAND_REG(PTA->PDIR, n)
  
#define PBout(n)   BITBAND_REG(PTB->PDOR, n)
#define PBin(n)    BITBAND_REG(PTB->PDIR, n)

#define PCout(n)   BITBAND_REG(PTC->PDOR, n)
#define PCin(n)    BITBAND_REG(PTC->PDIR, n)

#define PDout(n)   BITBAND_REG(PTD->PDOR, n)
#define PDin(n)    BITBAND_REG(PTD->PDIR, n)

#define PEout(n)   BITBAND_REG(PTE->PDOR, n)
#define PEin(n)    BITBAND_REG(PTE->PDIR, n)

#define PFout(n)   BITBAND_REG(PTF->PDOR, n)
#define PFin(n)    BITBAND_REG(PTF->PDIR, n)

#define PGout(n)   BITBAND_REG(PTG->PDOR, n)
#define PGin(n)    BITBAND_REG(PTG->PDIR, n)

//!< API functions
void GPIO_Init(GPIO_InitTypeDef * GPIO_InitStruct);
uint8_t GPIO_QuickInit(uint32_t instance, uint32_t pinx, GPIO_Mode_Type mode);
void GPIO_WriteBit(uint32_t instance, uint8_t pin, uint8_t data);
uint8_t GPIO_ReadBit(uint32_t instance, uint8_t pin);
void GPIO_ToggleBit(uint32_t instance, uint8_t pin);
void GPIO_SetBit(uint32_t instance, uint32_t pin);
void GPIO_ResetBit(uint32_t instance, uint32_t pin);
void GPIO_ITDMAConfig(uint32_t instance, uint8_t pin, GPIO_ITDMAConfig_Type config, bool status);
void GPIO_CallbackInstall(uint32_t instance, GPIO_CallBackType AppCBFun);
//!< low level functions
void PORT_PinPullConfig(uint32_t instance, uint8_t pin, PORT_Pull_Type pull);
void GPIO_PinConfig(uint32_t instance, uint8_t pin, GPIO_PinConfig_Type mode);
void PORT_PinMuxConfig(uint32_t instance, uint8_t pin, PORT_PinMux_Type pinMux);
uint32_t GPIO_ReadPort(uint32_t instance);
void GPIO_WritePort(uint32_t instance, uint32_t data);
void PORT_PinOpenDrainConfig(uint32_t instance, uint8_t pin, FunctionalState newState);
void PORT_PinPassiveFilterConfig(uint32_t instance, uint8_t pin, FunctionalState newState);
    
#ifdef __cplusplus
}
#endif

#endif


