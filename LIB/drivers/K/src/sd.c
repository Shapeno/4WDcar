/**
  ******************************************************************************
  * @file    sd.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬSDģ��ĵײ㹦�ܺ���
              ֻ֧��SD��SDHC�� ��֧��MMC��
  ******************************************************************************
  */
  
#include "sd.h"
#include "gpio.h"

#if defined(SDHC)


//DATA��λ����
#define ESDHC_BUS_WIDTH_1BIT                 (0x00)
#define ESDHC_BUS_WIDTH_4BIT                 (0x01)
#define ESDHC_BUS_WIDTH_8BIT                 (0x02)
//SD��ָ�����
#define ESDHC_XFERTYP_CMDTYP_NORMAL          (0x00)
#define ESDHC_XFERTYP_CMDTYP_SUSPEND         (0x01)
#define ESDHC_XFERTYP_CMDTYP_RESUME          (0x02)
#define ESDHC_XFERTYP_CMDTYP_ABORT           (0x03)
//SD��ָ���������
#define ESDHC_XFERTYP_RSPTYP_NO              (0x00)
#define ESDHC_XFERTYP_RSPTYP_136             (0x01)
#define ESDHC_XFERTYP_RSPTYP_48              (0x02)
#define ESDHC_XFERTYP_RSPTYP_48BUSY          (0x03)
//SD��ָ�
#define ESDHC_CMD0   (SDHC_XFERTYP_CMDINX(0)  | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
#define ESDHC_CMD1   (SDHC_XFERTYP_CMDINX(1)  | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
#define ESDHC_CMD2   (SDHC_XFERTYP_CMDINX(2)  | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136))
#define ESDHC_CMD3   (SDHC_XFERTYP_CMDINX(3)  | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD4   (SDHC_XFERTYP_CMDINX(4)  | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
#define ESDHC_CMD5   (SDHC_XFERTYP_CMDINX(5)  | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD6   (SDHC_XFERTYP_CMDINX(6)  | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_ACMD6  (SDHC_XFERTYP_CMDINX(6)  | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD7   (SDHC_XFERTYP_CMDINX(7)  | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_CMD8   (SDHC_XFERTYP_CMDINX(8)  | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD9   (SDHC_XFERTYP_CMDINX(9)  | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136))
#define ESDHC_CMD10  (SDHC_XFERTYP_CMDINX(10) | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136))
#define ESDHC_CMD11  (SDHC_XFERTYP_CMDINX(11) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD12  (SDHC_XFERTYP_CMDINX(12) | SDHC_XFERTYP_CMDTYP(ESDHC_XFERTYP_CMDTYP_ABORT) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_CMD13  (SDHC_XFERTYP_CMDINX(13) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_ACMD13 (SDHC_XFERTYP_CMDINX(13) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD15  (SDHC_XFERTYP_CMDINX(15) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
#define ESDHC_CMD16  (SDHC_XFERTYP_CMDINX(16) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD17  (SDHC_XFERTYP_CMDINX(17) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48) | SDHC_XFERTYP_DTDSEL_MASK | SDHC_XFERTYP_DPSEL_MASK)
#define ESDHC_CMD18  (SDHC_XFERTYP_CMDINX(18) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48) | SDHC_XFERTYP_DTDSEL_MASK | SDHC_XFERTYP_DPSEL_MASK | SDHC_XFERTYP_BCEN_MASK | SDHC_XFERTYP_MSBSEL_MASK | SDHC_XFERTYP_AC12EN_MASK)
#define ESDHC_CMD20  (SDHC_XFERTYP_CMDINX(20) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_ACMD22 (SDHC_XFERTYP_CMDINX(22) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48) | SDHC_XFERTYP_DTDSEL_MASK)
#define ESDHC_ACMD23 (SDHC_XFERTYP_CMDINX(23) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD24  (SDHC_XFERTYP_CMDINX(24) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48)|SDHC_XFERTYP_DPSEL_MASK)
#define ESDHC_CMD25  (SDHC_XFERTYP_CMDINX(25) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48) | SDHC_XFERTYP_MSBSEL_MASK | SDHC_XFERTYP_DPSEL_MASK | SDHC_XFERTYP_BCEN_MASK | SDHC_XFERTYP_AC12EN_MASK)
#define ESDHC_CMD26  (SDHC_XFERTYP_CMDINX(26) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD27  (SDHC_XFERTYP_CMDINX(27) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD28  (SDHC_XFERTYP_CMDINX(28) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_CMD29  (SDHC_XFERTYP_CMDINX(29) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_CMD30  (SDHC_XFERTYP_CMDINX(30) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD32  (SDHC_XFERTYP_CMDINX(32) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD33  (SDHC_XFERTYP_CMDINX(33) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD34  (SDHC_XFERTYP_CMDINX(34) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD35  (SDHC_XFERTYP_CMDINX(35) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD36  (SDHC_XFERTYP_CMDINX(36) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD37  (SDHC_XFERTYP_CMDINX(37) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD38  (SDHC_XFERTYP_CMDINX(38) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_CMD39  (SDHC_XFERTYP_CMDINX(39) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD40  (SDHC_XFERTYP_CMDINX(40) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_ACMD41 (SDHC_XFERTYP_CMDINX(41) | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD42  (SDHC_XFERTYP_CMDINX(42) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_ACMD42 (SDHC_XFERTYP_CMDINX(42) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_ACMD51 (SDHC_XFERTYP_CMDINX(51) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD52  (SDHC_XFERTYP_CMDINX(52) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD53  (SDHC_XFERTYP_CMDINX(53) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD55  (SDHC_XFERTYP_CMDINX(55) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48))
#define ESDHC_CMD56  (SDHC_XFERTYP_CMDINX(56) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_CMD60  (SDHC_XFERTYP_CMDINX(60) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))
#define ESDHC_CMD61  (SDHC_XFERTYP_CMDINX(61) | SDHC_XFERTYP_CICEN_MASK | SDHC_XFERTYP_CCCEN_MASK | SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_48BUSY))

