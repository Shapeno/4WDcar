/**
  ******************************************************************************
  * @file    dma.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬDMAģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */

#include "dma.h"
#include "common.h"

#if !defined(DMAMUX_BASES)
#define DMAMUX_BASES {DMAMUX};

#endif

/* gloabl const table defination */
static DMAMUX_Type * const DMAMUX_InstanceTable[] = DMAMUX_BASES;
static DMA_CallBackType DMA_CallBackTable[16] = {NULL};
static uint32_t DMAChlMAP;

uint32_t _DMA_ChlAlloc(void);
void DMA_ChlFree(uint32_t chl);

/* DMA�ж�������� */
/* Ĭ�ϵ�һ��DMA�ж��������Ϊ0 */
static const IRQn_Type DMA_IRQnTable[] = 
{
    (IRQn_Type)(0 + 0),
    (IRQn_Type)(0 + 1),
    (IRQn_Type)(0 + 2),
    (IRQn_Type)(0 + 3),
    (IRQn_Type)(0 + 4),
    (IRQn_Type)(0 + 5),
    (IRQn_Type)(0 + 6),
    (IRQn_Type)(0 + 7),
    (IRQn_Type)(0 + 8),
    (IRQn_Type)(0 + 9),
    (IRQn_Type)(0 + 10),
    (IRQn_Type)(0 + 11),
    (IRQn_Type)(0 + 12),
    (IRQn_Type)(0 + 13),
    (IRQn_Type)(0 + 14),
    (IRQn_Type)(0 + 15),
};


/**
 * @brief  ��ʼ��DMAģ��
 * @param  DMA_InitStruct :DMA��ʼ�����ýṹ�壬���dma.h
 * @retval ���䵽��DMA ͨ��
 */
uint32_t DMA_Init(DMA_InitTypeDef *DMA_InitStruct)
{
    uint8_t chl;
    
	/* enable DMA and DMAMUX clock */
#if defined(DMAMUX0)  
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;
#endif
#if  defined(DMAMUX1)
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX1_MASK;
#endif
#if  defined(DMAMUX)
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
#endif
	
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
    
    chl = DMA_InitStruct->chl;

    /* disable chl first */
    DMA0->ERQ &= ~(1<<(chl));
    /* dma chl source config */
    DMAMUX_InstanceTable[0]->CHCFG[chl] = DMAMUX_CHCFG_SOURCE(DMA_InitStruct->chlTriggerSource);
    /* trigger mode */
    switch(DMA_InitStruct->triggerSourceMode)
    {
        case kDMA_TriggerSource_Normal:
            DMAMUX_InstanceTable[0]->CHCFG[chl] &= ~DMAMUX_CHCFG_TRIG_MASK;
            break;
        case kDMA_TriggerSource_Periodic:
            DMAMUX_InstanceTable[0]->CHCFG[chl] |= DMAMUX_CHCFG_TRIG_MASK;
            break;
        default:
            break;
    }
    /* clear some register */
    DMA0->TCD[chl].ATTR  = 0;
    DMA0->TCD[chl].CSR   = 0;
    /* minor loop cnt */
    DMA0->TCD[chl].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(DMA_InitStruct->minorLoopByteCnt);
    /* major loop cnt */
	DMA0->TCD[chl].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(DMA_InitStruct->majorLoopCnt);
	DMA0->TCD[chl].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(DMA_InitStruct->majorLoopCnt);
    /* source config */
    DMA0->TCD[chl].SADDR = DMA_InitStruct->sAddr;
    DMA0->TCD[chl].SOFF = DMA_InitStruct->sAddrOffset;
    DMA0->TCD[chl].ATTR |= DMA_ATTR_SSIZE(DMA_InitStruct->sDataWidth);
    DMA0->TCD[chl].ATTR |= DMA_ATTR_SMOD(DMA_InitStruct->sMod);
    DMA0->TCD[chl].SLAST = DMA_SLAST_SLAST(DMA_InitStruct->sLastAddrAdj);
    /* destation config */
    DMA0->TCD[chl].DADDR = DMA_InitStruct->dAddr;
    DMA0->TCD[chl].DOFF = DMA_InitStruct->dAddrOffset;
    DMA0->TCD[chl].ATTR |= DMA_ATTR_DSIZE(DMA_InitStruct->dDataWidth);
    DMA0->TCD[chl].ATTR |= DMA_ATTR_DMOD(DMA_InitStruct->dMod);
    DMA0->TCD[chl].DLAST_SGA = DMA_DLAST_SGA_DLASTSGA(DMA_InitStruct->dLastAddrAdj);
    /* auto close enable(disable req on major loop complete)*/
    DMA0->TCD[chl].CSR |= DMA_CSR_DREQ_MASK;
	/* enable DMAMUX */
	DMAMUX_InstanceTable[0]->CHCFG[chl] |= DMAMUX_CHCFG_ENBL_MASK;
    
    return chl;
}

