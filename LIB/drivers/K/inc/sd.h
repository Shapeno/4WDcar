/**
  ******************************************************************************
  * @file    sd.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬSDģ��ĵײ㹦�ܺ���
              ֻ֧��SD��SDHC�� ��֧��MMC��
  ******************************************************************************
  */
  
#ifndef __CH_LIB_SD_H__
#define __CH_LIB_SD_H__


#include <stdint.h>
#include <common.h>


//SD�����Ͷ���
#define SD_CARD_TYPE_NONE      (0)
#define SD_CARD_TYPE_UNKNOW    (1)
#define SD_CARD_TYPE_SD        (2)
#define SD_CARD_TYPE_SDHC      (3)
#define SD_CARD_TYPE_SDIO      (4)
#define SD_CARD_TYPE_SDCOMBO   (5)
#define SD_CARD_TYPE_SDHCCOMBO (6)
#define SD_CARD_TYPE_MMC       (7)
#define SD_CARD_CEATA          (8)   

//SD��ʼ���ṹ��
typedef struct 
{
    uint32_t baudrate; //ͨ���ٶ�
}SD_InitTypeDef;

//SD������ṹ
typedef struct 
{
  uint32_t COMMAND;           //�����
  uint32_t ARGUMENT;          //����
  uint32_t BLOCKS;            //����
  uint32_t BLOCKSIZE;         //���С
  uint32_t RESPONSE[4];       //�����Ӧ��Ϣ
}SD_CommandTypeDef;

//SD����ͨѶ�������
#define ESDHC_OK                             (0)
#define ESDHC_ERROR_INIT_FAILED              (1)
#define ESDHC_ERROR_COMMAND_FAILED           (2)
#define ESDHC_ERROR_COMMAND_TIMEOUT          (3)
#define ESDHC_ERROR_DATA_TRANSFER            (4)
#define ESDHC_ERROR_INVALID_BUS_WIDTH        (5)



//!< API funtctions
uint8_t SD_Init(SD_InitTypeDef* SD_InitStruct);
uint32_t SD_GetSizeInMB(void);
uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buf);
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buf);
uint8_t SD_ReadMultiBlock(uint32_t sector, uint8_t *buf, uint16_t blockCnt);
uint8_t SD_WriteMultiBlock(uint32_t sector,const uint8_t *buf, uint16_t blockCnt);
uint32_t SD_QuickInit(uint32_t baudrate);
uint8_t SD_InitCard(void);


#endif



