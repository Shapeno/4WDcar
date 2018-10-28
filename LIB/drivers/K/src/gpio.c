/**
  ******************************************************************************
  * @file    gpio.c
  * @author  YANDLD
  * @version V2.5
  * @date    2014.3.24
  * @brief   ������ӹ�����www.landzo.com  ����ٷ��Ա��̳ǣ�http://landzo.taobao.com
  * @note    ���ļ�ΪоƬGPIOģ��ĵײ㹦�ܺ���
  ******************************************************************************
  */

#include "gpio.h"

/* leagacy support for Kineis Z version(inital version) */
#if (!defined(GPIO_BASES))

#ifdef PTF
#define GPIO_BASES {PTA, PTB, PTC, PTD, PTE, PTF}
#define PORT_BASES {PORTA, PORTB, PORTC, PORTD, PORTE, PORTF}
#else
#define GPIO_BASES {PTA, PTB, PTC, PTD, PTE}
#define PORT_BASES {PORTA, PORTB, PORTC, PORTD, PORTE}
#endif


#endif

/* gloabl vars */
static GPIO_Type * const GPIO_InstanceTable[] = GPIO_BASES;
static PORT_Type * const PORT_InstanceTable[] = PORT_BASES;
static GPIO_CallBackType GPIO_CallBackTable[ARRAY_SIZE(PORT_InstanceTable)] = {NULL};
static const uint32_t SIM_GPIOClockGateTable[] =
{
    SIM_SCGC5_PORTA_MASK,
    SIM_SCGC5_PORTB_MASK,
    SIM_SCGC5_PORTC_MASK,
    SIM_SCGC5_PORTD_MASK,
    SIM_SCGC5_PORTE_MASK,
};
static const IRQn_Type GPIO_IRQnTable[] = 
{
    PORTA_IRQn,
    PORTB_IRQn,
    PORTC_IRQn,
    PORTD_IRQn,
    PORTE_IRQn,
};

//! @defgroup CHKinetis
//! @{


//! @defgroup GPIO
//! @brief GPIO API functions
//! @{

 /**
 * @brief �������Ÿ��ù��� ��������ᱻ�ܶ���������ģ�������������
 * @note  ���ù��ܿɲο� Reference Manual �� Signal Multiplexing and Signal Descriptions �½� 
 * @code
 *      // ��һPORTA�˿ڵ�3���Ÿ��ó�1ģʽ.
 *      PORT_PinMuxConfig(HW_GPIOA, 3, kPinAlt1);
 * @endcode
 * @param  instance: GPIOģ��� 
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param  pinMux    :���ù���ѡ���ͬ�ĸ���ֵ����ͬ�Ĺ���
 *         @arg kPinAlt0 :���Ÿ��ó�0ģʽ
 *         @arg        . : .
 *         @arg        . : .
 *         @arg        . : .
 *         @arg kPinAlt7 :���Ÿ��ó�7ģʽ
 * @retval None
 */
void PORT_PinMuxConfig(uint32_t instance, uint8_t pin, PORT_PinMux_Type pinMux)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    PORT_InstanceTable[instance]->PCR[pin] &= ~(PORT_PCR_MUX_MASK);
    PORT_InstanceTable[instance]->PCR[pin] |=  PORT_PCR_MUX(pinMux);
}
 /**
 * @brief  ����һ�����ŵ����������蹦�� �û�һ�㲻�ص���
 * @code
 *      // ��PORTA�˿ڵ�3��������Ϊ��������(������������)
 *      PORT_PinPullConfig(HW_GPIOA, 3, kPullUp);
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param  pull      :���ŵ���ģʽѡ��
 *         @arg kPullDisabled :�رյ���������
 *         @arg kPullUp       :��������
 *         @arg kPullDown     :��������
 * @retval None
 */
