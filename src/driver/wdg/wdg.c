/*******************************************************************************
�ļ�����wdg.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ���ռ�
��д���ڣ�2011/10/24
��Ҫ���������Ź���ʼ��
�޸��ˣ�
�޸�����:
�޸ļ�¼��
*******************************************************************************/
#include "wdg.h"


static void IWDG_init(void)
{
    /* Enable the LSI oscillator ************************************************/
    RCC_LSICmd(ENABLE);
    
    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {}

    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    
    /* IWDG counter clock */
    IWDG_SetPrescaler(IWDG_PRESCALER);
    
    /* Set counter reload value to obtain 250ms IWDG TimeOut. */
    IWDG_SetReload(IWDG_RELOAD_TIM);
    
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}
/*******************************************************************************
* �������ƣ�CAN_init
* ����������CAN��ʼ��
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�������
*******************************************************************************/
void WDG_init(void)
{
    IWDG_init();
}
