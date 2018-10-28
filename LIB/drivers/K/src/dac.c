/**
  ******************************************************************************
  * @file    dac.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  ******************************************************************************
  */
  
 #include "dac.h"
 #include "common.h"
 #if defined(DAC0)
 
#if (!defined(DAC_BASES))

#ifdef DAC0
#define DAC_BASES   {DAC0}
#elif  DAC1
#define DAC_BASES   {DAC0, DAC1}
#endif

#endif
 
 /* global vars */
static DAC_Type * const DAC_InstanceTable[] = DAC_BASES;
static DAC_CallBackType DAC_CallBackTable[ARRAY_SIZE(DAC_InstanceTable)] = {NULL};
static const IRQn_Type DAC_IRQnTable[] = 
{
    DAC0_IRQn,
#ifdef DAC1
    DAC1_IRQn,
#endif
};

/**
 * @brief  ��ʼ��DACģ��
 * @code
 *  DAC_InitTypeDef DAC_InitStruct = {0};
 *  DAC_InitStruct.bufferMode = kDAC_Buffer_Swing; 
 *  DAC_InitStruct.instance = HW_DAC0;
 *  DAC_InitStruct.referenceMode = kDAC_Reference_2; 
 *  DAC_InitStruct.triggerMode = kDAC_TriggerSoftware; 
 *  DAC_Init(&DAC_InitStruct);
 * @endcode
 * @param  DAC_InitTypeDef: 
 *         instance      :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @retval None
 */
void DAC_Init(DAC_InitTypeDef* DAC_InitStruct)
{
    SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;
    /* trigger select */
    switch(DAC_InitStruct->triggerMode)
    {
        case kDAC_TriggerSoftware:
            DAC_InstanceTable[DAC_InitStruct->instance]->C0 |= DAC_C0_DACTRGSEL_MASK;
            break;
        case kDAC_TriggerHardware:
            DAC_InstanceTable[DAC_InitStruct->instance]->C0 &= ~DAC_C0_DACTRGSEL_MASK;
            break;
        default:
            break;
    }
    /* buffer mode */
    switch(DAC_InitStruct->bufferMode)
    {
        case kDAC_Buffer_Disable:
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 &= ~DAC_C1_DACBFEN_MASK;
            break;
        case kDAC_Buffer_Normal:
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 |= DAC_C1_DACBFEN_MASK;
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 &= ~DAC_C1_DACBFMD_MASK;
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 |= DAC_C1_DACBFMD(0);
            break;
        case kDAC_Buffer_Swing:
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 |= DAC_C1_DACBFEN_MASK;
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 &= ~DAC_C1_DACBFMD_MASK;
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 |= DAC_C1_DACBFMD(1);
            break;
        case kDAC_Buffer_OneTimeScan:
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 |= DAC_C1_DACBFEN_MASK;
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 &= ~DAC_C1_DACBFMD_MASK;
            DAC_InstanceTable[DAC_InitStruct->instance]->C1 |= DAC_C1_DACBFMD(2);
            break;
        default:
            break;
    }
    /* reference */
    switch(DAC_InitStruct->referenceMode)
    {
        case kDAC_Reference_1:
            DAC_InstanceTable[DAC_InitStruct->instance]->C0 &= ~DAC_C0_DACRFS_MASK;
            break;
        case kDAC_Reference_2:
            DAC_InstanceTable[DAC_InitStruct->instance]->C0 |= DAC_C0_DACRFS_MASK;
            break;
        default:
            break;
    }
    /* clear flags */
    DAC_InstanceTable[DAC_InitStruct->instance]->SR &= ~DAC_SR_DACBFRPBF_MASK;
    DAC_InstanceTable[DAC_InitStruct->instance]->SR &= ~DAC_SR_DACBFRPTF_MASK;
    DAC_InstanceTable[DAC_InitStruct->instance]->SR &= ~DAC_SR_DACBFWMF_MASK;
    /* enable module */
    DAC_InstanceTable[DAC_InitStruct->instance]->C0 |= DAC_C0_DACEN_MASK;
}
 
/**
 * @brief  ���DACģ��buffer��ָ��
 *
 * @param  instance      :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @note   ����DAC��ǰת������ buffer ָ�� λ��
 * @retval None
 */
