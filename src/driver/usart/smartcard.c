/*******************************************************************************
文件名：smartcard.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/09/03
简要描述：
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "smartcard.h"


u16 SC_rx_buffer[SC_RX_BUFFER_SIZE];
u16 p_SC = 0;


//------------------------------------------------------------------------------
// smartcard电源控制
//------------------------------------------------------------------------------
static void SC_power_cmd(FunctionalState NewState)
{
    if(DISABLE != NewState)
    {
        GPIO_ResetBits(SC_CMDVCC_GPIO_PORT, SC_CMDVCC_PIN);
    }
    else
    {
        GPIO_SetBits(SC_CMDVCC_GPIO_PORT, SC_CMDVCC_PIN);
    }
}
/*******************************************************************************
* 函数名称：SC_init
* 功能描述：初始化smartcard
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：
* 描述 ：
*******************************************************************************/
void SC_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(  SC_USART_CK_GPIO_CLK | SC_CMDVCC_GPIO_CLK
                           | SC_USART_TX_GPIO_CLK | SC_OFF_GPIO_CLK
                           | SC_3_5V_GPIO_CLK | SC_RESET_GPIO_CLK
                           | SC_DATADIR_GPIO_CLK | SC_A0_GPIO_CLK
                           | SC_A1_GPIO_CLK| SC_A2_GPIO_CLK, ENABLE);

    /* Enable SC_USART clock */
    SC_USART_APBPERIPHCLOCK(SC_USART_CLK, ENABLE);

    /* Configure Smartcard CMDVCC pin */
    GPIO_InitStructure.GPIO_Pin = SC_CMDVCC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SC_CMDVCC_GPIO_PORT, &GPIO_InitStructure);

    /* Configure Smartcard Reset pin */
    GPIO_InitStructure.GPIO_Pin = SC_RESET_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SC_RESET_GPIO_PORT, &GPIO_InitStructure);

    /* Configure Smartcard 3/5V pin */
    GPIO_InitStructure.GPIO_Pin = SC_3_5V_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SC_3_5V_GPIO_PORT, &GPIO_InitStructure);

    /* Configure Smartcard OFF pin */
    GPIO_InitStructure.GPIO_Pin = SC_OFF_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(SC_OFF_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART CK as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = SC_USART_CK_PIN;
    GPIO_Init(SC_USART_CK_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Tx pin as alternate function open-drain */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = SC_USART_TX_PIN;
    GPIO_Init(SC_USART_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure Smartcard A0 pin */
    GPIO_InitStructure.GPIO_Pin = SC_A0_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SC_A0_GPIO_PORT, &GPIO_InitStructure);

    /* Configure Smartcard A1 pin */
    GPIO_InitStructure.GPIO_Pin = SC_A1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SC_A1_GPIO_PORT, &GPIO_InitStructure);

    /* Configure Smartcard A2 pin */
    GPIO_InitStructure.GPIO_Pin = SC_A2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SC_A2_GPIO_PORT, &GPIO_InitStructure);

    /* Configure Smartcard DATADIR pin */
    GPIO_InitStructure.GPIO_Pin = SC_DATADIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(SC_DATADIR_GPIO_PORT, &GPIO_InitStructure);

    /* Connect PXx to SC_USART_TX */
    GPIO_PinAFConfig(SC_USART_TX_GPIO_PORT, SC_USART_TX_SOURCE, SC_USART_TX_AF);

    /* Connect PXx to SC_USART_CK */
    GPIO_PinAFConfig(SC_USART_CK_GPIO_PORT, SC_USART_CK_SOURCE, SC_USART_CK_AF);

    /* Select Smartcard 5V */
    GPIO_SetBits(SC_3_5V_GPIO_PORT, SC_3_5V_PIN);

    /* Enable SC_USART IRQ */
    NVIC_InitStructure.NVIC_IRQChannel = (u8)SC_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)SC_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)SC_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* SC_USART configuration */
    /* SC_USART configured as follow:
        - Word Length = 9 Bits
        - 1.5 Stop Bit
        - Even parity
        - BaudRate = 8064 baud
        - Hardware flow control disabled (RTS and CTS signals)
        - Tx and Rx enabled
        - USART Clock enabled
    */

    /* USART Clock set to 4.9152 MHz (PCLK2 (98.304 MHz) / 20) */
    USART_SetPrescaler(SC_USART, 0x05);

    /* USART Guard Time set to 16 Bit */
    USART_SetGuardTime(SC_USART, 11);

    USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
    USART_ClockInit(SC_USART, &USART_ClockInitStructure);

    USART_InitStructure.USART_BaudRate = 13212;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(SC_USART, &USART_InitStructure);

    /* Enable the SC_USART RXNE Interrupt */
    USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE);

    /* Enable SC_USART */
    USART_Cmd(SC_USART, ENABLE);

    /* Enable the NACK Transmission */
    USART_SmartCardNACKCmd(SC_USART, ENABLE);

    /* Enable the Smartcard Interface */
    USART_SmartCardCmd(SC_USART, ENABLE);

    /* Disable CMDVCC */
    SC_power_cmd(DISABLE);

    /* Set RSTIN LOW */
    SC_reset(Bit_RESET);
}
/*******************************************************************************
* 函数名称：SC_reset
* 功能描述：smartcard复位脚控制
* 入口参数：ResetState GPIO状态
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：
* 描述 ：
*******************************************************************************/
void SC_reset(BitAction ResetState)
{
    GPIO_WriteBit(SC_RESET_GPIO_PORT, SC_RESET_PIN, ResetState);
}
/*******************************************************************************
* 函数名称：SC_start
* 功能描述：启动smartcard
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：
* 描述 ：
*******************************************************************************/
void SC_start(void)
{
    /* Enable CMDVCC */
    SC_power_cmd(ENABLE);
}
/*******************************************************************************
* 函数名称：SC_bandary_update
* 功能描述：改变smartcard波特率
* 入口参数：bandrate 新的波特率
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：
* 描述 ：改变波特率不会引起输出时钟中断
*******************************************************************************/
void SC_bandary_update(u32 bandrate)
{
    USART_InitTypeDef USART_InitStructure;
    
    /* 更新波特率立即生效，不会对时钟产生任何影响 */
    USART_InitStructure.USART_BaudRate = bandrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(SC_USART, &USART_InitStructure);
}
/*******************************************************************************
* 函数名称：SC_write
* 功能描述：向smartcard发送数据
* 入口参数：p_data_buf 要发送的数据指针
*           len 要发送的数据长度
* 出口参数：无
* 返 回 值：TRUE/FALSE
* 全局变量：无
* 调用函数：
* 描述 ：先关闭接收中断，发送完成后开启接收中断
*******************************************************************************/
u8 SC_write(const u8 * p_data_buf, u16 len)
{
    u32 num;
    vu32 time_count;

    /* 检查指针 */
    if (NULL == p_data_buf)
    {
        return FALSE;
    }

    USART_ITConfig(SC_USART, USART_IT_RXNE, DISABLE);              // 关接收中断

    /* 循环发送p_data_buf中的数据*/
    for (num = 0; num < len; num++)
    {
        time_count = 0;
        USART_SendData(SC_USART, *(p_data_buf + num));               // 发送数据
        while (RESET == USART_GetFlagStatus(SC_USART, USART_FLAG_TXE))
        {
            if(time_count++ > 10000)                                 // 超时退出
            {
                (void)USART_ReceiveData(SC_USART);                 // 清接收中断
                USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE); // 使能接收中断
                return FALSE;
            }
        }
    }

    /* 判断最后一个字节是否发送完成 */
    time_count = 0;
    while (RESET == USART_GetFlagStatus(SC_USART, USART_FLAG_TC))
    {
        if(time_count++ > 10000)                                     // 超时退出
        {
            (void)USART_ReceiveData(SC_USART);                     // 清接收中断
            USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE);     // 使能接收中断
            return FALSE;
        }
    }

    (void)USART_ReceiveData(SC_USART);                             // 清接收中断
    USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE);             // 使能接收中断

    return TRUE;
}

