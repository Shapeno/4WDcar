/**
  ******************************************************************************
  * @file    adc.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief  ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬADCģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */
  
#include "adc.h"
#include "gpio.h"

/* leagacy support for Kineis Z Version(inital version) */
#if (!defined(ADC_BASES))
#ifdef ADC1
    #define ADC_BASES {ADC0, ADC1}
#elif ADC2
    #define ADC_BASES {ADC0, ADC1,ADC2}
#else
    #define ADC_BASES {ADC0}
#endif


#endif
/* global vars */
static ADC_Type * const ADC_InstanceTable[] = ADC_BASES;
static ADC_CallBackType ADC_CallBackTable[ARRAY_SIZE(ADC_InstanceTable)] = {NULL};

static const struct reg_ops SIM_ADCClockGateTable[] =
{
    {(void*)&(SIM->SCGC6), SIM_SCGC6_ADC0_MASK},
#ifdef ADC1
    {(void*)&(SIM->SCGC3), SIM_SCGC3_ADC1_MASK},
#endif
#ifdef ADC2
    {(void*)&(SIM->SCGC3), SIM_SCGC6_ADC2_MASK},
#endif
};
static const IRQn_Type ADC_IRQnTable[] = 
{
    ADC0_IRQn,
#ifdef ADC1
    ADC1_IRQn,
#endif
#ifdef ADC2
    ADC2_IRQn,
#endif
};



//! @defgroup CHKinetis
//! @{

//! @defgroup ADC(ģ��ת��ģ��)
//! @brief ADC API functions
//! @{

/**
 * @brief  �ж�ADת���Ƿ����
 * @code
 *      //�鿴ADC0 ģ���Aͨ����ת���Ƿ����
 *      ADC_IsConversionCompleted(HW_ADC0, kADC_MuxA); 
 * @endcode         
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0  :ADC0ģ��
 *         @arg HW_ADC1  :ADC1ģ��
 *         @arg HW_ADC2  :ADC2ģ��
 * @param  mux: ADC ת����ͨ�� ���� ѡ��
 *         @arg kADC_MuxA   :Aͨ��ģʽ
 *         @arg kADC_MuxB   :Bͨ��ģʽ
 * @retval 
 *         @arg 0:ת�����
 *         @arg 1:ת��δ���
 */
uint8_t ADC_IsConversionCompleted(uint32_t instance, uint32_t mux)
{
    if(ADC_InstanceTable[instance]->SC1[mux] & ADC_SC1_COCO_MASK)
    {
        return 0;
    }
    return 1;
    //return (!((ADC_InstanceTable[instance]->SC1[mux]) & ADC_SC1_COCO_MASK) >> ADC_SC1_COCO_SHIFT);  
}

/**
 * @brief  AD�ɼ�У׼�������ڲ�����
 * @code
 *      //�鿴ADC0 ģ���Aͨ����ת���Ƿ����
 *      ADC_IsConversionCompleted(HW_ADC0, kADC_MuxA); 
 * @endcode         
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0  :ADC0ģ��
 *         @arg HW_ADC1  :ADC1ģ��
 *         @arg HW_ADC2  :ADC2ģ��
 * @param  mux: ADC ת����ͨ�� ���� ѡ��
 *         @arg kADC_MuxA   :Aͨ��ģʽ
 *         @arg kADC_MuxB   :Bͨ��ģʽ
 * @retval 
 *         @arg 0:ת�����
 *         @arg 1:ת��δ���
 */
int32_t ADC_Calibration(uint32_t instance)
{
    uint32_t i;
    uint32_t PG, MG;
    
    // disable clock 
    
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_CALF_MASK; /* Clear the calibration's flag */
    ADC_InstanceTable[instance]->SC3 |= ADC_SC3_CAL_MASK;  /* Enable the calibration */

    for(i=0;i<100000;i++);
    //while(ADC_IsConversionCompleted(instance, 0)) {};      /* Wait conversion is competed */
    if(ADC_InstanceTable[instance]->SC3 & ADC_SC3_CALF_MASK)
    {
        /* cal failed */
        LIB_TRACE("ADC calibration failed\r\n");
        return 1;
    }
    /* Calculate plus-side calibration */
    PG = 0U;
    PG += ADC_InstanceTable[instance]->CLP0;
    PG += ADC_InstanceTable[instance]->CLP1;
    PG += ADC_InstanceTable[instance]->CLP2;
    PG += ADC_InstanceTable[instance]->CLP3;
    PG += ADC_InstanceTable[instance]->CLP4;
    PG += ADC_InstanceTable[instance]->CLPS;
    PG = 0x8000U | (PG>>1U);
    
    /* Calculate minus-side calibration */
    MG = 0U;
    MG += ADC_InstanceTable[instance]->CLM0;
    MG += ADC_InstanceTable[instance]->CLM1;
    MG += ADC_InstanceTable[instance]->CLM2;
    MG += ADC_InstanceTable[instance]->CLM3;
    MG += ADC_InstanceTable[instance]->CLM4;
    MG += ADC_InstanceTable[instance]->CLMS;
    MG = 0x8000U | (MG>>1U);
    ADC_InstanceTable[instance]->SC3 &= ~ADC_SC3_CAL_MASK;  /* Disable the calibration */
    ADC_InstanceTable[instance]->PG = PG;
    ADC_InstanceTable[instance]->MG = MG;
    return 0;
}

