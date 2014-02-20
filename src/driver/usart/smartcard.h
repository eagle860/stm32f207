#ifndef _SMARTCARD_H
#define _SMARTCARD_H


#include "public.h"


//------------------------------------------------------------------------------
// 宏定义
#define SC_RX_BUFFER_SIZE             272
#define SC_TIME_OUT                   10000
//#define _DEBUG      1
#ifdef _DEBUG                                                     //~ 用于开发板
    #define SC_USART                      USART6
    #define SC_USART_CLK                  RCC_APB2Periph_USART6
    #define SC_USART_APBPERIPHCLOCK       RCC_APB2PeriphClockCmd
    #define SC_USART_IRQn                 USART6_IRQn
    #define SC_USART_IRQHandler           USART6_IRQHandler
                                          
    #define SC_USART_TX_PIN               GPIO_Pin_6                
    #define SC_USART_TX_GPIO_PORT         GPIOC                       
    #define SC_USART_TX_GPIO_CLK          RCC_AHB1Periph_GPIOC
    #define SC_USART_TX_SOURCE            GPIO_PinSource6
    #define SC_USART_TX_AF                GPIO_AF_USART6
                                          
    #define SC_USART_CK_PIN               GPIO_Pin_7                
    #define SC_USART_CK_GPIO_PORT         GPIOG                    
    #define SC_USART_CK_GPIO_CLK          RCC_AHB1Periph_GPIOG
    #define SC_USART_CK_SOURCE            GPIO_PinSource7
    #define SC_USART_CK_AF                GPIO_AF_USART6
                                          
    #define SC_3_5V_PIN                   GPIO_Pin_15
    #define SC_3_5V_GPIO_PORT             GPIOH
    #define SC_3_5V_GPIO_CLK              RCC_AHB1Periph_GPIOH
                                          
    #define SC_RESET_PIN                  GPIO_Pin_7
    #define SC_RESET_GPIO_PORT            GPIOF
    #define SC_RESET_GPIO_CLK             RCC_AHB1Periph_GPIOF
                                          
    #define SC_CMDVCC_PIN                 GPIO_Pin_12
    #define SC_CMDVCC_GPIO_PORT           GPIOG
    #define SC_CMDVCC_GPIO_CLK            RCC_AHB1Periph_GPIOG
                                          
    #define SC_OFF_PIN                    GPIO_Pin_6
    #define SC_OFF_GPIO_PORT              GPIOF
    #define SC_OFF_GPIO_CLK               RCC_AHB1Periph_GPIOF
    #define SC_OFF_EXTI_LINE              EXTI_Line6
    #define SC_OFF_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOF
    #define SC_OFF_EXTI_PIN_SOURCE        EXTI_PinSource6
    #define SC_OFF_EXTI_IRQn              EXTI9_5_IRQn 
    #define SC_OFF_EXTI_IRQHandler        EXTI9_5_IRQHandler
#else // not define _DEBUG
    #define SC_USART                      USART6
    #define SC_USART_CLK                  RCC_APB2Periph_USART6
    #define SC_USART_APBPERIPHCLOCK       RCC_APB2PeriphClockCmd
    #define SC_USART_IRQn                 USART6_IRQn
    #define SC_USART_IRQHandler           USART6_IRQHandler
    
    #define SC_USART_TX_PIN               GPIO_Pin_6                
    #define SC_USART_TX_GPIO_PORT         GPIOC                       
    #define SC_USART_TX_GPIO_CLK          RCC_AHB1Periph_GPIOC
    #define SC_USART_TX_SOURCE            GPIO_PinSource6
    #define SC_USART_TX_AF                GPIO_AF_USART6
    
    #define SC_USART_CK_PIN               GPIO_Pin_8                
    #define SC_USART_CK_GPIO_PORT         GPIOC                    
    #define SC_USART_CK_GPIO_CLK          RCC_AHB1Periph_GPIOC
    #define SC_USART_CK_SOURCE            GPIO_PinSource8
    #define SC_USART_CK_AF                GPIO_AF_USART6
    
    #define SC_3_5V_PIN                   GPIO_Pin_2
    #define SC_3_5V_GPIO_PORT             GPIOE
    #define SC_3_5V_GPIO_CLK              RCC_AHB1Periph_GPIOE
    
    #define SC_RESET_PIN                  GPIO_Pin_1
    #define SC_RESET_GPIO_PORT            GPIOE
    #define SC_RESET_GPIO_CLK             RCC_AHB1Periph_GPIOE
    
    #define SC_CMDVCC_PIN                 GPIO_Pin_3
    #define SC_CMDVCC_GPIO_PORT           GPIOE
    #define SC_CMDVCC_GPIO_CLK            RCC_AHB1Periph_GPIOE
    
    #define SC_OFF_PIN                    GPIO_Pin_0
    #define SC_OFF_GPIO_PORT              GPIOE
    #define SC_OFF_GPIO_CLK               RCC_AHB1Periph_GPIOE
    #define SC_OFF_EXTI_LINE              EXTI_Line0
    #define SC_OFF_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOE
    #define SC_OFF_EXTI_PIN_SOURCE        EXTI_PinSource0
    #define SC_OFF_EXTI_IRQn              EXTI0_IRQn 
    #define SC_OFF_EXTI_IRQHandler        EXTI0_IRQHandler
#endif //~ _DEBUG

#define SC_A0_PIN                    GPIO_Pin_2
#define SC_A0_GPIO_PORT              GPIOF
#define SC_A0_GPIO_CLK               RCC_AHB1Periph_GPIOF

#define SC_A1_PIN                    GPIO_Pin_3
#define SC_A1_GPIO_PORT              GPIOF
#define SC_A1_GPIO_CLK               RCC_AHB1Periph_GPIOF

#define SC_A2_PIN                    GPIO_Pin_4
#define SC_A2_GPIO_PORT              GPIOF
#define SC_A2_GPIO_CLK               RCC_AHB1Periph_GPIOF

#define SC_DATADIR_PIN               GPIO_Pin_5
#define SC_DATADIR_GPIO_PORT         GPIOF
#define SC_DATADIR_GPIO_CLK          RCC_AHB1Periph_GPIOF



//------------------------------------------------------------------------------
// 全局变量声明
extern u16 SC_rx_buffer[SC_RX_BUFFER_SIZE];
extern u16 p_SC;


//------------------------------------------------------------------------------
// 接口函数
void SC_init(void);
void SC_start(void);
void SC_reset(BitAction ResetState);
void SC_bandary_update(u32 bandrate);
u8 SC_write(const u8 * p_data_buf,u16 len);


#endif //~ _SMARTCARD_H