void PORT_PinPullConfig(uint32_t instance, uint8_t pin, PORT_Pull_Type pull)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    switch(pull)
    {
        case kPullDisabled:
            PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_PE_MASK;
            break;
        case kPullUp:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_PE_MASK;
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_PS_MASK;
            break;
        case kPullDown:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_PE_MASK;
            PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_PS_MASK;
            break;
        default:
            break;
    }
}
/**
 * @brief  �˿����ŵĿ�©״̬���� �û�һ�㲻�ص���
 * @code
 *      // ��PORTA�˿ڵ�3��������Ϊ��©״̬ 
 *      PORT_PinOpenDrainConfig(HW_GPIOA, 3, ENABLE);
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param  newState  :���ܿ��ؿ���
 *         @arg ENABLE   :��������
 *         @arg DISABLE  :�رչ���
 * @retval None
 */
void PORT_PinOpenDrainConfig(uint32_t instance, uint8_t pin, FunctionalState newState)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    (newState == ENABLE) ? (PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_ODE_MASK):(PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_ODE_MASK);
}

/**
 * @brief  �˿����ŵĿ�����Դ�˲��� ��Ϊ����ʱ��Ч
 * @code
 *      // ��PORTA�˿ڵ�3��������Ϊ��©״̬ 
 *      PORT_PinPassiveFilterConfig(HW_GPIOA, 3, ENABLE);
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param  newState  :���ܿ��ؿ���
 *         @arg ENABLE   :��������
 *         @arg DISABLE  :�رչ���
 * @retval None
 */
void PORT_PinPassiveFilterConfig(uint32_t instance, uint8_t pin, FunctionalState newState)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    (newState == ENABLE) ? (PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_PFE_MASK):(PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_PFE_MASK);
}

 /**
 * @brief  ��������Ϊ���뻹���������  �û�һ�㲻�ص���
 * @note   ֻ�е�������ΪGPIOʱ��������
 * @code
 *      // ��PORTB�˿ڵ�3����������������
 *      GPIO_PinConfig(HW_GPIOB, 3, kInpput);
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param  mode      :��������������
 *         @arg kInpput  :���빦��ѡ��
 *         @arg kOutput  :�������ѡ��
 * @retval None
 */
void GPIO_PinConfig(uint32_t instance, uint8_t pin, GPIO_PinConfig_Type mode)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    (mode == kOutput) ? (GPIO_InstanceTable[instance]->PDDR |= (1 << pin)):(GPIO_InstanceTable[instance]->PDDR &= ~(1 << pin));
}

 /**
 * @brief  GPIO��ʼ������
 * @code
 *    //��ʼ������PORTB�˿ڵ�10����Ϊ�����������
 *    GPIO_InitTypeDef GPIO_InitStruct1;      //����һ���ṹ����
 *    GPIO_InitStruct1.instance = HW_GPIOB;   //ѡ��PORTB�˿�
 *    GPIO_InitStruct1.mode = kGPIO_Mode_OPP; //�������
 *    GPIO_InitStruct1.pinx = 10;             //ѡ��10����
 *    //���ó�ʼ��GPIO���� 
 *    GPIO_Init(&GPIO_InitStruct1);
 * @endcode
 * @param  GPIO_InitStruct: GPIO��ʼ���ṹ�壬����������״̬����  
           GPIO_InitStruct.instance   :�˿ں� HW_GPIOA ~ HW_GPIOE
 * @retval None
 */
void GPIO_Init(GPIO_InitTypeDef * GPIO_InitStruct)
{
    /* config state */
    switch(GPIO_InitStruct->mode)
    {
        case kGPIO_Mode_IFT:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPullDisabled);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kInput);
            break;
        case kGPIO_Mode_IPD:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPullDown);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kInput);
            break;
        case kGPIO_Mode_IPU:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPullUp);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kInput);
            break;
        case kGPIO_Mode_OOD:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPullUp);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, ENABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kOutput);
            break;
        case kGPIO_Mode_OPP:
            PORT_PinPullConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPullDisabled);
            PORT_PinOpenDrainConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, DISABLE);
            GPIO_PinConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kOutput);
            break;
        default:
            break;					
    }
    /* config pinMux */
    PORT_PinMuxConfig(GPIO_InitStruct->instance, GPIO_InitStruct->pinx, kPinAlt1);
}

 /**
 * @brief  ���ٳ�ʼ��һ��GPIO���� ʵ������GPIO_Init���������
 * @code
 *      //��ʼ������PORTB�˿ڵ�10����Ϊ�����������
 *      GPIO_QuickInit(HW_GPIOB, 10, kGPIO_Mode_OPP);
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param  mode  :���Ź���ģʽ
 *         @arg kGPIO_Mode_IFT :��������
 *         @arg kGPIO_Mode_IPD :��������
 *         @arg kGPIO_Mode_IPU :��������
 *         @arg kGPIO_Mode_OOD :��©���
 *         @arg kGPIO_Mode_OPP :�������
 * @retval None
 */
