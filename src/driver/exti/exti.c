/*********************************************
�ļ�����exti.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/08/30
��Ҫ������external interrupt��ʼ��������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/
#include "exti.h"

/*extern clock initial*/
static void exti_clock(void);

/*extern interrupt settings function*/
static void pe12_exti(void);

static void pe13_exti(void);

static void pe14_exti(void);

static void pe15_exti(void);


/******************************************
* �������ƣ��ⲿ�ж�PortE_pin12,PortE_pin13,
			PortE_pin14,PortE_pin15��ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void EXTI_init(void)
{
	exti_clock();

	pe12_exti();
	
	pe13_exti();
	
	pe14_exti();
	
	pe15_exti();
}

/******************************************
* �������ƣ��ⲿ�ж�PortE_pin12,PortE_pin13,
			PortE_pin14,PortE_pin15ʱ�ӳ�ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void exti_clock(void)
{
	/* Enable GPIOA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}

/******************************************
* �������ƣ��ⲿ�ж�PortE_pin12��ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void pe12_exti(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Configure PE12 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Connect EXTI Line12 to PE12 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);

	/* Configure EXTI Line12 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line12 Interrupt to the lowest priority */
	EXTI_ClearITPendingBit(EXTI_Line12);	
	NVIC_InitStructure.NVIC_IRQChannel = (u8) EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)EXTI15_10_PRI >> 4) & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)EXTI15_10_PRI & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/******************************************
* �������ƣ��ⲿ�ж�PortE_pin13��ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void pe13_exti(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Configure PE13 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Connect EXTI Line13 to PE13 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);
	
	/* Configure EXTI Line13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line13 Interrupt to the lowest priority */
	EXTI_ClearITPendingBit(EXTI_Line13);
	NVIC_InitStructure.NVIC_IRQChannel = (u8) EXTI15_10_IRQn;			//EXTI1_IRQn = 7,IRQn_Type
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)EXTI15_10_PRI >> 4) & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)EXTI15_10_PRI & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/******************************************
* �������ƣ��ⲿ�ж�PortE_pin14��ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void pe14_exti(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Configure PE14 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Connect EXTI Line14 to PE14 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource14);
	
	/* Configure EXTI Line14 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line14 Interrupt to the lowest priority */
	EXTI_ClearITPendingBit(EXTI_Line14);
	NVIC_InitStructure.NVIC_IRQChannel = (u8) EXTI15_10_IRQn;			//EXTI2_IRQHandler = 8,IRQn_Type
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)EXTI15_10_PRI >> 4) & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)EXTI15_10_PRI & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/******************************************
* �������ƣ��ⲿ�ж�PortE_pin15��ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void pe15_exti(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Configure PE15 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	/* Connect EXTI Line15 to PE15 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource15);
	
	/* Configure EXTI Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line15 Interrupt to the lowest priority */
	EXTI_ClearITPendingBit(EXTI_Line15);
	NVIC_InitStructure.NVIC_IRQChannel = (u8) EXTI15_10_IRQn;			//EXTI3_IRQn = 9,IRQn_Type
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)EXTI15_10_PRI >> 4) & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)EXTI15_10_PRI & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

