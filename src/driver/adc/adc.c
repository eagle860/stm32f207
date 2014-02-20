/*********************************************
�ļ�����adc.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/08/30
��Ҫ������adc��ʼ��������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/

#include "adc.h"


/******************************************
* �������ƣ�ADC3,channel 7�ĳ�ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void ADC_init(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/*Enable adc pheriphery clock*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	/*Enable adc GPIO clock*/
	RCC_AHB1PeriphClockCmd(ADC3_GPIO_CLK, ENABLE);

	/* Configure ADC3 Channel7 pin as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = ADC3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* ADC3 Init ****************************************************************/
	/*adc 12-bit*/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	/*call adc intial function*/
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/*adc mode config*/
	ADC_RegularChannelConfig(ADC3,ADC_Channel_7,1,ADC_SampleTime_3Cycles);
	ADC_DiscModeChannelCountConfig(ADC3,1);
	/*end of convert flag enable*/
	ADC_EOCOnEachRegularChannelCmd(ADC3,ENABLE);
	/*enable adc discontinous mode*/
	ADC_DiscModeCmd(ADC3,ENABLE);
	/*enable adc3*/
	ADC_Cmd(ADC3,ENABLE);
}


/******************************************
* �������ƣ�ADC3,channel 7��ת��ֵ��ȡ����
* ������������ȡadcת��ֵ
* ��ڲ�������
* ���ڲ�����adcת������洢ֵ��ַ
* �� �� ֵ��ת���ɹ�����ture
            ��ʱ����false
* ȫ�ֱ�������
* ���ú�������
* ���� ����
*******************************************/
u8 get_adc_value(u16 * p_data)
{
	u16 time_out;
	time_out = 1000;

	if(NULL == p_data)
	{
		return FALSE;
	}
	/*start the conversion*/
	ADC_SoftwareStartConv(ADC3);
	while((RESET == ADC_GetFlagStatus(ADC3,ADC_FLAG_EOC)) && time_out > 0)
	{
		time_out --;
	}

	if(time_out == 0)
		return FALSE;

	*p_data = ADC_GetConversionValue(ADC3);

	return TRUE;
}
