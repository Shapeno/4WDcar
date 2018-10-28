/**
  ******************************************************************************
  * @file    nfc.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.5.5
  * @brief   蓝宙电子官网：www.landzo.com  蓝宙官方淘宝商城：http://landzo.taobao.com
  ******************************************************************************
  */
  
#ifndef __CH_LIB_NFC_H__
#define __CH_LIB_NFC_H__

#include <stdint.h>

typedef enum
{
    kNFC_Width8Bit,
    kNFC_Width16Bit,
}NFC_Width_Type;


/* NFC init struct */
typedef struct
{
    uint32_t            cs;
    uint32_t            sectorSize; /* how many byte in a page, ex:2048+64 */
    NFC_Width_Type      width;

}NFC_InitTypeDef;

//!< Control API
void NFC_Init(NFC_InitTypeDef *NFC_InitStruct);
void NFC_SendResetCmd(void);
void NFC_ReadFlashID(uint8_t nfcBufNum, uint32_t* id0, uint32_t* id1);
uint32_t NFC_GetBufAddr(uint8_t nfcbufNum);

//!< RAW API
void NFC_BlockErase(uint32_t cs, uint32_t row_addr);
void NFC_PageProgram(uint8_t cs, uint8_t nfcbufNum, uint32_t row_addr, uint16_t col_addr);
void NFC_PageRead(uint8_t cs, uint8_t nfcBufNum, uint32_t row_addr, uint16_t col_addr);


#endif

