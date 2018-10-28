/**
  ******************************************************************************
  * @file    ftm.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ��ģ��ΪоƬFTMģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */
#ifndef __CH_LIB_FTM_H_
#define	__CH_LIB_FTM_H_

#ifdef __cplusplus
	extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* FTM Ӳ��ģ��� */
#define HW_FTM0    (0x00)
#define HW_FTM1    (0x01) 
#define HW_FTM2    (0x02)
#define HW_FTM3    (0x03)

/* FTM���õ�ͨ�� */
#define HW_FTM_CH0 (0x00)
#define HW_FTM_CH1 (0x01)
#define HW_FTM_CH2 (0x02)
#define HW_FTM_CH3 (0x03)
#define HW_FTM_CH4 (0x04)
#define HW_FTM_CH5 (0x05)
#define HW_FTM_CH6 (0x06)
#define HW_FTM_CH7 (0x07)

/* PWM ������� */
typedef enum
{
	kPWM_EdgeAligned,           //���ض��� ���
	kPWM_Combine,               //���ģʽ
    kPWM_Complementary,         //����ģʽ �������ģʽ ����Chl(n) �� Chl(n+1) �ǻ������
}FTM_PWM_Mode_Type;
/* PWM ���Է�ת */
#define kFTM_PWM_HighTrue       (0x00)
#define kFTM_PWM_LowTrue        (0x01)

/** QD �������� */
typedef enum
{
    kFTM_QD_NormalPolarity,     /* �������� */
    kFTM_QD_InvertedPolarity,   /*�������� */
}FTM_QD_PolarityMode_Type;
 
typedef enum
{
	kQD_PHABEncoding,           /* ʹ��AB������� */
	kQD_CountDirectionEncoding, /* ʹ�÷���-�����ͱ����� */
}FTM_QD_Mode_Type;

/* IC ���벶׽ */
typedef enum
{
	kFTM_IC_FallingEdge,
	kFTM_IC_RisingEdge,
	kFTM_IC_RisingFallingEdge,
}FTM_IC_Mode_Type;

//!< FTM �жϼ�DMA ����
typedef enum
{
    kFTM_IT_TOF,
    kFTM_IT_CH0,
    kFTM_IT_CH1,
    kFTM_IT_CH2,
    kFTM_IT_CH3,
    kFTM_IT_CH4,
    kFTM_IT_CH5,
    kFTM_IT_CH6,
    kFTM_IT_CH7,
    kFTM_DMA_CH0,
    kFTM_DMA_CH1,
    kFTM_DMA_CH2,
    kFTM_DMA_CH3,
    kFTM_DMA_CH4,
    kFTM_DMA_CH5,
    kFTM_DMA_CH6,
    kFTM_DMA_CH7,
}FTM_ITDMAConfig_Type;

/* ��Ƶ */
typedef enum
{
    kFTM_ClockDiv1,
    kFTM_ClockDiv2,
    kFTM_ClockDiv4,
    kFTM_ClockDiv8,
    kFTM_ClockDiv16,
    kFTM_ClockDiv32,
    kFTM_ClockDiv64,
    kFTM_ClockDiv128,
}FTM_ClockDiv_Type;