uint8_t GPIO_QuickInit(uint32_t instance, uint32_t pinx, GPIO_Mode_Type mode)
{
    GPIO_InitTypeDef GPIO_InitStruct1;
    GPIO_InitStruct1.instance = instance;
    GPIO_InitStruct1.mode = mode;
    GPIO_InitStruct1.pinx = pinx;
    GPIO_Init(&GPIO_InitStruct1);
    return  instance;
}

 /**
 * @brief  ����ָ����������ߵ�ƽ���ߵ͵�ƽ
 * @note   �������������ó��������
 * @code
 *      //����PORTB�˿ڵ�10��������ߵ�ƽ
 *      GPIO_WriteBit(HW_GPIOB, 10, 1);
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param  data   : ���ŵĵ�ƽ״̬  
 *         @arg 0 : �͵�ƽ 
 *         @arg 1 : �ߵ�ƽ
 * @retval None
 */
void GPIO_WriteBit(uint32_t instance, uint8_t pin, uint8_t data)
{
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    (data) ? (GPIO_InstanceTable[instance]->PSOR |= (1 << pin)):(GPIO_InstanceTable[instance]->PCOR |= (1 << pin));
}

void GPIO_SetBit(uint32_t instance, uint32_t pin)
{
    GPIO_InstanceTable[instance]->PSOR |= (1 << pin);
}

void GPIO_ResetBit(uint32_t instance, uint32_t pin)
{
    GPIO_InstanceTable[instance]->PCOR |= (1 << pin);
}

 /**
 * @brief  ��ȡһ�������ϵĵ�ƽ״̬
 * @code
 *      //��ȡPORTB�˿ڵ�10���ŵĵ�ƽ״̬
 *      uint8_t status ; //���ڴ洢���ŵ�״̬
 *      status = GPIO_ReadBit(HW_GPIOB, 10); //��ȡ���ŵ�״̬���洢��status��
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @retval 
 *         @arg 0 : �͵�ƽ
 *         @arg 1 : �ߵ�ƽ
 */
uint8_t GPIO_ReadBit(uint32_t instance, uint8_t pin)
{
    /* input or output */
    if(((GPIO_InstanceTable[instance]->PDDR) >> pin) & 0x01)
    {
        return ((GPIO_InstanceTable[instance]->PDOR >> pin) & 0x01);
    }
    else
    {
        return ((GPIO_InstanceTable[instance]->PDIR >> pin) & 0x01);
    }
}

 /**
 * @brief  ��תһ�����ŵĵ�ƽ״̬
 * @code
 *      //��תPORTB�˿ڵ�10���ŵĵ�ƽ״̬
 *      GPIO_ToggleBit(HW_GPIOB, 10); 
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @retval None
 */
void GPIO_ToggleBit(uint32_t instance, uint8_t pin)
{
    GPIO_InstanceTable[instance]->PTOR |= (1 << pin);
}

/**
 * @brief  ��ȡһ���˿�32λ������
 * @code
 *      //��ȡPORTB�˿ڵ��������ŵĵ�ƽ״̬
 *      uint32_t status ; //���ڴ洢���ŵ�״̬
 *      status = GPIO_ReadPort(HW_GPIOB); //��ȡ���ŵ�״̬���洢��status��
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @retval �˿ڵ�32λ����
 */
