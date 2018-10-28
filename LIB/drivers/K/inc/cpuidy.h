/**
  ******************************************************************************
  * @file    clock.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
	* @note    ���ļ�Ϊ�ڲ��ļ������ڻ�ȡоƬ�ĳ�����Ϣ���ٲ����û�ʹ��
  ******************************************************************************
  */
#ifndef __CH_LIB_CPUIDY_H__
#define __CH_LIB_CPUIDY_H__

#include <stdint.h>

/*оƬ��Ϣ�ṹ�嶨��*/
typedef enum
{
    kPFlashSizeInKB,   //оƬ�ڲ���PFlash��С                
    kDFlashSizeInKB,   //оƬ�ڲ���DFlash��С 
    kFlexNVMSizeInKB,  //оƬ�ڲ���FlexNVMFlash��С 
    kEEPROMSizeInByte, //оƬ�ڲ���EEPROM��С 
    kRAMSizeInKB,      //оƬ�ڲ���RAM��С 
    kMemNameCount,
}CPUIDY_MemSize_Type;


//!< API functions
char *CPUIDY_GetFamID(void);
uint32_t CPUIDY_GetPinCount(void);
uint32_t CPUIDY_GetMemSize(CPUIDY_MemSize_Type memSizeName);
void CPUIDY_GetUID(uint32_t * UIDArray);


#endif

