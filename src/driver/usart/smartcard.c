/*******************************************************************************
�ļ�����smartcard.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ���ռ�
��д���ڣ�2011/09/03
��Ҫ������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
*******************************************************************************/
#include "smartcard.h"


u16 SC_rx_buffer[SC_RX_BUFFER_SIZE];
u16 p_SC = 0;


//------------------------------------------------------------------------------
// smartcard��Դ����
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
* �������ƣ�SC_init
* ������������ʼ��smartcard
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�����
* ���� ��
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
* �������ƣ�SC_reset
* ����������smartcard��λ�ſ���
* ��ڲ�����ResetState GPIO״̬
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�����
* ���� ��
*******************************************************************************/
void SC_reset(BitAction ResetState)
{
    GPIO_WriteBit(SC_RESET_GPIO_PORT, SC_RESET_PIN, ResetState);
}
/*******************************************************************************
* �������ƣ�SC_start
* ��������������smartcard
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�����
* ���� ��
*******************************************************************************/
void SC_start(void)
{
    /* Enable CMDVCC */
    SC_power_cmd(ENABLE);
}
/*******************************************************************************
* �������ƣ�SC_bandary_update
* �����������ı�smartcard������
* ��ڲ�����bandrate �µĲ�����
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�����
* ���� ���ı䲨���ʲ����������ʱ���ж�
*******************************************************************************/
void SC_bandary_update(u32 bandrate)
{
    USART_InitTypeDef USART_InitStructure;
    
    /* ���²�����������Ч�������ʱ�Ӳ����κ�Ӱ�� */
    USART_InitStructure.USART_BaudRate = bandrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(SC_USART, &USART_InitStructure);
}
/*******************************************************************************
* �������ƣ�SC_write
* ������������smartcard��������
* ��ڲ�����p_data_buf Ҫ���͵�����ָ��
*           len Ҫ���͵����ݳ���
* ���ڲ�������
* �� �� ֵ��TRUE/FALSE
* ȫ�ֱ�������
* ���ú�����
* ���� ���ȹرս����жϣ�������ɺ��������ж�
*******************************************************************************/
u8 SC_write(const u8 * p_data_buf, u16 len)
{
    u32 num;
    vu32 time_count;

    /* ���ָ�� */
    if (NULL == p_data_buf)
    {
        return FALSE;
    }

    USART_ITConfig(SC_USART, USART_IT_RXNE, DISABLE);              // �ؽ����ж�

    /* ѭ������p_data_buf�е�����*/
    for (num = 0; num < len; num++)
    {
        time_count = 0;
        USART_SendData(SC_USART, *(p_data_buf + num));               // ��������
        while (RESET == USART_GetFlagStatus(SC_USART, USART_FLAG_TXE))
        {
            if(time_count++ > 10000)                                 // ��ʱ�˳�
            {
                (void)USART_ReceiveData(SC_USART);                 // ������ж�
                USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE); // ʹ�ܽ����ж�
                return FALSE;
            }
        }
    }

    /* �ж����һ���ֽ��Ƿ������ */
    time_count = 0;
    while (RESET == USART_GetFlagStatus(SC_USART, USART_FLAG_TC))
    {
        if(time_count++ > 10000)                                     // ��ʱ�˳�
        {
            (void)USART_ReceiveData(SC_USART);                     // ������ж�
            USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE);     // ʹ�ܽ����ж�
            return FALSE;
        }
    }

    (void)USART_ReceiveData(SC_USART);                             // ������ж�
    USART_ITConfig(SC_USART, USART_IT_RXNE, ENABLE);             // ʹ�ܽ����ж�

    return TRUE;
}

