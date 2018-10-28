/**
  ******************************************************************************
  * @file    uart.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.25
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬUARTģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */
#ifndef __CH_LIB_UART_H__
#define __CH_LIB_UART_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


/*!< UART Ӳ��ģ��� */
#define HW_UART0  (0x00U)  /* оƬ��UART0�˿� */
#define HW_UART1  (0x01U)
#define HW_UART2  (0x02U)
#define HW_UART3  (0x03U)
#define HW_UART4  (0x04U)
#define HW_UART5  (0x05U)

/*!< UART ���ٳ�ʼ���� */                      
#define UART1_RX_PE01_TX_PE00   (0x80E1U) //оƬ��UART1�˿ڣ�ʹ��PTE1����Ϊ�������ţ�ʹ��PTE0����Ϊ��������
#define UART0_RX_PF17_TX_PF18   (0xA528U)
#define UART3_RX_PE05_TX_PE04   (0x88E3U)
#define UART5_RX_PF19_TX_PF20   (0xA72DU)
#define UART5_RX_PE09_TX_PE08   (0x90E5U)
#define UART2_RX_PE17_TX_PE16   (0xA0E2U)
#define UART4_RX_PE25_TX_PE24   (0xB0E4U)
#define UART0_RX_PA01_TX_PA02   (0x8280U)
#define UART0_RX_PA15_TX_PA14   (0x9CC0U)
#define UART3_RX_PB10_TX_PB11   (0x94CBU)
#define UART0_RX_PB16_TX_PB17   (0xA0C8U)
#define UART1_RX_PC03_TX_PC04   (0x86D1U)
#define UART4_RX_PC14_TX_PC15   (0x9CD4U)
#define UART3_RX_PC16_TX_PC17   (0xA0D3U)
#define UART2_RX_PD02_TX_PD03   (0x84DAU)
#define UART0_RX_PD06_TX_PD07   (0x8CD8U)
#define UART2_RX_PF13_TX_PF14   (0x9B2AU) 
#define UART5_RX_PD08_TX_PD09   (0x90DDU) //оƬ��UART5�˿ڣ�ʹ��PTD8����Ϊ�������ţ�ʹ��PTD9����Ϊ��������
#define UART5_RX_PE08_TX_PE09   (0X90E5U)
/*!< parity ѡ�� */
typedef enum
{
    kUART_ParityDisabled = 0x0,  // У��λ��ֹ
    kUART_ParityEven     = 0x2,  // 1λ ��У�� 
    kUART_ParityOdd      = 0x3,  // 1λ żУ�� 
} UART_ParityMode_Type;

/*!< ÿ֡����λ���� */
typedef enum 
{
    kUART_8BitsPerChar  = 0,   // 8-bit ���� ������У��λ 
    kUART_9BitsPerChar  = 1,   // 9-bit ���� ������У��λ 
} UART_BitPerChar_Type;

/*!< �жϼ�DMA���� */
typedef enum
{
    kUART_IT_Tx,                // ����ÿ����һ֡��������ж� 
    kUART_DMA_Tx,               // ����ÿ����һ֡������ɴ���DMA 
    kUART_IT_Rx,                // ����ÿ����һ֡��������ж� 
    kUART_DMA_Rx,               // ����ÿ����һ֡������ɴ���DMA 
}UART_ITDMAConfig_Type;

/*!< UART��ʼ���ṹ */
typedef struct
{
    uint32_t                srcClock;       // ʱ��ԴƵ��
    uint8_t                 instance;       // UART ģ��� HW_UART0~HW_UART5
    uint32_t                baudrate;       // UART ������ 
    UART_ParityMode_Type    parityMode;     // UART У��λ 
    UART_BitPerChar_Type    bitPerChar;     // UART ÿһ֡������λ���� 
}UART_InitTypeDef;

/*!< UART �ص��������� */
typedef void (*UART_CallBackTxType)(uint16_t * pbyteToSend);
typedef void (*UART_CallBackRxType)(uint16_t byteReceived);

/*!< API functions */

uint8_t UART_QuickInit(uint32_t MAP, uint32_t baudrate);
void UART_Init(UART_InitTypeDef * UART_InitStruct);
void UART_DeInit(uint32_t instance);
int UART_printf(const char *format,...);
uint8_t UART_ReadByte(uint32_t instance, uint16_t *ch);
void UART_WriteByte(uint32_t instance, uint16_t ch);
void UART_SelectDebugInstance(uint32_t instance);

/* FIFO functions */
void UART_EnableTxFIFO(uint32_t instance, bool status);
void UART_EnableRxFIFO(uint32_t instance, bool status);
uint32_t UART_GetTxFIFOSize(uint32_t instance);
uint32_t UART_GetRxFIFOSize(uint32_t instance);
void UART_SetTxFIFOWatermark(uint32_t instance, uint32_t size);
void UART_SetRxFIFOWatermark(uint32_t instance, uint32_t size);

/* Interrupt and DMA functions */
void UART_CallbackTxInstall(uint32_t instance, UART_CallBackTxType AppCBFun);
void UART_CallbackRxInstall(uint32_t instance, UART_CallBackRxType AppCBFun);
void UART_ITDMAConfig(uint32_t instance, UART_ITDMAConfig_Type config, bool status);
int UART_printf(const char *fmt, ...);


/* DMA support */
#ifdef DMA0
void UART_SetDMATxMode(uint32_t instance, bool status);
void UART_DMASendByte(uint32_t instance, uint8_t* buf, uint32_t size);
uint32_t UART_DMAGetRemainByte(uint32_t instance);
#endif

//!< param check
#define IS_UART_ALL_INSTANCE(INSTANCE)  (INSTANCE < ARRAY_SIZE(UART_InstanceTable))

#ifdef __cplusplus
}
#endif

#endif

