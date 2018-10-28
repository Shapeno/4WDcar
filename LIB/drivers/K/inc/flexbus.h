/**
  ******************************************************************************
  * @file    flexbus.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬFlexBusģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */
#ifndef __CH_LIB_FLEXBUS_H__
#define __CH_LIB_FLEXBUS_H__

#include <stdint.h>
#include <stdbool.h>

//!< Flexbus ���ݶ��䷽ʽѡ��
#define kFLEXBUS_DataLeftAligned   (0x00)  //���������
#define kFLEXBUS_DataRightAligned  (0x01)  //�����Ҷ���

//!< Flexbus �Զ�Ӧ���ź�ʹ��
#define kFLEXBUS_AutoAckEnable     (0x00)  //�Զ�Ӧ��ʹ��
#define kFLEXBUS_AutoAckDisable    (0x01)  //�ر��Զ�Ӧ��

//!< Flexbus �˿�λ��ѡ��
#define kFLEXBUS_PortSize_8Bit     (0x01)  //�˿����ݿ�ȣ�8λ
#define kFLEXBUS_PortSize_16Bit    (0x02)  //�˿����ݿ�ȣ�16λ
#define kFLEXBUS_PortSize_32Bit    (0x00)  //�˿����ݿ�ȣ�32λ

//!< Flexbus Ƭѡ�ź�ѡ�� 
#define kFLEXBUS_CS0               (0x00)  //0��Ƭѡ�ź�
#define kFLEXBUS_CS1               (0x01)  //1��Ƭѡ�ź�
#define kFLEXBUS_CS2               (0x02)  //2��Ƭѡ�ź�
#define kFLEXBUS_CS3               (0x03)  //3��Ƭѡ�ź�
#define kFLEXBUS_CS4               (0x04)  //4��Ƭѡ�ź�
#define kFLEXBUS_CS5               (0x05)  //5��Ƭѡ�ź�

//!< Flexbus Ƭѡ��Χ �ο�ѡ��
#define kFLEXBUS_ADSpace_64KByte        (0x00) //���SRAM�ߴ� 64k
#define kFLEXBUS_ADSpace_128KByte       (0x01) //���SRAM�ߴ� 128k
#define kFLEXBUS_ADSpace_512KByte       (0x07) //���SRAM�ߴ� 512k
#define kFLEXBUS_ADSpace_1MByte         (0x0F) //���SRAM�ߴ� 1m

//!< Flexbus BE�źſ���ģʽѡ��
#define kFLEXBUS_BE_AssertedWrite       (0x00) //BE�źŹ��ܣ�д
#define kFLEXBUS_BE_AssertedReadWrite   (0x01) //BE�źŹ��ܣ���д


typedef enum
{
    kFLEXBUS_CSPMCR_Group1,
    kFLEXBUS_CSPMCR_Group2,  
    kFLEXBUS_CSPMCR_Group3,
    kFLEXBUS_CSPMCR_Group4,
    kFLEXBUS_CSPMCR_Group5,    
}FLEXBUS_PortMultiplexingSelect_Type;

//!< Flexus �����źŸ���ѡ��
#define kFLEXBUS_CSPMCR_GROUP1_ALE      (0x00)
#define kFLEXBUS_CSPMCR_GROUP1_CS1      (0x01)
#define kFLEXBUS_CSPMCR_GROUP1_TS       (0x02)
#define kFLEXBUS_CSPMCR_GROUP2_CS4      (0x00)
#define kFLEXBUS_CSPMCR_GROUP2_TSIZ0    (0x01)
#define kFLEXBUS_CSPMCR_GROUP2_BE_31_24 (0x02)
#define kFLEXBUS_CSPMCR_GROUP3_CS5      (0x00)
#define kFLEXBUS_CSPMCR_GROUP3_TSIZ1    (0x01)
#define kFLEXBUS_CSPMCR_GROUP3_BE_23_16 (0x02)
#define kFLEXBUS_CSPMCR_GROUP4_TBST     (0x00)
#define kFLEXBUS_CSPMCR_GROUP4_CS2      (0x01)
#define kFLEXBUS_CSPMCR_GROUP4_BE_15_8  (0x02)
#define kFLEXBUS_CSPMCR_GROUP5_TA       (0x00)
#define kFLEXBUS_CSPMCR_GROUP5_CS3      (0x01)
#define kFLEXBUS_CSPMCR_GROUP5_BE_7_0   (0x02)

//!< FLEXBUS��ʼ���ṹ��
typedef struct
{
    uint32_t div;             //�����ٶȷ�Ƶ
    uint32_t dataWidth;       //�����������ݿ��
    uint32_t baseAddress;     //�豸����ַ
    uint32_t ADSpaceMask;     //�豸�洢�ռ�
    uint32_t dataAlignMode;   //���ݶ��뷽ʽ
    uint32_t autoAckMode;     //�Զ�Ӧ��ģʽ
    uint32_t ByteEnableMode;  //BEʹ��ģʽ
    uint32_t CSn;             //Ƭѡ�ź�ͨ��
}FLEXBUS_InitTypeDef;

//!< �߼�����ѡ��
typedef struct
{
    bool kFLEXBUS_brustWriteEnable;
    bool kFLEXBUS_brustReadEnable;
    bool kFLEXBUS_EXTS;
    uint32_t kFLEXBUS_ASET:2;
    uint32_t kFLEXBUS_RDAH:2;
    uint32_t kFLEXBUS_WRAH:2;
    uint32_t kFLEXBUS_WS:6;    
}FLEXBUS_AdvancedConfigTypeDef;

//!< API functions
void FLEXBUS_Init(FLEXBUS_InitTypeDef* FLEXBUS_InitStruct);
void FLEXBUS_PortMuxConfig(FLEXBUS_PortMultiplexingSelect_Type group, uint32_t config);
void FLEXBUS_AdvancedConfig(uint32_t CS, FLEXBUS_AdvancedConfigTypeDef* FLEXBUS_AdvancedConfigStruct);

#endif