/**
 * @brief  ADCģ�鹤����ʼ������
 * @code
 *   //ʹ��adc0ģ���1ͨ�� ����ģʽ 8λ���� �������
 *   ADC_InitTypeDef ADC_InitStruct1;  //����һ���ṹ��
 *   ADC_InitStruct1.chl = 1;  //1ͨ��
 *   ADC_InitStruct1.clockDiv = kADC_ClockDiv8; //ADCת��ʱ��Ϊ����ʱ��(Ĭ��BusClock) ��8��Ƶ����ת���ٶ����
 *   ADC_InitStruct1.instance = HW_ADC0;        //ѡ��ADC0ģ��
 *   ADC_InitStruct1.resolutionMode = kADC_SingleDiff8or9; //����ģʽ��8λ���� ���ģʽ��9λ����
 *   ADC_InitStruct1.SingleOrDifferential = kADC_Single;   //ѡ�񵥶�ģʽ
 *   ADC_InitStruct1.triggerMode = kADC_TriggleSoftware;   //����Ϊ�������
 *   ADC_InitStruct1.vref = kADC_VoltageVREF;              //ʹ���ⲿVERFH VREFL ��Ϊģ���ѹ�ο�
 *   //��ʼ��ADCģ��
 *   ADC_Init(&ADC_InitStruct1);
 * @endcode
 * @param  ADC_InitStruct: ADC��ʼ���ṹ�壬�������ע��
 * @retval None
 */
void ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
    /* enable clock gate */
    *(uint32_t*)SIM_ADCClockGateTable[ADC_InitStruct->instance].addr |= SIM_ADCClockGateTable[ADC_InitStruct->instance].mask;
    
    /* do calibration */
    //ADC_Calibration(ADC_InitStruct->instance);
    
    /* select clock */
    //ADC_InstanceTable[ADC_InitStruct->instance]->CFG2 |= ADC_CFG2_ADACKEN_MASK;
    //ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
    //ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |= ADC_CFG1_ADICLK(3);
    
	/* set clock configuration */
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 &= ~ADC_CFG1_ADIV_MASK;
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |=  ADC_CFG1_ADIV(ADC_InitStruct->clockDiv); 
    
    /* long sample time */
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |=  ADC_CFG1_ADLPC_MASK;
    
    /* voltage reference */
    ADC_InstanceTable[ADC_InitStruct->instance]->SC2 &= ~ADC_SC2_REFSEL_MASK;
    ADC_InstanceTable[ADC_InitStruct->instance]->SC2 |= ADC_SC2_REFSEL(ADC_InitStruct->vref);
    
    /* resolutionMode */
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 &= ~(ADC_CFG1_MODE_MASK); 
	ADC_InstanceTable[ADC_InitStruct->instance]->CFG1 |= ADC_CFG1_MODE(ADC_InitStruct->resolutionMode);
    
    /* trigger mode */
    (kADC_TriggerHardware == ADC_InitStruct->triggerMode)?(ADC_InstanceTable[ADC_InitStruct->instance]->SC2 |=  ADC_SC2_ADTRG_MASK):(ADC_InstanceTable[ADC_InitStruct->instance]->SC2 &=  ADC_SC2_ADTRG_MASK);
    
    /* if continues conversion */
    (kADC_ContinueConversionEnable == ADC_InitStruct->continueMode)?(ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_ADCO_MASK):(ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_ADCO_MASK);
    
    /* if hardware average enabled */
    switch(ADC_InitStruct->hardwareAveMode)
    {
        case kADC_HardwareAverageDisable:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGE_MASK;
            break;
        case kADC_HardwareAverage_4:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGE_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(0);
            break;
        case kADC_HardwareAverage_8:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGE_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(1);
            break;
        case kADC_HardwareAverage_16:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGE_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(2);
            break;
        case kADC_HardwareAverage_32:
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGE_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 &= ~ADC_SC3_AVGS_MASK;
            ADC_InstanceTable[ADC_InitStruct->instance]->SC3 |= ADC_SC3_AVGS(3);
            break;
        default:
            break;
    }
    
    ADC_Calibration(ADC_InitStruct->instance);
}