//Kinetis SDHC ������λ����
#define ESDHC_PROCTL_DTW_1BIT                (0x00) //1λ���
#define ESDHC_PROCTL_DTW_4BIT                (0x01) //4λ���
#define ESDHC_PROCTL_DTW_8BIT                (0x10) //8λ���

static uint32_t SD_SendCommand(SD_CommandTypeDef* Command);

//SD����Ϣ
struct sd_card_handler
{
    uint32_t card_type;  
    uint32_t OCR;           //Operation Condition Register �����ѹ��Ϣ�� ��ʼ�����λ ���������Ĵ��� ��Ӧ��ʵ��OCR 39:8 λ
    uint32_t CID[4];        //Card IDentification (CID) register  CID[0-4] ��Ӧ ��ʵ��CID�� 127:8
    uint32_t CSD[4];        //Card-Specific Data register CSD ��¼����������Ҫ��Ϣ CSD[0-4] ��Ӧ ��ʵ��CSD 127:8
    uint16_t RCA;           //����Ե�ַ�Ĵ��� ��HOST�Ϳ�ͨѶ�Ļ���      
    uint32_t CSR[2];        //�����üĴ���
};     

static struct sd_card_handler sdh;

/**
 * @brief ����SD��ģ���ͨ���ٶ�
 * @param  baudrate  ��λHz
 * @retval None
 */                                                            
