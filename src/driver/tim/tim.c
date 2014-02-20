/*******************************************************************************
�ļ�����tim.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ���ռ�
��д���ڣ�2011/08/30
��Ҫ��������ʱ����ʼ��
�޸��ˣ�
�޸�����:
�޸ļ�¼��
*******************************************************************************/
#include "tim.h"


u16 TIM3_capture_buf[TIM3_CAPTURE_SIZE];
u16 TIM3_captured = 0;

u16 TIM4_capture_buf[TIM4_CAPTURE_SIZE];
u16 TIM4_captured = 0;

//------------------------------------------------------------------------------
// ��ʼ��TIM3
//------------------------------------------------------------------------------
static void TIM3_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    /* ʹ��TIM3ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(TIM3_GPIO_CLK, ENABLE);

    /* ����TIM3����IO�� */
    GPIO_InitStructure.GPIO_Pin =  TIM3_CAPTURE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(TIM3_CAPTURE_GPIO_PORT, &GPIO_InitStructure);
    /* Connect TIM3 pins to AF2 */
    GPIO_PinAFConfig(TIM3_CAPTURE_GPIO_PORT, TIM3_SOURCE, GPIO_AF_TIM3);

    /* ����TIM3�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)TIM3_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)TIM3_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_DeInit(TIM3);

    TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* ���ö�ʱ��3����.ͨ��3�����ز��� */
    TIM_ICInitStructure.TIM_Channel = TIM3_CHANNEL;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    //TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI2FP2);
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
    /* TIM enable counter */
    TIM_Cmd(TIM3, ENABLE);
    /* Enable the CC3 Interrupt Request */
    //TIM_ITConfig(TIM3, TIM3_IT_SOURCE, ENABLE);
}
//------------------------------------------------------------------------------
// ��ʼ��TIM4
//------------------------------------------------------------------------------
static void TIM4_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    /* ʹ��TIM4ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(TIM4_GPIO_CLK, ENABLE);

    /* ����TIM4����IO�� */
    GPIO_InitStructure.GPIO_Pin =  TIM4_CAPTURE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(TIM4_CAPTURE_GPIO_PORT, &GPIO_InitStructure);
    /* Connect TIM3 pins to AF2 */
    GPIO_PinAFConfig(TIM4_CAPTURE_GPIO_PORT, TIM4_SOURCE, GPIO_AF_TIM4);

    /* ����TIM4�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)TIM4_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)TIM4_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_DeInit(TIM4);

    TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* ���ö�ʱ��4����.ͨ��4�����ز��� */
    TIM_ICInitStructure.TIM_Channel = TIM4_CHANNEL;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    //TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
    TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);
    TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
    /* TIM enable counter */
    TIM_Cmd(TIM4, ENABLE);
    /* Enable the CC3 Interrupt Request */
    //TIM_ITConfig(TIM4, TIM4_IT_SOURCE, ENABLE);
}
//------------------------------------------------------------------------------
// ��ʼ��TIM6
//------------------------------------------------------------------------------
static void TIM6_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* ʹ��TIM6ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* ����TIM6�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)TIM6_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)TIM6_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* ����TIM6Ϊ1ms��ʱ�� */
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.TIM_Prescaler = 2 - 1;                        // 2��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;      // ���ϼ���
    TIM_TimeBaseStructure.TIM_Period = 24 * 1024 - 1;                // ��������
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;                  // ʱ�ӷָ�
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);                   // ʹ��TIM6�ж�
    TIM_Cmd(TIM6, ENABLE);                                           // ʹ��TIM6
}
//------------------------------------------------------------------------------
// ��ʼ��TIM7
//------------------------------------------------------------------------------
static void TIM7_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* ʹ��TIM7ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    /* ����TIM7�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)TIM7_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)TIM7_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* ����TIM7Ϊ10ms��ʱ�� */
    TIM_DeInit(TIM7);
    TIM_TimeBaseStructure.TIM_Prescaler = 20 - 1;                      // 20��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;      // ���ϼ���
    TIM_TimeBaseStructure.TIM_Period = 24 * 1024 - 1;                // ��������
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;                  // ʱ�ӷָ�
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);                   // ʹ��TIM7�ж�
    TIM_Cmd(TIM7, ENABLE);                                           // ʹ��TIM7
}
/*******************************************************************************
* �������ƣ�TIM_init
* ������������ʼ��TIM3,4,6,7
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�������
* ���� ��
*******************************************************************************/
void TIM_init(void)
{
    TIM3_init();
    TIM4_init();
    TIM6_init();
    TIM7_init();
}
/*******************************************************************************
* �������ƣ�start_TIM3_capture
* ��������������TIM3����
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�����TIM3_capture_buf
* ���ú�������
* ���� ��
*******************************************************************************/
void start_TIM3_capture(void)
{
    TIM3_captured = 0;                                         // ����buffer����
    TIM_ITConfig(TIM3, TIM3_IT_SOURCE, ENABLE);         // ����TIM3ͨ��2�����ж�
}
/*******************************************************************************
* �������ƣ�start_TIM4_capture
* ��������������TIM4����
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�����TIM4_capture_buf
* ���ú�������
* ���� ��
*******************************************************************************/
void start_TIM4_capture(void)
{
    TIM4_captured = 0;                                         // ����buffer����
    TIM_ITConfig(TIM4, TIM4_IT_SOURCE, ENABLE);         // ����TIM3ͨ��2�����ж�
}
/*******************************************************************************
* �������ƣ�stop_TIM3_capture
* ����������ֹͣTIM3����
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�������
* ���� ��
*******************************************************************************/
void stop_TIM3_capture(void)
{
    TIM_ITConfig(TIM3, TIM3_IT_SOURCE, DISABLE);        // �ر�TIM3ͨ��2�����ж�
}
/*******************************************************************************
* �������ƣ�stop_TIM4_capture
* ����������ֹͣTIM4����
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�������
* ���� ��
*******************************************************************************/
void stop_TIM4_capture(void)
{
    TIM_ITConfig(TIM4, TIM4_IT_SOURCE, DISABLE);        // �ر�TIM4ͨ��2�����ж�
}
