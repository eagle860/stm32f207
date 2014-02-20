/*******************************************************************************
文件名：can.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/09/12
简要描述：can总线初始化及驱动
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "can.h"


CanRxMsg RxMessage;

static void CAN1_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

    /* Connect CAN pins to AF9 */
    GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
    GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
 
    NVIC_InitStructure.NVIC_IRQChannel = (u8)CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)CAN1_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)CAN1_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* CAN register init */
    CAN_DeInit(CANx);
    CAN_StructInit(&CAN_InitStructure);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;             // 时间触发通讯模式
    CAN_InitStructure.CAN_ABOM = DISABLE;             // 自动离线管理
    CAN_InitStructure.CAN_AWUM = DISABLE;             // 自动唤醒模式
    CAN_InitStructure.CAN_NART = DISABLE;             // 非自动重传输模式
    CAN_InitStructure.CAN_RFLM = DISABLE;             // 接收FIFO锁定模式
    CAN_InitStructure.CAN_TXFP = DISABLE;             // 发送FIFO优先级
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;     // 正常模式
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
    /* Baudrate = CAN_CLK / (SYNC_SEG + BS1 + BS2) / Prescaler */
    /*  24.576/(1+8+3)/4 = 512K */
    CAN_InitStructure.CAN_BS1 = CAN_BS1_8tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 3;
    (void)CAN_Init(CANx, &CAN_InitStructure);
    
    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 1;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    /* Enable FIFO 0 message pending Interrupt */
    CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}
/*******************************************************************************
* 函数名称：CAN_init
* 功能描述：CAN初始化
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
*******************************************************************************/
void CAN_init(void)
{
    CAN1_init();
}