static void SD_SetBaudRate(uint32_t baudrate)
{
	uint32_t pres, div, min, minpres = 0x80, mindiv = 0x0F;
	int  val;
    uint32_t clock;
    #ifdef SIM_SOPT2_ESDHCSRC_MASK
    
    SIM->SOPT2 &= ~SIM_SOPT2_ESDHCSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_ESDHCSRC(0);
    #else
    SIM->SOPT2 &= ~SIM_SOPT2_SDHCSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_SDHCSRC(0);
    #endif
    CLOCK_GetClockFrequency(kCoreClock, &clock);
    
    /* Find closest setting */
    min = (uint32_t)-1;
    for (pres = 2; pres <= 256; pres <<= 1)
    {
        for (div = 1; div <= 16; div++)
        {
            val = pres * div * baudrate - clock;
            if (val >= 0)
            {
                if (min > val)
                {
                    min = val;
                    minpres = pres;
                    mindiv = div;
                }
            }
        }
   }
    
	/* disable SDHC */
	SDHC->SYSCTL &= (~ SDHC_SYSCTL_SDCLKEN_MASK);
   
	/* set prescaler */
	div = SDHC->SYSCTL & (~ (SDHC_SYSCTL_DTOCV_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DVS_MASK));
	SDHC->SYSCTL = div | (SDHC_SYSCTL_DTOCV(0x0E) | SDHC_SYSCTL_SDCLKFS(minpres >> 1) | SDHC_SYSCTL_DVS(mindiv - 1));
   
	/* waitting for stabile */
	while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK));
   
	/* enable module */
	SDHC->SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK;
	SDHC->IRQSTAT |= SDHC_IRQSTAT_DTOE_MASK;
} 

/**
 * @brief SDģ����ٳ�ʼ������
 * @param  baudrate  :ͨ�Ų�����
 * @retval 0:����  ����:δ��ɳ�ʼ��
 */       
uint32_t SD_QuickInit(uint32_t baudrate)
{
    SD_InitTypeDef SD_InitStruct1;
    SD_InitStruct1.baudrate = baudrate;
    /* init pinmux */
    PORT_PinMuxConfig(HW_GPIOE, 0, kPinAlt4); /* ESDHC.D1  */
    PORT_PinMuxConfig(HW_GPIOE, 1, kPinAlt4); /* ESDHC.D0  */
    PORT_PinMuxConfig(HW_GPIOE, 2, kPinAlt4); /* ESDHC.CLK */
    PORT_PinMuxConfig(HW_GPIOE, 3, kPinAlt4); /* ESDHC.CMD */
    PORT_PinMuxConfig(HW_GPIOE, 4, kPinAlt4); /* ESDHC.D3  */
    PORT_PinMuxConfig(HW_GPIOE, 5, kPinAlt4); /* ESDHC.D2  */
    

    PORT_PinPullConfig(HW_GPIOE, 0, kPullUp);
    PORT_PinPullConfig(HW_GPIOE, 1, kPullUp);
    PORT_PinPullConfig(HW_GPIOE, 2, kPullUp);
    PORT_PinPullConfig(HW_GPIOE, 3, kPullUp);
    PORT_PinPullConfig(HW_GPIOE, 4, kPullUp);
    PORT_PinPullConfig(HW_GPIOE, 5, kPullUp);
    
    SD_Init(&SD_InitStruct1);
    return SD_InitCard();
}


