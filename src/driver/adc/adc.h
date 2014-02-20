/*********************************************
文件名：adc.h
版本号：v1.0
版权说明：
作者：蔡和
编写日期：2011/08/30
简要描述：adc外部接口函数声明
修改人：
修改日期:
修改记录：
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
