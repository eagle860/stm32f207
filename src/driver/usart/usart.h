/*********************************************
文件名：usart.h
版本号：v1.0
版权说明：
作者：蔡和
编写日期：2011/08/30
简要描述：usart外部接口函数声明及宏定义
修改人：
修改日期:
修改记录：
********************************************/
#ifndef _SERIAL_H
#define _SERIAL_H
#include "public.h"


/* Definition for USARTx resources ********************************************/
/*串口1的配置参数*/
#define   COM1_USART1                   USART1

#define   COM1_CLK                      RCC_APB2Periph_USART1
#define   COM1_TX_PIN                   GPIO_Pin_9
#define   COM1_TX_GPIO_PORT             GPIOA
#define   COM1_TX_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define   COM1_TX_SOURCE                GPIO_PinSource9
#define   COM1_TX_AF                    GPIO_AF_USART1

#define   COM1_RX_PIN                   GPIO_Pin_10
#define   COM1_RX_GPIO_PORT             GPIOA
#define   COM1_RX_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define   COM1_RX_SOURCE                GPIO_PinSource10
#define   COM1_RX_AF                    GPIO_AF_USART1

#define   USART1_BaudRate               115200
#define   USART1_WordLength             USART_WordLength_8b
#define   USART1_StopBits               USART_StopBits_1
#define   USART1_Parity                 USART_Parity_No
#define   USART1_HardwareFlowControl    USART_HardwareFlowControl_None
#define   USART1_Mode                   (USART_Mode_Rx | USART_Mode_Tx)

/*串口3的配置参数*/
#define   COM3_USART3                   USART3

#define   COM3_CLK                      RCC_APB1Periph_USART3
#define   COM3_TX_PIN                   GPIO_Pin_8
#define   COM3_TX_GPIO_PORT             GPIOD
#define   COM3_TX_GPIO_CLK              RCC_AHB1Periph_GPIOD
#define   COM3_TX_SOURCE                GPIO_PinSource8
#define   COM3_TX_AF                    GPIO_AF_USART3

#define   COM3_RX_PIN                   GPIO_Pin_9
#define   COM3_RX_GPIO_PORT             GPIOD
#define   COM3_RX_GPIO_CLK              RCC_AHB1Periph_GPIOD
#define   COM3_RX_SOURCE                GPIO_PinSource9
#define   COM3_RX_AF                    GPIO_AF_USART3

#define   USART3_BaudRate               115200
#define   USART3_WordLength             USART_WordLength_8b
#define   USART3_StopBits               USART_StopBits_1
#define   USART3_Parity                 USART_Parity_No
#define   USART3_HardwareFlowControl    USART_HardwareFlowControl_None
#define   USART3_Mode                   (USART_Mode_Rx | USART_Mode_Tx)

/*串口4的配置参数*/
#define   COM4_USART4                   UART4

#define   COM4_CLK                      RCC_APB1Periph_UART4
#define   COM4_TX_PIN                   GPIO_Pin_10
#define   COM4_TX_GPIO_PORT             GPIOC
#define   COM4_TX_GPIO_CLK              RCC_AHB1Periph_GPIOC
#define   COM4_TX_SOURCE                GPIO_PinSource10
#define   COM4_TX_AF                    GPIO_AF_UART4

#define   COM4_RX_PIN                   GPIO_Pin_11
#define   COM4_RX_GPIO_PORT             GPIOC
#define   COM4_RX_GPIO_CLK              RCC_AHB1Periph_GPIOC
#define   COM4_RX_SOURCE                GPIO_PinSource11
#define   COM4_RX_AF                    GPIO_AF_UART4


#define   USART4_BaudRate               115200
#define   USART4_WordLength             USART_WordLength_8b
#define   USART4_StopBits               USART_StopBits_1
#define   USART4_Parity                 USART_Parity_No
#define   USART4_HardwareFlowControl    USART_HardwareFlowControl_None
#define   USART4_Mode                   (USART_Mode_Rx | USART_Mode_Tx)

/*串口相关全局变量：开始*/
/*these parameters should not be called by*/
/*other function,except the interrupt*/
extern u16 USART1_complete_number;
extern u16 USART1_data_number;

extern u16 USART3_complete_number;
extern u16 USART3_data_number;

extern u16 USART4_complete_number;
extern u16 USART4_data_number;

extern u8 *p_USART1_TX_buf;
extern u8 *p_USART3_TX_buf;
extern u8 *p_USART4_TX_buf;

extern u8 USART1_complete_flag;
extern u8 USART3_complete_flag;
extern u8 USART4_complete_flag;


/*usart1, uasrt2, usart3 initial function*/
void USART_init(void);

/*COM1发送函数*/
u8 uart1_write(u8 *pdata,u16 data_len);

/*COM3发送函数*/
u8 uart3_write(u8 *pdata,u16 data_len);

/*COM4发送函数*/
u8 uart4_write(u8 *pdata,u16 data_len);

#endif