uint32_t DAC_GetBufferReadPointer(uint32_t instance)
{
    return (DAC_InstanceTable[instance]->C2 & DAC_C2_DACBFRP_MASK) >> DAC_C2_DACBFRP_SHIFT;
}

/**
 * @brief  ����DACģ��buffer��ָ��
 *
 * @param  instance     :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @param  value        :ָ��λ�� 0-15
 * @retval None
 */
void DAC_SetBufferReadPointer(uint32_t instance, uint32_t value)
{
    DAC_InstanceTable[instance]->C2 &= ~DAC_C2_DACBFRP_MASK;
    DAC_InstanceTable[instance]->C2 |= DAC_C2_DACBFRP(value);
}

/**
 * @brief  ����DACģ���ȡbufferָ��ʱ���������ֵ
 *
 * @param  instance     :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @param  value        :ָ��λ������ 0-15
 * @retval None
 */
void DAC_SetBufferUpperLimit(uint32_t instance, uint32_t value)
{
    DAC_InstanceTable[instance]->C2 &= ~DAC_C2_DACBFUP_MASK;
    DAC_InstanceTable[instance]->C2 |= DAC_C2_DACBFUP(value);
}

/**
 * @brief  ����DACģ���жϺ�DMA
 *
 * @param  instance     :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @param  config       :����ѡ��
 *         @arg kDAC_DMA_Disable                : ��ֹDAC DMA����
 *         @arg kDAC_IT_Disable                 : ��ֹDAC �жϹ���
 *         @arg kDAC_IT_BufferPointer_WaterMark : ����DAC ˮλ�ж�
 *         @arg kDAC_IT_BufferPointer_TopFlag   : ����DAC ReadPointer = 0�ж�
 *         @arg kDAC_IT_BufferPointer_BottomFlag: ��ʼDAC ReadPointer = UpLimit �ж�
 *         @arg kDAC_DMA_BufferPointer_WaterMark: 
 *         @arg kDAC_DMA_BufferPointer_TopFlag  :
 *         @arg kDAC_DMA_BufferPointer_BottomFlag:
 * @retval None
 */
void DAC_ITDMAConfig(uint32_t instance, DAC_ITDMAConfig_Type config)
{
    switch(config)
    {
        case kDAC_IT_Disable:
            NVIC_DisableIRQ(DAC_IRQnTable[instance]);
            break;
        case kDAC_DMA_Disable:
            DAC_InstanceTable[instance]->C1 &= ~DAC_C1_DMAEN_MASK;
            break;
        case kDAC_IT_BufferPointer_WaterMark:
            DAC_InstanceTable[instance]->C0 |= DAC_C0_DACBWIEN_MASK;
            NVIC_EnableIRQ(DAC_IRQnTable[instance]);
            break;
        case kDAC_IT_BufferPointer_TopFlag:
            DAC_InstanceTable[instance]->C0 |= DAC_C0_DACBTIEN_MASK;
            NVIC_EnableIRQ(DAC_IRQnTable[instance]);
            break;
        case kDAC_IT_BufferPointer_BottomFlag:
            DAC_InstanceTable[instance]->C0 |= DAC_C0_DACBBIEN_MASK;
            NVIC_EnableIRQ(DAC_IRQnTable[instance]);
            break;
        case kDAC_DMA_BufferPointer_WaterMark:
            DAC_InstanceTable[instance]->C0 |= DAC_C0_DACBWIEN_MASK;
            DAC_InstanceTable[instance]->C1 |= DAC_C1_DMAEN_MASK;
            break;
        case kDAC_DMA_BufferPointer_TopFlag:
            DAC_InstanceTable[instance]->C0 |= DAC_C0_DACBTIEN_MASK;
            DAC_InstanceTable[instance]->C1 |= DAC_C1_DMAEN_MASK;
            break;
        case kDAC_DMA_BufferPointer_BottomFlag:
            DAC_InstanceTable[instance]->C0 |= DAC_C0_DACBBIEN_MASK;
            DAC_InstanceTable[instance]->C1 |= DAC_C1_DMAEN_MASK;
            break;
        default:
            break;
    }
}

/**
 * @brief  �������DAC��ʼ����
 *
 * @param  instance     :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @note   will only trigger once and will advence the read pointer by one step
 * @retval None
 */