void ADC_EnableHardwareTrigger(uint32_t instance, bool status)
{
    (status)?
    (ADC_InstanceTable[instance]->SC2 |=  ADC_SC2_ADTRG_MASK):
    (ADC_InstanceTable[instance]->SC2 &=  ADC_SC2_ADTRG_MASK);
}

/**
 * @brief  �������AD��ʼ������
 * @code
 *    //��ʼ�� ADC0 ͨ��20 ����DM1 ���� ���� 12λ
 *    ADC_QuickInit(ADC0_SE20_DM1, kADC_SingleDiff12or13);
 *    //��ȡADת�����
 *    value = ADC_QuickReadValue(ADC0_SE20_DM1);
 * @endcode
 * @param  MAP: ���ٳ�ʼ���꣬���ADC.H�ļ�
 * @param  resolutionMode: ת���ֱ�������
 *         @arg kADC_SingleDiff8or9   :ת������Ϊ8/9λ
 *         @arg kADC_SingleDiff10or11 :ת������Ϊ10/11λ
 *         @arg kADC_SingleDiff12or13 :ת������Ϊ12/13λ
 *         @arg kADC_SingleDIff16     :ת������Ϊ16λ
 * @retval ADCģ���
 */
uint8_t ADC_QuickInit(uint32_t MAP, ADC_ResolutionMode_Type resolutionMode)
{
    uint8_t i;
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    ADC_InitTypeDef AD_InitStruct1;
    AD_InitStruct1.instance = pq->ip_instance;
    AD_InitStruct1.clockDiv = kADC_ClockDiv8;
    AD_InitStruct1.resolutionMode = resolutionMode;
    AD_InitStruct1.triggerMode = kADC_TriggerSoftware;
    AD_InitStruct1.singleOrDiffMode = kADC_Single;
    AD_InitStruct1.continueMode = kADC_ContinueConversionDisable;
    AD_InitStruct1.hardwareAveMode = kADC_HardwareAverageDisable;
    AD_InitStruct1.vref = kADC_VoltageVREF;
    
    /* init pinmux */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux);
        PORT_PinPullConfig(pq->io_instance, pq->io_base + i, kPullDisabled); 
    }
    /* init moudle */
    ADC_Init(&AD_InitStruct1);
    
    /* init adc chlmux */
    ADC_ChlMuxConfig(pq->ip_instance, pq->reserved);
    
    return pq->ip_instance;
}
  
/**
 * @brief  ADC��ʼһ��ת�� 
 * @note   �������� ������ʽ ���ȴ�ת�����
 * @code
 *    //���� ADC0 ͨ��20 ��Aģʽ������ת��
 *    ADC_StartConversion(HW_ADC0, 20, kADC_MuxA);
 * @endcode
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0  :ADC0ģ��
 *         @arg HW_ADC1  :ADC1ģ��
 *         @arg HW_ADC2  :ADC2ģ��
 * @param  chl: ADC ͨ����
 * @param  mux: ADC ת����ͨ�� ���� ѡ��
 *         @arg kADC_MuxA   :Aת��������
 *         @arg kADC_MuxB   :Bת��������
 * @retval None
 */
void ADC_StartConversion(uint32_t instance, uint32_t chl, uint32_t mux)
{
    ADC_InstanceTable[instance]->SC1[mux] &= ~(ADC_SC1_ADCH_MASK);
    ADC_InstanceTable[instance]->SC1[mux] |= ADC_SC1_ADCH(chl);
}

void ADC_ChlMuxConfig(uint32_t instance, uint32_t mux)
{
    if(kADC_ChlMuxA ==  mux)
    {
        ADC_InstanceTable[instance]->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
    }
    if(kADC_ChlMuxB ==  mux)
    {
        ADC_InstanceTable[instance]->CFG2 |= ADC_CFG2_MUXSEL_MASK; 
    }
}

/**
 * @brief  ��ȡADCת������
 * @note   �������� ������ʽ 
 * @code
 *    //��ȡ ADC0ģ���µ���Aģʽ������ת�����
 *    uint32_t data;   //�洢ת�����
 *    data = ADC_ReadValue(HW_ADC0, kADC_MuxA);
 * @endcode
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0  :ADC0ģ��
 *         @arg HW_ADC1  :ADC1ģ��
 *         @arg HW_ADC2  :ADC2ģ��
 * @param  mux: ADC ת����ͨ�� ���� ѡ��
 *         @arg kADC_MuxA   :Aͨ��ģʽ
 *         @arg kADC_MuxB   :Bͨ��ģʽ
 * @retval :��ȡ��� �����ǰ��δ���ת�� �򷵻���һ�ν��
 */
