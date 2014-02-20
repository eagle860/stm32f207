#ifndef _TIM_H
#define _TIM_H


#include "public.h"


//------------------------------------------------------------------------------
// �궨��
#define TIM3_CAPTURE_SIZE       2048
#define TIM4_CAPTURE_SIZE       2048

// ����98.304MHz����Ƶ����Ƶϵ��Ϊ0�������ź�Ϊ512KHzʱ������ֵΪ3220
#define TIM3_Prescaler          3                           // ����ͨ��3��Ƶϵ��
#define TIM4_Prescaler          3                           // ����ͨ��4��Ƶϵ��

// �ܽ�ӳ��ο�docĿ¼�µ�STM32F207VE.pdf
#define TIM3_CAPTURE_PIN        GPIO_Pin_7
#define TIM3_CAPTURE_GPIO_PORT  GPIOC
#define TIM3_SOURCE             GPIO_PinSource7
#define TIM3_GPIO_CLK           RCC_AHB1Periph_GPIOC

#define TIM3_CHANNEL            TIM_Channel_2
#define TIM3_IT_SOURCE          TIM_IT_CC2
#define TIM3_GetCapture         TIM_GetCapture2

#define TIM4_CAPTURE_PIN        GPIO_Pin_7
#define TIM4_CAPTURE_GPIO_PORT  GPIOB
#define TIM4_SOURCE             GPIO_PinSource7
#define TIM4_GPIO_CLK           RCC_AHB1Periph_GPIOB

#define TIM4_CHANNEL            TIM_Channel_2
#define TIM4_IT_SOURCE          TIM_IT_CC2
#define TIM4_GetCapture         TIM_GetCapture2


//------------------------------------------------------------------------------
// ȫ�ֱ���
extern u16 TIM3_capture_buf[TIM3_CAPTURE_SIZE];
extern u16 TIM3_captured;

extern u16 TIM4_capture_buf[TIM4_CAPTURE_SIZE];
extern u16 TIM4_captured;


//------------------------------------------------------------------------------
// �ӿں���
void TIM_init(void);
void start_TIM3_capture(void);
void start_TIM4_capture(void);
void stop_TIM3_capture(void);
void stop_TIM4_capture(void);


#endif //~ _TIM_H