void DAC_SoftwareStartConversion(uint32_t instance)
{
    DAC_InstanceTable[instance]->C0 |= DAC_C0_DACSWTRG_MASK;
}

/**
 * @brief  ����DAC Buffer��ˮλ
 *
 * @param  instance     :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @param  value        :ˮλֵ 0-15
 * @retval None
 */
void DAC_SetWaterMark(uint32_t instance, DAC_WaterMarkMode_Type value)
{
    DAC_InstanceTable[instance]->C1 &= ~DAC_C1_DACBFWM_MASK;
	switch(value)
	{
		case kDAC_WaterMark_1Word:
			DAC_InstanceTable[instance]->C1 |= DAC_C1_DACBFWM(0);
			break;
		case kDAC_WaterMark_2Word:
			DAC_InstanceTable[instance]->C1 |= DAC_C1_DACBFWM(1);
			break;
		case kDAC_WaterMark_3Word:
			DAC_InstanceTable[instance]->C1 |= DAC_C1_DACBFWM(2);
			break;
		case kDAC_WaterMark_4Word:
			DAC_InstanceTable[instance]->C1 |= DAC_C1_DACBFWM(3);
			break;
		default:break;
	}
}

/**
 * @brief  ���DAC ����������
 *
 * @param  instance     :ģ���
 *         @arg HW_DAC0 :оƬ��DAC0 ģ��
 *         @arg HW_DAC1 :оƬ��DAC1 ģ��
 * @param  buf          :����ָ��
 * @param  len          :����
 * @retval None
 */
void DAC_SetBufferValue(uint32_t instance, uint16_t* buf, uint8_t len)
{
	uint8_t i;
	for(i = 0; i < len; i++)
	{
        DAC_InstanceTable[instance]->DAT[i].DATL = (buf[i] & 0x00FF);
        DAC_InstanceTable[instance]->DAT[i].DATH = (buf[i] & 0xFF00)>>8;
	}                       
}

/**
 * @brief  ע���жϻص�����
 * @param  instance: DAC ģ���
 *         @arg HW_DAC0  :DAC0ģ��
 *         @arg HW_DAC1  :DAC1ģ��
 * @param AppCBFun: �ص�����ָ�����
 * @retval None
 * @note ���ڴ˺����ľ���Ӧ�������Ӧ��ʵ��
 */
void DAC_CallbackInstall(uint8_t instance, DAC_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        DAC_CallBackTable[instance] = AppCBFun;
    }
}


void DAC0_IRQHandler(void)
{
    /* clear IT pending bit */
    uint32_t C0 = DAC_InstanceTable[HW_DAC0]->C0;
    if(C0 & DAC_C0_DACBWIEN_MASK)
    {
        DAC_InstanceTable[HW_DAC0]->SR &= ~DAC_SR_DACBFWMF_MASK;
    }
    if(C0 & DAC_C0_DACBBIEN_MASK)
    {
        DAC_InstanceTable[HW_DAC0]->SR &= ~DAC_SR_DACBFRPBF_MASK;
    }
    if(C0 & DAC_C0_DACBTIEN_MASK)
    {
        DAC_InstanceTable[HW_DAC0]->SR &= ~DAC_SR_DACBFRPTF_MASK;
    }
    /* call back */
    if(DAC_CallBackTable[HW_DAC0] != NULL)
    {
        DAC_CallBackTable[HW_DAC0]();
    }  
}


void DAC1_IRQHandler(void)
{
    /* clear IT pending bit */
    uint32_t C0 = DAC_InstanceTable[HW_DAC0]->C0;
    if(C0 & DAC_C0_DACBWIEN_MASK)
    {
        DAC_InstanceTable[HW_DAC0]->SR &= ~DAC_SR_DACBFWMF_MASK;
    }
    if(C0 & DAC_C0_DACBBIEN_MASK)
    {
        DAC_InstanceTable[HW_DAC0]->SR &= ~DAC_SR_DACBFRPBF_MASK;
    }
    if(C0 & DAC_C0_DACBTIEN_MASK)
    {
        DAC_InstanceTable[HW_DAC0]->SR &= ~DAC_SR_DACBFRPTF_MASK;
    }
    /* call back */
    if(DAC_CallBackTable[HW_DAC1] != NULL)
    {
        DAC_CallBackTable[HW_DAC1]();
    }  
}
 
#endif
