#ifndef _CAN_H
#define _CAN_H


#include "public.h"


//------------------------------------------------------------------------------
// 宏定义
#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1
#define CAN_RX_PIN                 GPIO_Pin_0
#define CAN_TX_PIN                 GPIO_Pin_1
#define CAN_GPIO_PORT              GPIOD
#define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOD
#define CAN_AF_PORT                GPIO_AF_CAN1
#define CAN_RX_SOURCE              GPIO_PinSource0
#define CAN_TX_SOURCE              GPIO_PinSource1       


//------------------------------------------------------------------------------
// 全局变量
extern CanRxMsg RxMessage;


//------------------------------------------------------------------------------
// 接口函数
void CAN_init(void);


#endif //~ _CAN_H