int32_t ADC_ReadValue(uint32_t instance, uint32_t mux)
{
    return ADC_InstanceTable[instance]->R[mux];
}

/**
 * @brief  ��ȡADCת�����(�򻯰�) ֻ������ADC���ٳ�ʼ���꼴��
 * @note   ����ʽ ֱ������ת�����    
 * @code
 *    //��ȡAD0ģ��20ͨ��DM1���ŵ�ת�����
 *    uint32_t value;   //�洢����ת�����
 *    value = ADC_QuickReadValue(ADC0_SE20_DM1);
 * @endcode
 * @param  MAP: ���ٳ�ʼ���꣬���ADC.H�ļ�
 * @retval ת�����
 */
int32_t ADC_QuickReadValue(uint32_t MAP)
{
    QuickInit_Type * pq = (QuickInit_Type*)&(MAP);
    uint32_t instance = pq->ip_instance;
    uint32_t chl = pq->channel;
    ADC_StartConversion(instance, chl, kADC_MuxA);
    /* waiting for ADC complete */
    while((ADC_InstanceTable[instance]->SC1[kADC_MuxA] & ADC_SC1_COCO_MASK) == 0);
    return ADC_ReadValue(instance, kADC_MuxA);
}

/**
 * @brief  ADC�жϼ�DMA���ܿ��غ���
 * @code
 *    //����AD0ģ�� ת������ж�
 *    ADC_ITDMAConfig(HW_ADC0, kADC_MuxA, kADC_IT_EOF);
 * @endcode         
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0  :ADC0ģ��
 *         @arg HW_ADC1  :ADC1ģ��
 *         @arg HW_ADC2  :ADC2ģ��
 * @param  mux: ADC ת����ͨ�� ���� ѡ��
 *         @arg kADC_MuxA   :Aͨ��ģʽ
 *         @arg kADC_MuxB   :Bͨ��ģʽ
 * @param  config:   ADC�жϼ�DMA����
 *         @arg kADC_IT_Disable    :�ر��ж�
 *         @arg kADC_DMA_Disable   :�ر�DMA���� 
 *         @arg kADC_IT_EOF        :ת������ж�
 *         @arg kADC_DMA_EOF       :DMA����ж�
 * @retval None
 */
void ADC_ITDMAConfig(uint32_t instance, uint32_t mux, ADC_ITDMAConfig_Type config)
{
    switch(config)
    {
        case kADC_IT_Disable:
            NVIC_DisableIRQ(ADC_IRQnTable[instance]);
            ADC_InstanceTable[instance]->SC1[mux] &= ~ADC_SC1_AIEN_MASK;
            break;
        case kADC_DMA_Disable:
            ADC_InstanceTable[instance]->SC2 &= ~ADC_SC2_DMAEN_MASK;
            break;
        case kADC_IT_EOF:
            ADC_InstanceTable[instance]->SC1[mux] |= ADC_SC1_AIEN_MASK;
            NVIC_EnableIRQ(ADC_IRQnTable[instance]);
            break; 
        case kADC_DMA_EOF:
            ADC_InstanceTable[instance]->SC2 |= ADC_SC2_DMAEN_MASK;
            break;
        default:
            break;
    }
}

/**
 * @brief  ע���жϻص�����
 * @param  instance: ADC ģ���
 *         @arg HW_ADC0  :ADC0ģ��
 *         @arg HW_ADC1  :ADC1ģ��
 *         @arg HW_ADC2  :ADC2ģ��
 * @param AppCBFun: �ص�����ָ�����
 * @retval None
 * @note ���ڴ˺����ľ���Ӧ�������Ӧ��ʵ��
 */
void ADC_CallbackInstall(uint32_t instance, ADC_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        ADC_CallBackTable[instance] = AppCBFun;
    }
}

//! @}

//! @}
/**
 * @brief  �жϴ��������
 * @param  ADC0_IRQHandler :оƬ��ADC0ģ���жϺ������
 *         ADC1_IRQHandler :оƬ��ADC0ģ���жϺ������
 * @note �����ڲ������ж��¼�����
 */
void ADC0_IRQHandler(void)
{
    if(ADC_CallBackTable[HW_ADC0] != NULL)
    {
        ADC_CallBackTable[HW_ADC0]();
    }
}

