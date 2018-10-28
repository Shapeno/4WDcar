/**
  ******************************************************************************
  * @file    tsi.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬTSIģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */
  
 #include "tsi.h"
 #include "common.h"
 #include "gpio.h"
 
#ifdef TSI0

static TSI_CallBackType TSI_CallBackTable[1] = {NULL};
 
static const uint16_t * TSI_ChlCNTRTable[] =
{
    (uint16_t*)&TSI0->CNTR1 + 0,
    (uint16_t*)&TSI0->CNTR1 + 1,
    (uint16_t*)&TSI0->CNTR3 + 0,
    (uint16_t*)&TSI0->CNTR3 + 1,
    (uint16_t*)&TSI0->CNTR5 + 0,
    (uint16_t*)&TSI0->CNTR5 + 1,
    (uint16_t*)&TSI0->CNTR7 + 0,
    (uint16_t*)&TSI0->CNTR7 + 1,
    (uint16_t*)&TSI0->CNTR9 + 0,
    (uint16_t*)&TSI0->CNTR9 + 1,
    (uint16_t*)&TSI0->CNTR11 + 0,
    (uint16_t*)&TSI0->CNTR11 + 1,
    (uint16_t*)&TSI0->CNTR13 + 0,
    (uint16_t*)&TSI0->CNTR13 + 1,
    (uint16_t*)&TSI0->CNTR15 + 0,
    (uint16_t*)&TSI0->CNTR15 + 1,
};

/**
 * @brief  ���ָ��ͨ������ֵ
 * @param  chl :tsiģ���ͨ��1~15
 * @retval ��ͨ��������ֵ
 */
uint32_t TSI_GetCounter(uint32_t chl)
{
    return *TSI_ChlCNTRTable[chl];
}

/**
 * @brief  TSIͨ��У�麯��
 * @param  chl      :tsiģ���ͨ��1~15
 * @param  threshld :����У��ֵ
 * @retval ��ͨ��������ֵ
 */
static void TSI_Calibration(uint32_t chl, uint32_t threshld)
{
    uint32_t cnt;
	TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;
    TSI0->GENCS &= ~TSI_GENCS_STM_MASK;
	TSI0->GENCS |= TSI_GENCS_SWTS_MASK;
	while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK)){};
    cnt = TSI_GetCounter(chl);
    LIB_TRACE("TSI_Calibration cnt:%d\r\n", cnt);
    LIB_TRACE("TSI_Calibration threshld:%d\r\n", threshld);
	TSI0->THRESHOLD = TSI_THRESHOLD_HTHH(cnt + threshld) | TSI_THRESHOLD_LTHH(cnt - threshld);
    TSI0->GENCS &= ~TSI_GENCS_TSIEN_MASK;
}

/**
 * @brief  TSI��ʼ������
 * @code
 *    //��ʼ������TSIģ���1ͨ��Ϊ���ڴ�����ʽ���жϷ�ֵΪ200
 *    TSI_InitTypeDef  TSI_InitStruct1;      //����һ���ṹ����
 *    TSI_InitStruct1.chl = 1;   //ѡ��ͨ��
 *    TSI_InitStruct1.triggerMode = kTSI_TriggerPeriodicalScan; //Ӳ������ɨ��
 *    TSI_InitStruct1.threshld = 200;   //���÷�ֵΪ200
 *    TSI_Init(&TSI_InitStruct1);
 * @endcode
 * @param  TSI_InitStruct: TSI��ʼ���ṹ��,�洢ͨ���Ĺ���״̬
 * @retval None
 */
void TSI_Init(TSI_InitTypeDef* TSI_InitStruct)
{
	SIM->SCGC5 |= (SIM_SCGC5_TSI_MASK); 
    /* disalbe moudle */
    TSI0->GENCS = 0;
    /* Number of Consecutive Scans Per Electrode Electrode */
    TSI0->GENCS |= TSI_GENCS_NSCN(TSI_InitStruct->consecutiveScanTimes);
    /* Electrode Oscillator Prescaler */
    TSI0->GENCS |= TSI_GENCS_PS(TSI_InitStruct->electrodeOSCPrescaler);
    /* clear SCANC */
    TSI0->SCANC = 0;
    /* Ref OSC Charge Current Select */
    TSI0->SCANC |= TSI_SCANC_REFCHRG(TSI_InitStruct->refChargeCurrent);
    /* External OSC Charge Current Select */
    TSI0->SCANC |= TSI_SCANC_EXTCHRG(TSI_InitStruct->extChargeCurrent);
    /* clock source is bus clock LPOSCCLK? */
    TSI0->SCANC |= TSI_SCANC_SMOD(0)|TSI_SCANC_AMPSC(0);
    /* enable all pens */
    /* FIXME: seems if there is only one TSI pen, TSI cannot work in continues module(STM=1) */	
    TSI0->PEN |= (1<<TSI_InitStruct->chl|(1<<0));			
    TSI_Calibration(TSI_InitStruct->chl, TSI_InitStruct->threshld);
    /* config trigger mode */
    switch(TSI_InitStruct->triggerMode)
    {
        case kTSI_TriggerSoftware:
            TSI0->GENCS &= ~TSI_GENCS_STM_MASK;
            break;
        case kTSI_TriggerPeriodicalScan:
            TSI0->GENCS |= TSI_GENCS_STM_MASK;
            break;
        default:
            break;
    }
    /* clear all IT flags */
    TSI0->GENCS |= TSI_GENCS_OUTRGF_MASK;	  
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK;	 
	TSI0->GENCS |= TSI_GENCS_EXTERF_MASK;
	TSI0->GENCS |= TSI_GENCS_OVRF_MASK;
//	TSI0->STATUS = 0xFFFFFFFF;
    /* enable moudle */
	TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;
}