uint8_t SD_InitCard(void)
{
	volatile uint32_t delay_cnt = 0;
	uint8_t result;  
	uint32_t i = 0;
	uint8_t hc = 0;     
    SD_CommandTypeDef SD_CommandStruct1;
	/* initalize 80 clock */
	SDHC->SYSCTL |= SDHC_SYSCTL_INITA_MASK;
	while (SDHC->SYSCTL & SDHC_SYSCTL_INITA_MASK){}; //�ȴ���ʼ�����
        
	//--------------���¿�ʼSD����ʼ�� �����Э��---------------------------
	//��ʼSD����ʼ������ --------------------------------
	//˵�� �CCMD0 -> CMD8 -> while(CMD55+ACMD41) ->CMD2 -> CMD3 ->CMD9
	//            -> CMD7(ѡ�п�)-> CMD16(���ÿ��С)->(CMD55+ACMD6)����λ4��λ��
	//---------------------------��ʽ��ʼ------------------------------  now Let's begin !

    /* now let's begin */
	SD_CommandStruct1.COMMAND = ESDHC_CMD0;
	SD_CommandStruct1.ARGUMENT = 0;
	SD_CommandStruct1.BLOCKS = 0;
    SD_CommandStruct1.BLOCKSIZE = 512;
	result = SD_SendCommand(&SD_CommandStruct1);
	if(result != ESDHC_OK) 
    {
        LIB_TRACE("CMD0 error\r\n");
        return ESDHC_ERROR_INIT_FAILED;
    }
	//CMD8  �ж���V1.0����V2.0�Ŀ�
	SD_CommandStruct1.COMMAND = ESDHC_CMD8;
	SD_CommandStruct1.ARGUMENT =0x000001AA;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);
	if (result > 0)  //CMD8����Ӧ  ���������ͨ��
	{
		result = ESDHC_ERROR_INIT_FAILED;
	}
	if (result == 0) //SDHC ��
	{
        LIB_TRACE("SDHC detected\r\n");
		hc = true;  					
	}
    
	do 
	{								 
		for(delay_cnt=0;delay_cnt<1000;delay_cnt++);
		i++;   
		SD_CommandStruct1.COMMAND = ESDHC_CMD55;
		SD_CommandStruct1.ARGUMENT =0;
        SD_CommandStruct1.BLOCKS = 0;
        result = SD_SendCommand(&SD_CommandStruct1);
		
		SD_CommandStruct1.COMMAND = ESDHC_ACMD41;
		if(hc)
		{
			SD_CommandStruct1.ARGUMENT = 0x40300000;
		}
		else
		{
			SD_CommandStruct1.ARGUMENT = 0x00300000;
		}
		result = SD_SendCommand(&SD_CommandStruct1);
	}while ((0 == (SD_CommandStruct1.RESPONSE[0] & 0x80000000)) && (i < 300));
    if(i == 300)
    {
        LIB_TRACE("Timeout\r\n");
        return ESDHC_ERROR_INIT_FAILED;
    }
    
	//CMD2 ȡCID
	SD_CommandStruct1.COMMAND = ESDHC_CMD2;
	SD_CommandStruct1.ARGUMENT = 0;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);
	if(result != ESDHC_OK) return ESDHC_ERROR_INIT_FAILED;    
	sdh.CID[0] = SD_CommandStruct1.RESPONSE[0];
	sdh.CID[1] = SD_CommandStruct1.RESPONSE[1];
	sdh.CID[2] = SD_CommandStruct1.RESPONSE[2];
	sdh.CID[3] = SD_CommandStruct1.RESPONSE[3];
	LIB_TRACE("CID[0]:0x%X\r\n", sdh.CID[0]);
    LIB_TRACE("CID[1]:0x%X\r\n", sdh.CID[1]);
    LIB_TRACE("CID[2]:0x%X\r\n", sdh.CID[2]);
    LIB_TRACE("CID[3]:0x%X\r\n", sdh.CID[3]);
	//CMD3 ȡRCA
	SD_CommandStruct1.COMMAND = ESDHC_CMD3;
	SD_CommandStruct1.ARGUMENT = 0;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);
	if(result != ESDHC_OK) return ESDHC_ERROR_INIT_FAILED;   
    sdh.RCA = SD_CommandStruct1.RESPONSE[0]>>16;
	//CMD9 ȡCSD
	SD_CommandStruct1.COMMAND = ESDHC_CMD9;
	SD_CommandStruct1.ARGUMENT = sdh.RCA<<16;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);
	if(result != ESDHC_OK) return ESDHC_ERROR_INIT_FAILED;  
	sdh.CSD[0] = SD_CommandStruct1.RESPONSE[0];
	sdh.CSD[1] = SD_CommandStruct1.RESPONSE[1];
	sdh.CSD[2] = SD_CommandStruct1.RESPONSE[2];
	sdh.CSD[3] = SD_CommandStruct1.RESPONSE[3];
	
	//CMD7 ѡ�п�
	SD_CommandStruct1.COMMAND = ESDHC_CMD7;
	SD_CommandStruct1.ARGUMENT = sdh.RCA<<16;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);
	if(result != ESDHC_OK) return ESDHC_ERROR_INIT_FAILED;  
	//CMD16 ���ÿ��С
	SD_CommandStruct1.COMMAND = ESDHC_CMD16;
	SD_CommandStruct1.ARGUMENT = 512;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);
	if(result != ESDHC_OK) return ESDHC_ERROR_INIT_FAILED;  
	
	//CMD55 ʹ��ACMD����
	SD_CommandStruct1.COMMAND = ESDHC_CMD55;
	SD_CommandStruct1.ARGUMENT = sdh.RCA<<16;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);
	if(result != ESDHC_OK) return ESDHC_ERROR_INIT_FAILED;  
	//ACMD6 �޸�SD��ͨѶλ��
	SD_CommandStruct1.COMMAND = ESDHC_ACMD6;
	SD_CommandStruct1.ARGUMENT = 2;
	SD_CommandStruct1.BLOCKS = 0;
	result = SD_SendCommand(&SD_CommandStruct1);//�޸�SD��λ4λͨѶλ��	
	if(result != ESDHC_OK) return ESDHC_ERROR_INIT_FAILED;  
	 //���� Kinetis�� SDIOģ��λ4��ģʽ
	SDHC->PROCTL &= (~SDHC_PROCTL_DTW_MASK);
	SDHC->PROCTL |=  SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_4BIT);
	//�жϿ�����
	if((sdh.CSD[3]>>22)&0x03)
	{
        sdh.card_type = SD_CARD_TYPE_SDHC;
	}
	else
	{
        sdh.card_type = SD_CARD_TYPE_SD;
	}
    
	return ESDHC_OK;	
}