uint32_t DMA_ChlAlloc(void)
{
    uint32_t i;
    uint32_t MaxDMAChl;
    
    /* get max DMA chl on this device */
    MaxDMAChl = (ARRAY_SIZE(DMAMUX_InstanceTable[0]->CHCFG)>32)?(ARRAY_SIZE(DMAMUX_InstanceTable[0]->CHCFG)):(32);
    
    /* alloc a channel */
    for(i=0;i<MaxDMAChl;i++)
    {
        if(!(DMAChlMAP & (1<< i)))
        {
            DMAChlMAP |= (1<<i);
            return i;
        }
    }
    return 0;
} 

void DMA_ChlFree(uint32_t chl)
{
    DMAChlMAP &= ~(1<<chl);
}

/**
 * @brief  ��� DMA MajorLoopCount ����ֵ
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval ����ֵ
 */
uint32_t DMA_GetMajorLoopCount(uint8_t chl)
{
    return (DMA0->TCD[chl].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK) >> DMA_CITER_ELINKNO_CITER_SHIFT;
}

/**
 * @brief  ���� DMA MajorLoopCount ����ֵ
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval None
 * @note   ��ֵ���ܳ��� DMA_CITER_ELINKNO_CITER_MASK
 */
void DMA_SetMajorLoopCounter(uint8_t chl, uint32_t val)
{
    DMA0->TCD[chl].CITER_ELINKNO &= ~DMA_CITER_ELINKNO_CITER_MASK;
    DMA0->TCD[chl].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(val);
}


/**
 * @brief  ʹ��ͨ����Ӧ����
 * @code
 *     //����DMA ��0ͨ���������ݴ���
 *     DMA_EnableRequest(HW_DMA_CH0);
 * @endcode
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval None
 */
void DMA_EnableRequest(uint8_t chl)
{
    DMA0->SERQ = DMA_SERQ_SERQ(chl);
}

/**
 * @brief  ��ֹͨ����Ӧ����
 * @code
 *     //����DMA ��0ͨ���������ݴ���
 *     DMA_EnableRequest(HW_DMA_CH0);
 * @endcode
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval None
 */
void DMA_DisableRequest(uint8_t chl)
{
    DMA0->CERQ = DMA_CERQ_CERQ(chl);
}

/**
 * @brief  ��Majloop ������  �Ƿ��Զ��ر�Request
 */
void DMA_EnableAutoDisableRequest(uint8_t chl , bool flag)
{
    if(flag)
    {
        DMA0->TCD[chl].CSR |= DMA_CSR_DREQ_MASK;
    }
    else
    {
        DMA0->TCD[chl].CSR &= ~DMA_CSR_DREQ_MASK;  
    }
}

/**
 * @brief  ʹ��Major LoopLink ����
 * @note   ��һ��ͨ������MajorLoopLink�� �Զ���ʼ��һ��ͨ���Ĵ���
 * @param  chl: DMAͨ����
 * @param  linkChl: ��Ҫ���ӵ�ͨ����
 * @param  flag ʹ�ܻ��߹ر�
 * @retval None
 */
void DMA_EnableMajorLink(uint8_t chl , uint8_t linkChl, bool flag)
{
    if(flag)
    {
        /* enable major loop link */
        DMA0->TCD[chl].CSR |= DMA_CSR_MAJORELINK_MASK;
        /* set chl */
        DMA0->TCD[chl].CSR &= ~DMA_CSR_MAJORLINKCH_MASK;
        DMA0->TCD[chl].CSR |= DMA_CSR_MAJORLINKCH(linkChl);
    }
    else
    {
        DMA0->TCD[chl].CSR &= ~DMA_CSR_MAJORELINK_MASK;
    }
}

/**
 * @brief  ����DMA��������ж�
 * @code
 *     //����DMA ��0ͨ���Ĵ�������жϹ���
 *     DMA_StartTransfer(HW_DMA_CH0);
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param config: ����ģʽ
 *         @arg kDMA_IT_Half_Disable ��ֹDMA����һ���жϴ���
 *         @arg kDMA_IT_Major_Disable ��ֹDMA��������жϴ���
 *         @arg kDMA_IT_Half ����DMA����һ���жϴ���
 *         @arg kDMA_IT_Major ����DMA��������жϴ���
 * @retval None
 */
void DMA_ITConfig(uint8_t chl, DMA_ITConfig_Type config, bool status)
{
    if(status)
    {
        NVIC_EnableIRQ(DMA_IRQnTable[chl]);
    }
    switch(config)
    {
        case kDMA_IT_Half:
            (status)?
            (DMA0->TCD[chl].CSR |= DMA_CSR_INTHALF_MASK):
            (DMA0->TCD[chl].CSR &= ~DMA_CSR_INTHALF_MASK);
            break;
        case kDMA_IT_Major:
            (status)?
            (DMA0->TCD[chl].CSR |= DMA_CSR_INTMAJOR_MASK):
            (DMA0->TCD[chl].CSR &= ~DMA_CSR_INTMAJOR_MASK);
            break; 
        default:
            break;
    }
}

