/*********************************************
�ļ�����adc.h
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/08/30
��Ҫ������adc�ⲿ�ӿں�������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/
#ifndef _ADC_H
#define _ADC_H

#include "public.h"

#define  ADC3_GPIO_CLK    RCC_AHB1Periph_GPIOF 
#define  ADC3_GPIO_PORT   GPIOF
#define  ADC3_GPIO_PIN    GPIO_Pin_9

/*adc initial function*/
void ADC_init(void);

/*get the value of adc convert*/
u8 get_adc_value(u16 * p_data);

#endif
