/**
  ******************************************************************************
  * @file    can.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.4.10
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  ******************************************************************************
  */
  
#ifndef __CH_LIB_CAN_H__
#define __CH_LIB_CAN_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "common.h"  

//!< hardware instances
#define HW_CAN0  (0x00U)  //CAN0ģ��
#define HW_CAN1  (0x01U)  //CAN1ģ��  
     
//CAN�����ٶ�ѡ��
typedef enum 
{
	kCAN_Baudrate_25K,   //CANͨ���ٶ�Ϊ25KHz
	kCAN_Baudrate_50K,
	kCAN_Baudrate_100K,
	kCAN_Baudrate_125K,
	kCAN_Baudrate_250K,
	kCAN_Baudrate_500K,
	kCAN_Baudrate_1000K, //CANͨ���ٶ�Ϊ1MHz
}CAN_Baudrate_Type;

typedef enum
{
    kCAN_Frame_Remote,  //Զ��֡
    kCAN_Frame_Data,    //����֡
}CAN_Frame_Type;


typedef struct
{
    uint32_t                instance;  //CANͨ�Ŷ˿� HW_CAN0/HW_CAN1
    CAN_Baudrate_Type       baudrate;  //CANͨ���ٶ�����
}CAN_InitTypeDef;


//!< CAN QuickInit macro
#define CAN1_TX_PE24_RX_PE25   (0xB0A1U)  //CAN1ģ�� ��������ΪPTE24 ��������ΪPTE25
#define CAN0_TX_PA12_RX_PA13   (0x9880U)  //CAN0ģ�� ��������ΪPTA12 ��������ΪPTA13
#define CAN0_TX_PB18_RX_PB19   (0xA488U)  //CAN0ģ�� ��������ΪPTB18 ��������ΪPTB19
#define CAN1_TX_PC17_RX_PC16   (0xA091U)  //CAN1ģ�� ��������ΪPTC17 ��������ΪPTC16


/*!< �жϼ�DMA���� */
typedef enum
{
    kCAN_IT_Tx_Disable,  //�رշ����ж�
    kCAN_IT_Rx_Disable,  //�رս����ж�
    kCAN_IT_Tx,          //���������ж�
    kCAN_IT_RX,          //���������ж�
}CAN_ITDMAConfig_Type;

/*!< CAN �ص��������� */
typedef void (*CAN_CallBackType)(void);

//!< API functions
void CAN_SetReceiveMask(uint32_t instance, uint32_t mb, uint32_t mask);
uint32_t CAN_IsMessageBoxBusy(uint32_t instance, uint32_t mb);
uint32_t CAN_WriteData(uint32_t instance, uint32_t mb, uint32_t id, uint8_t* buf, uint8_t len);
uint32_t CAN_ReadData(uint32_t instance, uint32_t mb, uint8_t *buf, uint8_t *len);
uint32_t CAN_QuickInit(uint32_t CANxMAP, CAN_Baudrate_Type baudrate);
void CAN_ITDMAConfig(uint32_t instance, uint32_t mb, CAN_ITDMAConfig_Type config);
void CAN_SetReceiveMB(uint32_t instance, uint32_t mb, uint32_t id);
void CAN_CallbackInstall(uint32_t instance, CAN_CallBackType AppCBFun);
void CAN_Init(CAN_InitTypeDef* CAN_InitStruct);

#ifdef __cplusplus
}
#endif

#endif