/**
 * @brief  ע���жϻص�����
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param AppCBFun: �ص�����ָ��
 * @retval None
 */
void DMA_CallbackInstall(uint8_t chl, DMA_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        DMA_CallBackTable[chl] = AppCBFun;
    }
}

/**
 * @brief  ���DMA�����Ƿ����
 * @code
 *     //���DMA��0ͨ���Ƿ�������ݴ���
 *     status = IsMajorLoopComplete(HW_DMA_CH0);
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @retval 0:���ݴ������ 1:���ݴ���δ���
 */

uint8_t DMA_IsMajorLoopComplete(uint8_t chl)
{
    if(DMA0->ERQ & (1 << chl))
    {
        if(DMA0->TCD[chl].CSR & DMA_CSR_DONE_MASK)
        {
            /* clear this bit */
            DMA0->CDNE = DMA_CDNE_CDNE(chl);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    /* this chl is idle, so return 0 and clear DONE bit anyway; */
    DMA0->CDNE = DMA_CDNE_CDNE(chl);
    return 0;
}

/**
 * @brief  ����DMAģ��ָ��ͨ����Ŀ���ַ
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param address: 32λ��Ŀ�����ݵ�ַ
 * @retval None
 */
void DMA_SetDestAddress(uint8_t ch, uint32_t address)
{
    DMA0->TCD[ch].DADDR = address;
}

uint32_t DMA_GetDestAddress(uint8_t ch)
{
    return DMA0->TCD[ch].DADDR;
}

/**
 * @brief  ����DMAģ��ָ��ͨ����Դ��ַ
 * @param  chl: DMAͨ����
 *         @arg HW_DMA_CH0
 *         @arg HW_DMA_CH1
 *         @arg HW_DMA_CH2
 *         @arg HW_DMA_CH3
 * @param address: 32λ��Դ���ݵ�ַ
 * @retval None
 */
void DMA_SetSourceAddress(uint8_t ch, uint32_t address)
{
    DMA0->TCD[ch].SADDR = address;
}

uint32_t DMA_GetSourceAddress(uint8_t ch)
{
    return DMA0->TCD[ch].SADDR;
}

/**
 * @brief  ȡ��DMAģ��ָ��ͨ�������ݴ���
 * @param  None
 * @retval None
 */
void DMA_CancelTransfer(void)
{
    DMA0->CR |= DMA_CR_CX_MASK;
}

/**
 * @brief  �жϴ��������
 * @param  DMA0_IRQHandler :оƬ��DMA0ͨ���жϺ������
 *               ...       :         ....
 *         DMA15_IRQHandler:оƬ��DMA15ͨ���жϺ������
 * @note �����ڲ������ж��¼�����
 */
void DMA0_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(0);
    if(DMA_CallBackTable[0]) DMA_CallBackTable[0]();
}
void DMA1_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(1);
    if(DMA_CallBackTable[1]) DMA_CallBackTable[1]();
}
void DMA2_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(2);
    if(DMA_CallBackTable[2]) DMA_CallBackTable[2]();
}
void DMA3_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(3);
    if(DMA_CallBackTable[3]) DMA_CallBackTable[3]();
}
void DMA4_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(4);
    if(DMA_CallBackTable[4]) DMA_CallBackTable[4]();
}
void DMA5_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(5);
    if(DMA_CallBackTable[5]) DMA_CallBackTable[5]();
}
void DMA6_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(6);
    if(DMA_CallBackTable[6]) DMA_CallBackTable[6]();
}
void DMA7_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(7);
    if(DMA_CallBackTable[7]) DMA_CallBackTable[7]();
}
void DMA8_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(8);
    if(DMA_CallBackTable[8]) DMA_CallBackTable[8]();
}
void DMA9_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(9);
    if(DMA_CallBackTable[9]) DMA_CallBackTable[9]();
}
void DMA10_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(10);
    if(DMA_CallBackTable[10]) DMA_CallBackTable[10]();
}
void DMA11_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(11);
    if(DMA_CallBackTable[11]) DMA_CallBackTable[11]();
}
void DMA12_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(12);
    if(DMA_CallBackTable[12]) DMA_CallBackTable[12]();
}
void DMA13_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(13);
    if(DMA_CallBackTable[13]) DMA_CallBackTable[13]();
}
void DMA14_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(14);
    if(DMA_CallBackTable[14]) DMA_CallBackTable[14]();
}
void DMA15_IRQHandler(void)
{
    DMA0->CINT = DMA_CINT_CINT(15);
    if(DMA_CallBackTable[15]) DMA_CallBackTable[15]();
}