/**
 * @brief SDģ���ʼ�����ã�4�ߴ���
 * @note  �������SDģ��ĳ�ʼ�����ã��������������
 * @param  SD_InitStruct  :SDģ�����ýṹ��
 * @retval ESDHC_OK:����  ESDHC_ERROR_INIT_FAILED:δ��ɳ�ʼ��
 */   
uint8_t SD_Init(SD_InitTypeDef* SD_InitStruct)
{
    /* enable clock gate */
    #ifdef SIM_SCGC3_SDHC_MASK
	SIM->SCGC3 |= SIM_SCGC3_SDHC_MASK;
	#else
	SIM->SCGC3 |= SIM_SCGC3_ESDHC_MASK;
    #endif
    /* reset module */
	SDHC->SYSCTL = SDHC_SYSCTL_RSTA_MASK | SDHC_SYSCTL_SDCLKFS(0x80);
	while(SDHC->SYSCTL & SDHC_SYSCTL_RSTA_MASK){};

	SDHC->VENDOR = 0;
	SDHC->PROCTL = SDHC_PROCTL_EMODE(2)|SDHC_PROCTL_D3CD_MASK; 
        
    /* set watermark */
	SDHC->WML = SDHC_WML_RDWML(1) | SDHC_WML_WRWML(1);

    /* set baudrate */
	SD_SetBaudRate(SD_InitStruct->baudrate);
        
    /* clear all IT pending bit */
	SDHC->IRQSTAT = 0xFFFFFFFF;
        
	/* enable irq status */
	SDHC->IRQSTATEN = 0xFFFFFFFF;

	return ESDHC_OK;
}

/**
 * @brief ��SD����һ������
 * @note  һ����������Ϊ512�ֽ�
 * @param  sector  :Ҫ��ȡ��SD��������
 * @param  buffer  :���ݴ洢��ַ
 * @retval ESDHC_OK:����  ����:��ȡ����
 */   															  
uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer)
{
    uint16_t results;
    uint32_t	j;
    uint32_t	*p = (uint32_t*)buffer;
	SD_CommandTypeDef SD_CommandStruct1;
	if(sdh.card_type == SD_CARD_TYPE_SD)
	{
		sector = sector<<9;
	}
    
    SD_CommandStruct1.COMMAND = ESDHC_CMD17;
    SD_CommandStruct1.ARGUMENT = sector;
    SD_CommandStruct1.BLOCKS = 1;
    SD_CommandStruct1.BLOCKSIZE = 512;
    results = SD_SendCommand(&SD_CommandStruct1);
    if(results != ESDHC_OK)
    {
        return ESDHC_ERROR_DATA_TRANSFER; 
    }
    
    /* read data */
    for (j = (512+3)>>2;j!= 0;j--)
    {
        if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
        {
            SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK; 
            return 1;
        }
        
        /* waiitting for host side buffer have empty space */
        while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) {};
            
        *p++ = SDHC->DATPORT;			
    }
    return ESDHC_OK;
}

/**
 * @brief дSD����һ������
 * @param  sector  :Ҫд���SD��������
 * @param  buffer  :���ݴ洢��ַ
 * @retval ESDHC_OK:����  ����:��ȡ����
 */ 													  
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buffer)
{
	uint16_t results;
	uint32_t	j;
    uint32_t	*p = (uint32_t*)buffer;
	SD_CommandTypeDef SD_CommandStruct1;
	if(sdh.card_type == SD_CARD_TYPE_SD)
	{
		sector = sector<<9;
	}

    SD_CommandStruct1.COMMAND = ESDHC_CMD24;
    SD_CommandStruct1.ARGUMENT = sector;
    SD_CommandStruct1.BLOCKS = 1;
    SD_CommandStruct1.BLOCKSIZE = 512;
    results = SD_SendCommand(&SD_CommandStruct1);
    if(results != ESDHC_OK) 
    {
        return ESDHC_ERROR_DATA_TRANSFER; 
    }
    
    /* write data */
    for (j = (512)>>2;j!= 0;j--)
    {
        if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
        {
            SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK; 
            return 1;
        }
        
        /* waiitting for host side buffer have empty space */
        while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BWEN_MASK)){};
            
        SDHC->DATPORT = *p++;
    }
	
	return ESDHC_OK;
}

/**
 * @brief ���SD������
 * @retval SD����������λMB
 */ 
uint32_t SD_GetSizeInMB(void)
{
	uint32_t BlockBumber;  //�����
	uint32_t Muti;         //����
	uint32_t BlockLen;     //ÿ�鳤��
	uint32_t Capacity;     //����
	//�������
	if((sdh.CSD[3]>>22)&0x03)
	{
		//------------------------------------------------------------
		//CSD V2.00�汾(SDHC��)
		//���������㹫ʽ
		//memory capacity = (C_SIZE+1) * 512K byte 
		//------------------------------------------------------------
		BlockLen = (sdh.CSD[2]>>24)&0xFF;
		Capacity=((sdh.CSD[1]>>8)&0xFFFFFF)+1;
		Capacity=(Capacity+1)/2;
		return Capacity;
	}
	else
	{
		/*
		CSD V1.00�汾(��ͨSD��)
		���������㹫ʽ  BLOCKNR = (C_SIZE+1) * MULT 
		MULT = 2^(C_SIZE_MULT+2)
		BLOCK_LEN = 2^((C_SIZE_MULT < 8) )
		 ����=BLOCKNR*BLOCK_LEN
		*/
        Muti=(sdh.CSD[1]>>7)&0x7;
        Muti=2<<(Muti+1);
        //�������
        BlockBumber =((sdh.CSD[2]>>0)&0x03);
        BlockBumber=(BlockBumber<<10)+((sdh.CSD[1]>>22)&0x0FFF);
        BlockBumber++;
        BlockBumber=BlockBumber*Muti;   //�õ�����
        BlockLen = (sdh.CSD[2]>>8)&0x0F;//�õ�ÿ���С
        BlockLen = 2<<(BlockLen-1);
        Capacity=BlockBumber*BlockLen;  //�������� ��λByte
        Capacity=Capacity/1024/1024;    //��λMB	
        return Capacity;
	}
}