uint32_t GPIO_ReadPort(uint32_t instance)
{
    return (GPIO_InstanceTable[instance]->PDIR);
}
/**
 * @brief  ��һ���˿�д��32λ����
 * @code
 *      //��PORTB�˿�д��0xFFFFFFFF
 *      GPIO_WriteByte(HW_GPIOB, 0xFFFFFFFF); 
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  data  :32λ����
 * @retval None
 */
void GPIO_WritePort(uint32_t instance, uint32_t data)
{
    GPIO_InstanceTable[instance]->PDOR = data;
}


/**
 * @brief  ����GPIO�����ж����ͻ���DMA����
 * @code
 *      //����PORTB�˿ڵ�10����Ϊ�½��ش����ж�
 *      GPIO_ITDMAConfig(HW_GPIOB, 10, kGPIO_IT_FallingEdge, true); 
 * @endcode
 * @param  instance: GPIOģ���
 *         @arg HW_GPIOA :оƬ��PORTA�˿�
 *         @arg HW_GPIOB :оƬ��PORTB�˿�
 *         @arg HW_GPIOC :оƬ��PORTC�˿�
 *         @arg HW_GPIOD :оƬ��PORTD�˿�
 *         @arg HW_GPIOE :оƬ��PORTE�˿�
 * @param  pin  :�˿��ϵ����ź� 0~31
 * @param config: ����ģʽ
 *         @arg kGPIO_DMA_RisingEdge DMA�����ش���
 *         @arg kGPIO_DMA_FallingEdge DMA�½��ش���
 *         @arg kGPIO_DMA_RisingFallingEdge DMA�������½��ض�����
 *         @arg kGPIO_IT_Low �͵�ƽ�����ж�
 *         @arg kGPIO_IT_RisingEdge �����ش����ж�
 *         @arg kGPIO_IT_FallingEdge �½��ش����ж�
 *         @arg kGPIO_IT_RisingFallingEdge �������½��ض������ж�
 *         @arg kGPIO_IT_High �ߵ�ƽ�����ж�
 * @retval None
 */
void GPIO_ITDMAConfig(uint32_t instance, uint8_t pin, GPIO_ITDMAConfig_Type config, bool status)
{
    /* init moudle */
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    PORT_InstanceTable[instance]->PCR[pin] &= ~PORT_PCR_IRQC_MASK;
    
    if(!status)
    {
        NVIC_DisableIRQ(GPIO_IRQnTable[instance]);
        return;
    }
    
    switch(config)
    {
        case kGPIO_DMA_RisingEdge:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(1);
            break;
        case kGPIO_DMA_FallingEdge:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(2);
            break;
        case kGPIO_DMA_RisingFallingEdge:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(3);
            break;
        case kGPIO_IT_Low:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(8);
            NVIC_EnableIRQ(GPIO_IRQnTable[instance]);
            break;
        case kGPIO_IT_RisingEdge:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(9);
            NVIC_EnableIRQ(GPIO_IRQnTable[instance]);
            break;
        case kGPIO_IT_FallingEdge:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(10);
            NVIC_EnableIRQ(GPIO_IRQnTable[instance]);
            break;
        case kGPIO_IT_RisingFallingEdge:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(11);
            NVIC_EnableIRQ(GPIO_IRQnTable[instance]);
            break;
        case kGPIO_IT_High:
            PORT_InstanceTable[instance]->PCR[pin] |= PORT_PCR_IRQC(12);
            NVIC_EnableIRQ(GPIO_IRQnTable[instance]);
            break;
        default:
            break;
    }
}
/**
 * @brief  ע���жϻص�����
 * @param  instance: GPIOģ���ж���ں�
 *         @arg HW_GPIOA :оƬ��PORTA�˿��ж����
 *         @arg HW_GPIOB :оƬ��PORTB�˿��ж����
 *         @arg HW_GPIOC :оƬ��PORTC�˿��ж����
 *         @arg HW_GPIOD :оƬ��PORTD�˿��ж����
 *         @arg HW_GPIOE :оƬ��PORTE�˿��ж����
 * @param AppCBFun: �ص�����ָ�����
 * @retval None
 * @note ���ڴ˺����ľ���Ӧ�������Ӧ��ʵ��
 */
