/**
  ******************************************************************************
  * @file    enet.h
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.26
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬENETģ��ĵײ㹦�ܺ���������Ӧ����鿴ʵ������
  ******************************************************************************
  */
  
#ifndef __CH_LIB_ENET_H__
#define __CH_LIB_ENET_H__


#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
   

/* TX������������λ���� */
#define TX_BD_R			0x0080
#define TX_BD_TO1		0x0040
#define TX_BD_W			0x0020
#define TX_BD_TO2		0x0010
#define TX_BD_L			0x0008
#define TX_BD_TC		0x0004
#define TX_BD_ABC		0x0002

/* TX��ǿ�ͻ�����������λ���� */
#define TX_BD_INT       0x00000040 
#define TX_BD_TS        0x00000020 
#define TX_BD_PINS      0x00000010 
#define TX_BD_IINS      0x00000008 
#define TX_BD_TXE       0x00800000 
#define TX_BD_UE        0x00200000 
#define TX_BD_EE        0x00100000
#define TX_BD_FE        0x00080000 
#define TX_BD_LCE       0x00040000 
#define TX_BD_OE        0x00020000 
#define TX_BD_TSE       0x00010000 

#define TX_BD_BDU       0x00000080    

/* RX������������λ���� */
// 0ƫ�Ʊ�־ - ״̬:��˸�ʽ
#define RX_BD_E			0x0080
#define RX_BD_R01		0x0040
#define RX_BD_W			0x0020
#define RX_BD_R02		0x0010
#define RX_BD_L			0x0008
#define RX_BD_M			0x0001
#define RX_BD_BC		0x8000
#define RX_BD_MC		0x4000
#define RX_BD_LG		0x2000
#define RX_BD_NO		0x1000
#define RX_BD_CR		0x0400
#define RX_BD_OV		0x0200
#define RX_BD_TR		0x0100

/* RX��ǿ�ͻ�����������λ���� */
#define RX_BD_ME               0x00000080    
#define RX_BD_PE               0x00000004    
#define RX_BD_CE               0x00000002    
#define RX_BD_UC               0x00000001
    
#define RX_BD_INT              0x00008000    

#define RX_BD_ICE              0x20000000    
#define RX_BD_PCR              0x10000000    
#define RX_BD_VLAN             0x04000000    
#define RX_BD_IPV6             0x02000000    
#define RX_BD_FRAG             0x01000000    

#define RX_BD_BDU              0x00000080   

/* MII�ӿڳ�ʱ */
#define MII_TIMEOUT		0x1FFFF

/* ��̫֡��ض��� */
#define CFG_NUM_ENET_TX_BUFFERS     1     //���ͻ���������
#define CFG_NUM_ENET_RX_BUFFERS     1     //���ջ���������  ���������趨����Ϊ1�� ���˾ͻ����
#define CFG_ENET_BUFFER_SIZE        1518    //��̫����֡����������

/* �������������ṹ�� */
typedef struct
{
    uint16_t status;	            /* control and status */
  	uint16_t length;	            /* transfer length */
    uint8_t  *data;	                /* buffer address */
  	uint32_t ebd_status;
  	uint16_t length_proto_type;
  	uint16_t payload_checksum;
  	uint32_t bdu;
  	uint32_t timestamp;
  	uint32_t reserverd_word1;
  	uint32_t reserverd_word2;
} NBUF;

typedef enum
{
    kENET_RMII_10M,
    kENET_RMII_100M,
}ENET_RMII_SpeedType;


//!< ��̫����ʼ���ṹ
typedef struct
{
    uint8_t* pMacAddress;
    bool is10MSpped;
    bool isHalfDuplex;
}ENET_InitTypeDef;
     
typedef enum
{
    kENET_IT_TXF_Disable,   //!< ��ֹ����һ֡������ж�     
    kENET_IT_RXF_Disable,   //!< ��ֹ����һ֡������ж�    
    kENET_IT_TXF,           //!< ����ENET����һ֡�ж�    
    kENET_IT_RXF,           //!< ����ENET����һ֡�ж�
}ENET_ITDMAConfig_Type;

//!< ENET CallBack Type
typedef void (*ENET_CallBackTxType)(void);
typedef void (*ENET_CallBackRxType)(void);

//!< API functions

/* RMII API */
void ENET_MII_Init(void);
bool ENET_MII_Read(uint16_t phy_addr, uint16_t reg_addr, uint16_t *data);
bool ENET_MII_Write(uint16_t phy_addr, uint16_t reg_addr, uint16_t data);

/* controller API */
void ENET_Init(ENET_InitTypeDef* ENET_InitStrut);
void ENET_MacSendData(uint8_t *data, uint16_t len);
uint16_t ENET_MacReceiveData(uint8_t *data);
void ENET_ITDMAConfig(ENET_ITDMAConfig_Type config);
void ENET_CallbackTxInstall(ENET_CallBackTxType AppCBFun);
void ENET_CallbackRxInstall(ENET_CallBackRxType AppCBFun);
bool ENET_IsTxTransferComplete(void);

#ifdef __cplusplus
}
#endif



#endif