/**
 * @brief ���IRQSTAT�Ĵ���״̬
 * @note  �ڲ�����
 * @retval SDģ����ж�״̬
 */ 
uint32_t SD_StatusWait (uint32_t  mask)
{
    volatile uint32_t tm = 0;
    uint32_t result;
    do
    {
        tm++;
        result = SDHC->IRQSTAT & mask;
        if(tm > 500000) break;
    } 
    while (0 == result);
    return result;
}

static void SDHC_WaitCommandLineIdle(void)
{
    volatile uint32_t tm = 0;
    while (SDHC->PRSSTAT & (SDHC_PRSSTAT_CIHB_MASK | SDHC_PRSSTAT_CDIHB_MASK))
    {
        tm++;
        if(tm > 50000) break;
    }
}
/**
 * @brief ��SD����������
 * @param  Command  :SD������ṹ����
 * @retval 0:����  ����:����
 */ 	
uint32_t SD_SendCommand(SD_CommandTypeDef* Command)
{
    uint32_t xfertyp;
    uint32_t blkattr;
    xfertyp = Command->COMMAND;
    
    /* resume cmd must set DPSEL */
    if (ESDHC_XFERTYP_CMDTYP_RESUME == ((xfertyp & SDHC_XFERTYP_CMDTYP_MASK) >> SDHC_XFERTYP_CMDTYP_SHIFT))
    {
        xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
    }
    
    /* set block size and block cnt */
    blkattr = SDHC_BLKATTR_BLKSIZE(Command->BLOCKSIZE) | SDHC_BLKATTR_BLKCNT(Command->BLOCKS);
    
    if (Command->BLOCKS > 1)
    {
        xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
        xfertyp |= SDHC_XFERTYP_BCEN_MASK;
        xfertyp |= SDHC_XFERTYP_MSBSEL_MASK;
    }
    
    SDHC_WaitCommandLineIdle();
        
    /* clear status */
    SDHC->IRQSTAT |= SDHC_IRQSTAT_CIE_MASK |
                    SDHC_IRQSTAT_CEBE_MASK | 
                    SDHC_IRQSTAT_CCE_MASK | 
                    SDHC_IRQSTAT_CC_MASK | 
                    SDHC_IRQSTAT_CTOE_MASK | 
                    SDHC_IRQSTAT_CRM_MASK;
        
    /* issue cmd */
    SDHC->CMDARG = Command->ARGUMENT;
    SDHC->BLKATTR = blkattr;
    SDHC->DSADDR = 0;
    SDHC->XFERTYP = xfertyp;

    /* waitting for respond */
    if(SD_StatusWait (SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK)
    {
        SDHC->IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
        return ESDHC_ERROR_COMMAND_FAILED;
    }
    
    /* get respond data */
    if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
    {
        Command->RESPONSE[0] = SDHC->CMDRSP[0];
        Command->RESPONSE[1] = SDHC->CMDRSP[1];
        Command->RESPONSE[2] = SDHC->CMDRSP[2];
        Command->RESPONSE[3] = SDHC->CMDRSP[3];
    }
    return ESDHC_OK;
}

/**
 * @brief ��SD���Ķ������
 * @note  һ����������Ϊ512�ֽ�
 * @param  sector  :Ҫ��ȡ��SD��������
 * @param  buffer  :���ݴ洢��ַ
 * @param  count   :������ȡ����������
 * @retval ESDHC_OK:����  ����:��ȡ����
 */ 		
uint8_t SD_ReadMultiBlock(uint32_t sector, uint8_t *buf, uint16_t blockCnt)
{
	uint32_t i,j;
	uint16_t results;
    uint32_t *p = (uint32_t*)buf;
	SD_CommandTypeDef SD_CommandStruct1;
    
	if(sdh.card_type  == SD_CARD_TYPE_SD) //�������ͨSD�� �ѿ��ַת�����ֽڵ�ַ
	{
		sector = sector<<9;
	}
	SD_CommandStruct1.COMMAND = ESDHC_CMD18;
	SD_CommandStruct1.BLOCKS = blockCnt;
	SD_CommandStruct1.BLOCKSIZE = 512;
	SD_CommandStruct1.ARGUMENT = sector;
	results = SD_SendCommand(&SD_CommandStruct1);
	if(results != ESDHC_OK)
    {
        return ESDHC_ERROR_COMMAND_FAILED; 
    }
        
    /* read data */
	for(i = 0; i < blockCnt; i++)
	{
		if (((uint32_t)buf & 0x03) == 0)
		{
            for (j = (512+3)>>2;j!= 0;j--)
			{
                if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
                {
                    SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK; 
                    return ESDHC_ERROR_DATA_TRANSFER;
                }
        
                /* waiitting for host side buffer have empty space */
                while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) {};
            
                *p++ = SDHC->DATPORT;		
			}
		}
	}
	
	/* waitting for card is OK */
	do
	{
			SD_CommandStruct1.COMMAND = ESDHC_CMD13;
			SD_CommandStruct1.ARGUMENT = sdh.RCA<<16;
			SD_CommandStruct1.BLOCKS = 0;
			results = SD_SendCommand(&SD_CommandStruct1);
			if(results != ESDHC_OK)
            {
                LIB_TRACE("ESDHC_CMD13 error\r\n");
                continue;  
            }
			if (SD_CommandStruct1.RESPONSE[0] & 0xFFD98008)
			{
					blockCnt = 0; /* necessary to get real number of written blocks */
					break;
			}

	} while (0x000000900 != (SD_CommandStruct1.RESPONSE[0] & 0x00001F00));
	return ESDHC_OK;
}