void ADC1_IRQHandler(void)
{
    if(ADC_CallBackTable[HW_ADC1] != NULL)
    {
        ADC_CallBackTable[HW_ADC1]();
    }
}

/*
const QuickInit_Type FTM_QuickInitTable[] =
{
    { 0, 0, 0, 0, 0, 0, 0}, //ADC0_SE0_DP0
    { 0, 0, 0, 0, 0, 1, 0}, //ADC0_SE1_DP1
    { 0, 0, 0, 0, 0, 3, 0}, //ADC0_SE3_DP3
    { 0, 2, 0, 2, 1, 4, 1}, //ADC0_SE4B_PC2
    { 0, 3, 0, 1, 1, 5, 1}, //ADC0_SE5B_PD1
    { 0, 3, 0, 5, 1, 6, 1}, //ADC0_SE6B_PD5
    { 0, 3, 0, 6, 1, 7, 1}, //ADC0_SE7B_PD6
    { 0, 1, 0, 0, 1, 8, 0}, //ADC0_SE8_PB0
    { 0, 1, 0, 1, 1, 9, 0}, //ADC0_SE9_PB1
    { 0, 1, 0, 2, 1,12, 0}, //ADC0_SE12_PB2
    { 0, 1, 0, 3, 1,13, 0}, //ADC0_SE13_PB3
    { 0, 2, 0, 0, 1,14, 0}, //ADC0_SE14_PC0
    { 0, 2, 0, 1, 1,15, 0}, //ADC0_SE15_PC1
    { 0, 4, 0,24, 1,17, 0}, //ADC0_SE17_E24
    { 0, 4, 0,25, 1,18, 0}, //ADC0_SE18_E25
    { 0, 0, 0, 0, 0,19, 0}, //ADC0_SE19_DM0
    { 0, 0, 0, 0, 0,20, 0}, //ADC0_SE20_DM1
    { 0, 0, 0, 0, 0,26, 0}, //ADC0_SE26_TEMP
    { 1, 0, 0, 0, 0, 0, 0}, //ADC1_SE0_DP0
    { 1, 0, 0, 0, 0, 1, 0}, //ADC1_SE1_DP1
    { 1, 0, 0, 0, 0, 3, 0}, //ADC1_SE3_DP3
    { 1, 4, 0, 0, 1, 4, 0}, //ADC1_SE4_PE0
    { 1, 4, 0, 1, 1, 5, 0}, //ADC1_SE5_PE1
    { 1, 4, 0, 2, 1, 6, 0}, //ADC1_SE6_PE2
    { 1, 4, 0, 3, 1, 7, 0}, //ADC1_SE7_PE3
    { 1, 2, 0, 8, 1, 4, 1}, //ADC1_SE4B_PC8
    { 1, 2, 0, 9, 1, 5, 1}, //ADC1_SE5B_PC9
    { 1, 2, 0,10, 1, 6, 1}, //ADC1_SE6B_PC10
    { 1, 2, 0,11, 1, 7, 1}, //ADC1_SE7B_PC11
    { 1, 1, 0, 0, 1, 8, 0}, //ADC1_SE8_PB0
    { 1, 1, 0, 1, 1, 9, 0}, //ADC1_SE9_PB1
    { 1, 1, 0,10, 1,14, 0}, //ADC1_SE14_PB10
    { 1, 1, 0,11, 1,15, 0}, //ADC1_SE15_PB11
    { 1, 0, 0,17, 1,17, 0}, //ADC1_SE17_PA17
    { 1, 0, 0, 0, 0,19, 0}, //ADC1_SE19_DM0
    { 1, 0, 0, 0, 0,20, 0}, //ADC1_SE20_DM1
    { 1, 0, 0, 0, 0,26, 0}, //ADC1_SE26_TEMP
    { 0, 0, 0, 0, 0,21, 0}, //ADC0_SE21
    { 0, 0, 0, 0, 0,22, 0}, //ADC0_SE22
    { 1, 0, 0, 0, 0,16, 0}, //ADC1_SE16
    { 1, 0, 0, 0, 0,18, 0}, //ADC1_SE18
    { 0, 0, 0, 0, 0,23, 0}, //ADC0_SE23
    { 1, 0, 0, 0, 0,23, 0}, //ADC1_SE23
    { 1, 0, 0, 0, 0,23, 0}, //ADC1_SE23
    { 1, 1, 0, 4, 1,10, 0}, //ADC1_SE10_B04
    { 1, 1, 0, 5, 1,11, 0}, //ADC1_SE11_B05
};
*/