//!< FTM PWM ���ٳ�ʼ��
#define FTM0_CH4_PB12   (0x205908U)  /* FTM0ģ���4ͨ�� PTB12���� �������� */
#define FTM0_CH5_PB13   (0x285b08U)
#define FTM0_CH5_PA00   (0x2840c0U)
#define FTM0_CH6_PA01   (0x3042c0U)
#define FTM0_CH7_PA02   (0x3844c0U)
#define FTM0_CH0_PA03   (0x46c0U)
#define FTM0_CH1_PA04   (0x848c0U)
#define FTM0_CH2_PA05   (0x104ac0U)
#define FTM0_CH3_PA06   (0x184cc0U)
#define FTM0_CH4_PA07   (0x204ec0U)
#define FTM0_CH0_PC01   (0x4310U)
#define FTM0_CH1_PC02   (0x84510U)
#define FTM0_CH2_PC03   (0x104710U)
#define FTM0_CH3_PC04   (0x184910U)
#define FTM0_CH4_PD04   (0x204918U)
#define FTM0_CH5_PD05   (0x284b18U)
#define FTM0_CH6_PD06   (0x304d18U)
#define FTM0_CH7_PD07   (0x384f18U)
#define FTM1_CH0_PB12   (0x58c9U)
#define FTM1_CH1_PB13   (0x85ac9U)
#define FTM1_CH0_PA08   (0x50c1U)
#define FTM1_CH1_PA09   (0x852c1U)
#define FTM1_CH0_PA12   (0x58c1U)
#define FTM1_CH1_PA13   (0x85ac1U)
#define FTM1_CH0_PB00   (0x40c9U)
#define FTM1_CH1_PB01   (0x842c9U)
#define FTM2_CH0_PA10   (0x54c2U)
#define FTM2_CH1_PA11   (0x856c2U)
#define FTM2_CH0_PB18   (0x64caU)
#define FTM2_CH1_PB19   (0x866caU)
#define FTM3_CH0_PE05   (0X00004BA3U)
#define FTM3_CH1_PE06   (0X00084DA3U)
#define FTM3_CH2_PE07   (0X00104FA3U)
#define FTM3_CH3_PE08   (0X001851A3U)
#define FTM3_CH4_PE09   (0X002053A3U)
#define FTM3_CH5_PE10   (0X002855A3U)
#define FTM3_CH6_PE11   (0X003057A3U)
#define FTM3_CH7_PE12   (0X003859A3U)
#define FTM3_CH4_PC08   (0X002050D3U)
#define FTM3_CH5_PC09   (0X002852D3U)
#define FTM3_CH6_PC10   (0X003054D3U)
#define FTM3_CH7_PC11   (0X003856D3U)
#define FTM3_CH0_PD00   (0X0000411BU)
#define FTM3_CH1_PD01   (0X0008431BU)
#define FTM3_CH2_PD02   (0X0010451BU)
#define FTM3_CH3_PD03   (0X0018471BU)



//!< FTM ����������ٳ�ʼ��
#define FTM1_QD_PHA_PA08_PHB_PA09       (0x9181U) /* FTM1ģ���PTA8��PTA9Ϊ�������� */
#define FTM1_QD_PHA_PA12_PHB_PA13       (0x99c1U)
#define FTM1_QD_PHA_PB00_PHB_PB01       (0x8189U)
#define FTM2_QD_PHA_PA10_PHB_PA11       (0x9582U)
#define FTM2_QD_PHA_PB18_PHB_PB19       (0xa58aU)

//!< Callback
typedef void (*FTM_CallBackType)(void);

//!< API functions

/* PWM functions */
uint8_t FTM_PWM_QuickInit(uint32_t MAP, FTM_PWM_Mode_Type mode, uint32_t req);
void FTM_PWM_ChangeDuty(uint32_t instance, uint8_t chl, uint32_t pwmDuty);
void FTM_PWM_InvertPolarity(uint32_t instance, uint8_t chl, uint32_t config);
/* QD functions */
uint32_t FTM_QD_QuickInit(uint32_t MAP, FTM_QD_PolarityMode_Type polarity, FTM_QD_Mode_Type mode);
void FTM_QD_GetData(uint32_t instance, int* value, uint8_t* direction);
void FTM_QD_ClearCount(uint32_t instance);
/* IC functions */
void FTM_IC_QuickInit(uint32_t MAP, FTM_ClockDiv_Type ps);
void FTM_IC_SetTriggerMode(uint32_t instance, uint32_t chl, FTM_IC_Mode_Type mode);
/* IT & DMA config */
void FTM_ITDMAConfig(uint32_t instance, FTM_ITDMAConfig_Type config, bool flag);
void FTM_CallbackInstall(uint32_t instance, FTM_CallBackType AppCBFun);
/* control function */
uint32_t FTM_GetChlCounter(uint32_t instance, uint32_t chl);
void FTM_SetMoudleCounter(uint32_t instance, uint32_t val);




#ifdef __cplusplus
}
#endif





#endif
