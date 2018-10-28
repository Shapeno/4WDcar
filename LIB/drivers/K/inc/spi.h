/**
  ******************************************************************************
  * @file    spi.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬSPIģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */

#ifndef __CH_LIB_SPI_H__
#define __CH_LIB_SPI_H__

#include <stdint.h>
#include <stdbool.h>
  
//!< SPIģ���豸
#define HW_SPI0     (0x00)
#define HW_SPI1     (0x01)
#define HW_SPI2     (0x02)

#define HW_CTAR0    (0x00)  
#define HW_CTAR1    (0x01)  

#define HW_SPI_CS0  (0x00)
#define HW_SPI_CS1  (0x01)
#define HW_SPI_CS2  (0x02)
#define HW_SPI_CS3  (0x03)

//!< SPI֡��ʽѡ��
typedef enum
{
    kSPI_CPOL0_CPHA0,
    kSPI_CPOL0_CPHA1,
    kSPI_CPOL1_CPHA0,
    kSPI_CPOL1_CPHA1
}SPI_FrameFormat_Type;

typedef enum
{
    kSPI_Master,
    kSPI_Slave,
} SPI_Mode_Type;

//!< interrupt and DMA select
typedef enum
{
    kSPI_IT_TCF,            //!< SPI����һ������ж�ʹ��
    kSPI_DMA_TFFF,          //!< transmit FIFO full
    kSPI_DMA_RFDF,          //!< receive FIFO drain
}SPI_ITDMAConfig_Type;

/*!< ÿ֡����λ���� */
typedef enum 
{
    kSPI_PCS_ReturnInactive  = 0,   //!< ������ɺ�CS�źű���Ƭѡ��״̬
    kSPI_PCS_KeepAsserted  = 1,     //!< ������ɺ�CS�źű���δѡ��״̬
}SPI_PCS_Type;

typedef enum
{
    kSPI_MSB,      //!< �ȷ������λ
    kSPI_LSB,      //!< �ȷ������λ
} SPI_BitOlder_Type;

//!< ��ʼ���ṹ
typedef struct
{
    uint32_t                instance;               //!< ģ���
	SPI_Mode_Type           mode;                   //!< ����ģʽ
    uint8_t                 dataSize;               //!< ÿ֡�����ж���λ ͨ��Ϊ8��16
    SPI_BitOlder_Type       bitOrder;               //!< �ȷ���λ�����ȷ���λ
    SPI_FrameFormat_Type    frameFormat;            //!< ����֡��ʽѡ��
    uint32_t                baudrate;               //!< ����
    uint32_t                ctar;                   //!< ֡��ʽ�Ĵ���ѡ�� 
}SPI_InitTypeDef;

//!< ���ٳ�ʼ���ṹ

#define SPI0_SCK_PC05_SOUT_PC06_SIN_PC07   (0xca90U)
#define SPI0_SCK_PD01_SOUT_PD02_SIN_PD03   (0xc298U)
#define SPI1_SCK_PE02_SOUT_PE01_SIN_PE03   (0xc2a1U)
#define SPI0_SCK_PA15_SOUT_PA16_SIN_PA17   (0xde80U)
#define SPI2_SCK_PB21_SOUT_PB22_SIN_PB23   (0xea8aU)
#define SPI2_SCK_PD12_SOUT_PD13_SIN_PD14   (0xd89aU)

//!< Callback Type
typedef void (*SPI_CallBackType)(void);

//!< API functions
void SPI_Init(SPI_InitTypeDef * SPI_InitStruct);
uint16_t SPI_ReadWriteByte(uint32_t instance,uint32_t ctar, uint16_t data, uint16_t CSn, SPI_PCS_Type csState);
void SPI_ITDMAConfig(uint32_t instance, SPI_ITDMAConfig_Type config, bool status);
void SPI_CallbackInstall(uint32_t instance, SPI_CallBackType AppCBFun);
uint32_t SPI_QuickInit(uint32_t MAP, SPI_FrameFormat_Type frameFormat, uint32_t baudrate);
void SPI_CTARConfig(uint32_t instance, uint32_t ctar, SPI_FrameFormat_Type frameFormat, uint8_t dataSize, uint8_t bitOrder, uint32_t baudrate);

/* FIFO functions */
void SPI_EnableTxFIFO(uint32_t instance, bool status);
void SPI_EnableRxFIFO(uint32_t instance, bool status);

#endif

