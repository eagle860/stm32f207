/*******************************************************************************
文件名：tim.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/08/30
简要描述：定时器初始化
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "tim.h"


u16 TIM3_capture_buf[TIM3_CAPTURE_SIZE];
u16 TIM3_captured = 0;

u16 TIM4_capture_buf[TIM4_CAPTURE_SIZE];
u16 TIM4_captured = 0;

//------------------------------------------------------------------------------
// 初始化TIM3
//------------------------------------------------------------------------------
static void TIM3_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    /* 使能TIM3时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(TIM3_GPIO_CLK, ENABLE);

    /* 配置TIM3捕获IO口 */
    GPIO_InitStructure.GPIO_Pin =  TIM3_CAPTURE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(TIM3_CAPTURE_GPIO_PORT, &GPIO_InitStructure);
    /* Connect TIM3 pins to AF2 */
    GPIO_PinAFConfig(TIM3_CAPTURE_GPIO_PORT, TIM3_SOURCE, GPIO_AF_TIM3);

    /* 配置TIM3中断 */
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

    /* 配置定时器3设置.通道3上升沿捕获 */
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
// 初始化TIM4
//------------------------------------------------------------------------------
static void TIM4_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    /* 使能TIM4时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(TIM4_GPIO_CLK, ENABLE);

    /* 配置TIM4捕获IO口 */
    GPIO_InitStructure.GPIO_Pin =  TIM4_CAPTURE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(TIM4_CAPTURE_GPIO_PORT, &GPIO_InitStructure);
    /* Connect TIM3 pins to AF2 */
    GPIO_PinAFConfig(TIM4_CAPTURE_GPIO_PORT, TIM4_SOURCE, GPIO_AF_TIM4);

    /* 配置TIM4中断 */
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

    /* 配置定时器4设置.通道4上升沿捕获 */
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
// 初始化TIM6
//------------------------------------------------------------------------------
static void TIM6_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* 使能TIM6时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* 配置TIM6中断 */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)TIM6_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)TIM6_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置TIM6为1ms定时器 */
    TIM_DeInit(TIM6);
    TIM_TimeBaseStructure.TIM_Prescaler = 2 - 1;                        // 2分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;      // 向上计数
    TIM_TimeBaseStructure.TIM_Period = 24 * 1024 - 1;                // 计数周期
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;                  // 时钟分割
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);                   // 使能TIM6中断
    TIM_Cmd(TIM6, ENABLE);                                           // 使能TIM6
}
//------------------------------------------------------------------------------
// 初始化TIM7
//------------------------------------------------------------------------------
static void TIM7_init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* 使能TIM7时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    /* 配置TIM7中断 */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)TIM7_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)TIM7_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置TIM7为10ms定时器 */
    TIM_DeInit(TIM7);
    TIM_TimeBaseStructure.TIM_Prescaler = 20 - 1;                      // 20分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;      // 向上计数
    TIM_TimeBaseStructure.TIM_Period = 24 * 1024 - 1;                // 计数周期
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;                  // 时钟分割
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);                   // 使能TIM7中断
    TIM_Cmd(TIM7, ENABLE);                                           // 使能TIM7
}
/*******************************************************************************
* 函数名称：TIM_init
* 功能描述：初始化TIM3,4,6,7
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
* 描述 ：
*******************************************************************************/
void TIM_init(void)
{
    TIM3_init();
    TIM4_init();
    TIM6_init();
    TIM7_init();
}
/*******************************************************************************
* 函数名称：start_TIM3_capture
* 功能描述：启动TIM3捕获
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：TIM3_capture_buf
* 调用函数：无
* 描述 ：
*******************************************************************************/
void start_TIM3_capture(void)
{
    TIM3_captured = 0;                                         // 捕获buffer归零
    TIM_ITConfig(TIM3, TIM3_IT_SOURCE, ENABLE);         // 启动TIM3通道2捕获中断
}
/*******************************************************************************
* 函数名称：start_TIM4_capture
* 功能描述：启动TIM4捕获
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：TIM4_capture_buf
* 调用函数：无
* 描述 ：
*******************************************************************************/
void start_TIM4_capture(void)
{
    TIM4_captured = 0;                                         // 捕获buffer归零
    TIM_ITConfig(TIM4, TIM4_IT_SOURCE, ENABLE);         // 启动TIM3通道2捕获中断
}
/*******************************************************************************
* 函数名称：stop_TIM3_capture
* 功能描述：停止TIM3捕获
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
* 描述 ：
*******************************************************************************/
void stop_TIM3_capture(void)
{
    TIM_ITConfig(TIM3, TIM3_IT_SOURCE, DISABLE);        // 关闭TIM3通道2捕获中断
}
/*******************************************************************************
* 函数名称：stop_TIM4_capture
* 功能描述：停止TIM4捕获
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
* 描述 ：
*******************************************************************************/
void stop_TIM4_capture(void)
{
    TIM_ITConfig(TIM4, TIM4_IT_SOURCE, DISABLE);        // 关闭TIM4通道2捕获中断
}