void GPIO_CallbackInstall(uint32_t instance, GPIO_CallBackType AppCBFun)
{
    /* init moudle */
    SIM->SCGC5 |= SIM_GPIOClockGateTable[instance];
    if(AppCBFun != NULL)
    {
        GPIO_CallBackTable[instance] = AppCBFun;
    }
}

//! @}


/**
 * @brief  �жϴ��������
 * @param  PORTA_IRQHandler :оƬ��PORTA�˿��жϺ������
 *         PORTB_IRQHandler :оƬ��PORTB�˿��жϺ������
 *         PORTC_IRQHandler :оƬ��PORTC�˿��жϺ������
 *         PORTD_IRQHandler :оƬ��PORTD�˿��жϺ������
 *         PORTE_IRQHandler :оƬ��PORTE�˿��жϺ������
 * @note �����ڲ������ж��¼�����
 */
void PORTA_IRQHandler(void)
{
    uint32_t ISFR;
    /* safe copy */
    ISFR = PORT_InstanceTable[HW_GPIOA]->ISFR;
    /* clear IT pending bit */
    PORT_InstanceTable[HW_GPIOA]->ISFR = 0xFFFFFFFF;
    if(GPIO_CallBackTable[HW_GPIOA])
    {
        GPIO_CallBackTable[HW_GPIOA](ISFR);
    }	
}

void PORTB_IRQHandler(void)
{
    uint32_t ISFR;
    /* safe copy */
    ISFR = PORT_InstanceTable[HW_GPIOB]->ISFR;
    /* clear IT pending bit */
    PORT_InstanceTable[HW_GPIOB]->ISFR = 0xFFFFFFFF;
    if(GPIO_CallBackTable[HW_GPIOB])
    {
        GPIO_CallBackTable[HW_GPIOB](ISFR);
    }	
}

void PORTC_IRQHandler(void)
{
    uint32_t ISFR;
    /* safe copy */
    ISFR = PORT_InstanceTable[HW_GPIOC]->ISFR;
    /* clear IT pending bit */
    PORT_InstanceTable[HW_GPIOC]->ISFR = PORT_ISFR_ISF_MASK;
    if(GPIO_CallBackTable[HW_GPIOC])
    {
        GPIO_CallBackTable[HW_GPIOC](ISFR);
    }
}

void PORTD_IRQHandler(void)
{
    uint32_t ISFR;
    /* safe copy */
    ISFR = PORT_InstanceTable[HW_GPIOD]->ISFR;
    /* clear IT pending bit */
    PORT_InstanceTable[HW_GPIOD]->ISFR = PORT_ISFR_ISF_MASK;
    if(GPIO_CallBackTable[HW_GPIOD])
    {
        GPIO_CallBackTable[HW_GPIOD](ISFR);
    }	
}

void PORTE_IRQHandler(void)
{
    uint32_t ISFR;
    /* safe copy */
    ISFR = PORT_InstanceTable[HW_GPIOE]->ISFR;
    /* clear IT pending bit */
    PORT_InstanceTable[HW_GPIOE]->ISFR = PORT_ISFR_ISF_MASK;
    if(GPIO_CallBackTable[HW_GPIOE])
    {
        GPIO_CallBackTable[HW_GPIOE](ISFR);
    }	
}

#if (defined(MK70F12))
void PORTF_IRQHandler(void)
{
    uint32_t ISFR;
    /* safe copy */
    ISFR = PORT_InstanceTable[HW_GPIOF]->ISFR;
    /* clear IT pending bit */
    PORT_InstanceTable[HW_GPIOF]->ISFR = PORT_ISFR_ISF_MASK;
    if(GPIO_CallBackTable[HW_GPIOF])
    {
        GPIO_CallBackTable[HW_GPIOF](ISFR);
    }
}
#endif


//! @}
