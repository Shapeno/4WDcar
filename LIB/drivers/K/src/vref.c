/**
  ******************************************************************************
  * @file    vref.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  ******************************************************************************
  */
  
 #include "vref.h"
 #include "common.h"
 
 /**
 * @brief  ��ʼ��VREF ģ��
 * @param  DAC_InitTypeDef: ��ʼ���ṹ
 * @retval None
 */
void VREF_Init(VREF_InitTypeDef *VREF_InitStruct)
{
    /* open clock gate */
    SIM->SCGC4 |= SIM_SCGC4_VREF_MASK;
    /* enable moudle */
    #ifdef VREF_SC_ICOMPEN_MASK
    VREF->SC = VREF_SC_VREFEN_MASK | VREF_SC_REGEN_MASK | VREF_SC_ICOMPEN_MASK | VREF_SC_MODE_LV(VREF_InitStruct->bufferMode);
    VREF->TRM |= VREF_TRM_CHOPEN_MASK;
    #else
    VREF->SC = VREF_SC_VREFEN_MASK | VREF_SC_REGEN_MASK  | VREF_SC_MODE_LV(VREF_InitStruct->bufferMode);
    #endif
    
}

 /**
 * @brief  ���ٳ�ʼ��VREFģ��
 * @note   VREF����ʼ���� ��Ҫ�������35MS �������ȶ��Ĳο���ѹ���
 * @retval None
 */
void VREF_QuickInit(void)
{
    VREF_InitTypeDef  VREF_InitStruct1;
    VREF_InitStruct1.instance = 0;
    VREF_InitStruct1.bufferMode = kVREF_BufferMode_Lv1;
    VREF_Init(&VREF_InitStruct1);
}

 /**
 * @brief  �ָ�VREF��Ĭ��״̬
 * @retval None
 */
void VREF_DeInit(void)
{
    SIM->SCGC4 &= ~SIM_SCGC4_VREF_MASK;
    VREF->SC = 0;
}

 /**
 * @brief  ����VREFУ׼ֵ
 * @param  У׼ֵ
 * @retval None
 */
void VREF_SetTrimValue(uint32_t val)
{
    VREF->TRM &= ~VREF_TRM_TRIM_MASK;
    VREF->TRM |= VREF_TRM_TRIM(val);
}
 
