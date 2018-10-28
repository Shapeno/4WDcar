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
#include "cpuidy.h"
#include "common.h"

/* memory size filed */
#define PFLASH_SIZE_FIELD           ((SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK)>>SIM_FCFG1_PFSIZE_SHIFT)
#define DFLASH_SIZE_FIELD           ((SIM->FCFG1 & SIM_FCFG1_PFSIZE_MASK)>>SIM_FCFG1_PFSIZE_SHIFT)
#define FLEXNVM_SIZE_FIELD          ((SIM->FCFG1 & SIM_FCFG1_NVMSIZE_MASK)>>SIM_FCFG1_NVMSIZE_SHIFT)
#define EEPROM_SIZE_FIELD           ((SIM->FCFG1 & SIM_FCFG1_EESIZE_MASK)>>SIM_FCFG1_EESIZE_SHIFT)  
#define RAM_SIZE_FIELD              ((SIM->SOPT1 & SIM_SOPT1_RAMSIZE_MASK)>>SIM_SOPT1_RAMSIZE_SHIFT)

/* memory size table */
static const uint32_t CPUIDY_PFlashSizeTable[16] = {8, 16, 24, 32, 48, 64, 96 ,128, 192, 256, 0, 512, 0, 0, 0, 0};
static const uint32_t CPUIDY_RAMSizeTable[16] = {4, 8, 12, 16, 24, 32, 48, 64, 96, 128, 0, 0, 0, 0, 0, 0};
static const uint32_t CPUIDY_EEPROMSizeTable[16] = {16*1024, 8*1024, 4*1024, 2*1024, 1*1024, 512, 256, 128, 64, 32, 0, 0, 0, 0, 0};
static const uint32_t CPUIDY_NVMSizeTable[16] = {8, 16, 24, 32, 48, 64, 96, 128, 192, 256 ,0, 512, 0, 0, 0, 0};

/* pin count field */
#define PINCOUNT_FIELD              ((SIM->SDID & SIM_SDID_PINID_MASK)>>SIM_SDID_PINID_SHIFT)
static const uint32_t CPUIDY_PinCountTable[16] = {16, 24, 32, 46, 48, 64, 80, 81, 100, 121, 144, 168, 196, 200, 256};

/* family type field */
#define FAMID_FIELD                 ((SIM->SDID & SIM_SDID_FAMID_MASK)>>SIM_SDID_FAMID_SHIFT) 

/* family type string */
static const char *CPUIDY_FamIDTable[8] = 
{
    "K10",
    "K20",
    "K61 or K30",
    "K40",
    "K60",
    "K70",
    "K50 or K52",
    "K51 or K53",
};

//! @defgroup CHKinetis
//! @{


//! @defgroup CPUIDY
//! @brief CPUIDY API functions
//! @{

/**
 * @brief  ���оƬ��ϵ��ID
 * @code
 *      // ��ӡоƬ�ͺ�
 *      printf("Family Type:%s\r\n", CPUIDY_GetFamID());
 * @endcode
 * @param  None
 * @retval ID�ַ���ָ��
 */
char *CPUIDY_GetFamID(void)
{
    return (char*)CPUIDY_FamIDTable[FAMID_FIELD];
}

/**
 * @brief  ���оƬ��������
 * @code
 *      // ��ȡоƬ��������
 *      uint32_t PinCnt;
 *      PinCnt = CPUIDY_GetPinCount();
 *      //��оƬ����������ʾ����
 *      printf("Pin Cnt:%d\r\n", PinCnt);
 * @endcode
 * @retval ��������
 */
uint32_t CPUIDY_GetPinCount(void)
{
    return CPUIDY_PinCountTable[PINCOUNT_FIELD];
}

/**
 * @brief  ���оƬROM/RAM ��С
 * @code
 *      // ��ȡоƬPflash�Ĵ�С����ʾ����
 *      uint32_t PFlashSize;
 *      PFlashSize = CPUIDY_GetMemSize(kPFlashSizeInKB);
 *      printf("PFlash Size:%dKB\r\n", PFlashSize);
 * @endcode
 * @param  memSizeName: �洢������
 *         @arg kPFlashSizeInKB  :���Flash�ĳߴ�
 *         @arg kDFlashSizeInKB  :����Flash�ĳߴ�
 *         @arg kFlexNVMSizeInKB :FlexNVM�ĳߴ�
 *         @arg kEEPORMSizeInByte:EEPOROM�ĳߴ�
 *         @arg kRAMSizeInKB     :оƬRAM�ĳߴ�
 * @param  memSizeInKB   :��ô洢����С���ݵ�ָ�� ��λKB
 * @retval None
 */
uint32_t CPUIDY_GetMemSize(CPUIDY_MemSize_Type memSizeName)
{
    uint32_t ret_value;
    switch (memSizeName)
		{
        case kPFlashSizeInKB:
            ret_value = CPUIDY_PFlashSizeTable[PFLASH_SIZE_FIELD];
            break;
        case kDFlashSizeInKB:
            ret_value = 0;
            break;
        case kFlexNVMSizeInKB:
            ret_value = CPUIDY_NVMSizeTable[FLEXNVM_SIZE_FIELD];
            break;
        case kEEPROMSizeInByte:
            ret_value = CPUIDY_EEPROMSizeTable[EEPROM_SIZE_FIELD];
            break;
        case kRAMSizeInKB:
            ret_value = CPUIDY_RAMSizeTable[RAM_SIZE_FIELD];
            break;
        default:
            ret_value = 0;
            break;
		}
    return ret_value;
}

/**
 * @brief  ���оƬUID��Ϣ(ȫ��Ψһʶ����)
 * @code
 *      // ��ȡоƬ��UID����ʾ����
 *      uint32_t UID[4];
 *      CPUIDY_GetUID(UID);
 *      for(i = 0; i < 4; i++)
 *      {
 *          printf("UID[i]:0x%d", UID[i]);
 *      }
 * @endcode
 * @param  UIDArray: UID���ݵ��׵�ַ
 * @retval None
 */
void CPUIDY_GetUID(uint32_t * UIDArray)
{
    UIDArray[0] = SIM->UIDL;
    UIDArray[1] = SIM->UIDML;
    UIDArray[2] = SIM->UIDMH;
    UIDArray[3] = SIM->UIDH;
}

//! @}

//! @}