/**
 * @brief  TSI���ٳ�ʼ������
 * @code
 *    //���ٳ�ʼ������TSIģ���1ͨ����PTA0������Ĭ�ϴ�������
 *    TSI_QuickInit(TSI0_CH1_PA00);
 * @endcode
 * @param  MAP: TSI��ʼ��Ԥ���壬���tsi.h�ļ�
 * @retval ͨ����
 */
uint32_t TSI_QuickInit(uint32_t MAP)
{
    uint32_t i;
    TSI_InitTypeDef TSI_InitStruct;
    QuickInit_Type * pq = (QuickInit_Type*)&MAP;
    /* config pinmux */
    for(i = 0; i < pq->io_offset; i++)
    {
        PORT_PinMuxConfig(pq->io_instance, pq->io_base + i, (PORT_PinMux_Type) pq->mux);
    }
    /* config TSI moudle */
    TSI_InitStruct.chl = pq->channel;
    TSI_InitStruct.triggerMode = kTSI_TriggerPeriodicalScan;
    TSI_InitStruct.threshld = 700;
    TSI_InitStruct.consecutiveScanTimes = 2;
    TSI_InitStruct.electrodeOSCPrescaler = kTSI_EletrodeOscDiv_2;
    TSI_InitStruct.extChargeCurrent = kTSI_ChargeCurrent_32uA;
    TSI_InitStruct.refChargeCurrent = kTSI_ChargeCurrent_32uA;
    TSI_Init(&TSI_InitStruct);
    return pq->ip_instance;
}

/**
 * @brief  TSIģ���ж����ͻ���DMA��������
 * @code
 *      //����ɨ����������ж�ģʽ
 *      TSI_ITDMAConfig(kTSI_IT_EndOfScan); 
 * @endcode
 * @param config: �ж�����ģʽ
 *         @arg kTSI_IT_Disable �ر��жϹ���
 *         @arg kTSI_IT_OutOfRange ������ֵ�����ж�
 *         @arg kTSI_IT_EndOfScan ɨ����������ж�
 * @retval None
 */
void TSI_ITDMAConfig(TSI_ITDMAConfig_Type config)
{
    switch(config)
    {
        case kTSI_IT_Disable:
            TSI0->GENCS &= ~TSI_GENCS_TSIIE_MASK;
            break;
        case kTSI_IT_OutOfRange:
            TSI0->GENCS |= TSI_GENCS_TSIIE_MASK;
            TSI0->GENCS &= ~TSI_GENCS_ESOR_MASK;
            NVIC_EnableIRQ(TSI0_IRQn);
            break;
        case kTSI_IT_EndOfScan:
            TSI0->GENCS |= TSI_GENCS_TSIIE_MASK;
            TSI0->GENCS |= TSI_GENCS_ESOR_MASK;
            NVIC_EnableIRQ(TSI0_IRQn);
            break; 
        default:
            break;
    }
}

/**
 * @brief  ע���жϻص�����
 * @param AppCBFun: �ص�����ָ�����
 * @retval None
 * @note ���ڴ˺����ľ���Ӧ�������Ӧ��ʵ��
 */
void TSI_CallbackInstall(TSI_CallBackType AppCBFun)
{
    if(AppCBFun != NULL)
    {
        TSI_CallBackTable[0] = AppCBFun;
    }
}
/**
 * @brief  �жϴ��������
 * @param  TSI0_IRQHandler  :оƬ��TSIģ���жϺ������
 * @note �����ڲ������ж��¼�����
 */
void TSI0_IRQHandler(void)
{
    /* clear all IT pending bit */
    TSI0->GENCS |= TSI_GENCS_OUTRGF_MASK;
	TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
  //  chl_array = TSI0->STATUS & 0xFFFF;
   // TSI0->STATUS = 0xFFFFFFFF;
    if(TSI_CallBackTable[0])
    {
        TSI_CallBackTable[0]();
    }  
}

#endif

/*
 static  QuickInit_Type TSI_QuickInitTable[] =
{
    { 0, 1, 0, 0, 1, 0},    //TSI0_CH0_PB00
    { 0, 0, 0, 0, 1, 1},    //TSI0_CH1_PA00
    { 0, 0, 0, 1, 1, 2},    //TSI0_CH2_PA01
    { 0, 0, 0, 2, 1, 3},    //TSI0_CH3_PA02
    { 0, 0, 0, 3, 1, 4},    //TSI0_CH4_PA03
    { 0, 0, 0, 4, 1, 5},    //TSI0_CH5_PA04
    { 0, 1, 0, 1, 1, 6},    //TSI0_CH6_PB01
    { 0, 1, 0, 2, 1, 7},    //TSI0_CH7_PB02
    { 0, 1, 0, 3, 1, 8},    //TSI0_CH8_PB03
    { 0, 1, 0,16, 1, 9},    //TSI0_CH9_PB16
    { 0, 1, 0,17, 1,10},    //TSI0_CH10_PB17
    { 0, 1, 0,18, 1,11},    //TSI0_CH11_PB18
    { 0, 1, 0,19, 1,12},    //TSI0_CH12_PB19
    { 0, 2, 0, 0, 1,13},    //TSI0_CH13_PC00
    { 0, 2, 0, 1, 1,14},    //TSI0_CH14_PC01
    { 0, 2, 0, 2, 1,15},    //TSI0_CH15_PC02
};
*/