/**
 * @brief дSD���Ķ������
 * @param  sector  :Ҫд���SD��������
 * @param  buffer  :���ݴ洢��ַ
 * @param  count   :����д���������
 * @retval ESDHC_OK:����  ����:��ȡ����
 */ 	
uint8_t SD_WriteMultiBlock(uint32_t sector, const uint8_t *buf, uint16_t blockCnt)
{
    uint32_t i,j;
	uint16_t results;
    uint32_t *p = (uint32_t*)buf;
	SD_CommandTypeDef SD_CommandStruct1;
    
	if(sdh.card_type  == SD_CARD_TYPE_SD)
	{
		sector = sector<<9;
	}
    
    /* issue cmd */
	SD_CommandStruct1.COMMAND = ESDHC_CMD25;
	SD_CommandStruct1.BLOCKS = blockCnt;
	SD_CommandStruct1.BLOCKSIZE = 512;
	SD_CommandStruct1.ARGUMENT = sector;
	results = SD_SendCommand(&SD_CommandStruct1);
	if(results != ESDHC_OK) 
	{
		return ESDHC_ERROR_DATA_TRANSFER;  
	}
    
    /* write data */
	for(i = 0; i < blockCnt; i++)
	{
        for (j = (512)>>2; j != 0; j--)
        {
            if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK; 
                return 1;
            }
        
            /* waiitting for host side buffer have empty space */
            while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BWEN_MASK)){};
            
            SDHC->DATPORT = *p++;
        }
	}
    
	/* waitting for card is OK */
	do
	{
			SD_CommandStruct1.COMMAND = ESDHC_CMD13;
			SD_CommandStruct1.ARGUMENT = sdh.RCA<<16;
			SD_CommandStruct1.BLOCKS = 0;
			results = SD_SendCommand(&SD_CommandStruct1);
			if(results != ESDHC_OK) 
            {
                LIB_TRACE("ESDHC_CMD13 error\r\n");
                continue;  
            }
			if (SD_CommandStruct1.RESPONSE[0] & 0xFFD98008)
			{
					blockCnt = 0; // necessary to get real number of written blocks 
					break;
			}

	} while (0x000000900 != (SD_CommandStruct1.RESPONSE[0] & 0x00001F00));
    
	return ESDHC_OK;
}

#endif

