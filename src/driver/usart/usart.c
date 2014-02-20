/*********************************************
�ļ�����usart.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/08/30
��Ҫ������usart��ʼ��������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/
#include "usart.h"

/*����ȫ�ֱ���********************************************************/
/*����1��3��4���ա��������ݵ�����ָ��*/
u8 *p_USART1_TX_buf = NULL;
u8 *p_USART3_TX_buf = NULL;
u8 *p_USART4_TX_buf = NULL;

/*��Ҫ�������ջ��͵����ݳ���,*/
/*�ѽ��ջ��͵�������*/
u16 USART1_data_number = 0;
u16 USART1_complete_number = 0;
u8  USART1_complete_flag = 1;

u16 USART3_data_number = 0;
u16 USART3_complete_number = 0;
u8  USART3_complete_flag = 1;

u16 USART4_data_number = 0;
u16 USART4_complete_number = 0;
u8  USART4_complete_flag = 1;

/*COM1��ʼ����غ���*/
static void COM1_init(void);
static void COM1_config(void);
static void NVIC_COM1_config(void);

/*COM3��ʼ����غ���*/
static void COM3_init(void); 
static void COM3_config(void);
static void NVIC_COM3_config(void);

/*COM4��ʼ����غ���*/
static void COM4_init(void);  
static void COM4_config(void);
static void NVIC_COM4_config(void);


/******************************************
* �������ƣ����ڳ�ʼ������
* �������������ڳ�ʼ��
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�����
* ���� �����ڳ�ʼ�����������а����˴���1��3��
   		 4�ĳ�ʼ��
*******************************************/
void USART_init(void)
{
	COM1_init();
	COM3_init();
	COM4_init();		
}

/******************************************
* �������ƣ�����1��ʼ������
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void COM1_init(void)
{
	NVIC_COM1_config();

	COM1_config();
}

/******************************************
* �������ƣ�����1�ĳ�ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void COM1_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate            = USART1_BaudRate;
	USART_InitStructure.USART_WordLength          = USART1_WordLength;
	USART_InitStructure.USART_StopBits   		  = USART1_StopBits;
	USART_InitStructure.USART_Parity     		  = USART1_Parity;
	USART_InitStructure.USART_HardwareFlowControl = USART1_HardwareFlowControl;
	USART_InitStructure.USART_Mode                = USART1_Mode;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(COM1_TX_GPIO_CLK | COM1_RX_GPIO_CLK, ENABLE);

    /* Enable UART clock */
    RCC_APB2PeriphClockCmd(COM1_CLK, ENABLE);

    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(COM1_TX_GPIO_PORT, COM1_TX_SOURCE, COM1_TX_AF);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(COM1_RX_GPIO_PORT, COM1_RX_SOURCE, COM1_RX_AF);

    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin   = COM1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM1_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin  = COM1_RX_PIN;
    GPIO_Init(COM1_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_ClearFlag(USART1,USART_FLAG_CTS | USART_FLAG_LBD | USART_IT_RXNE | USART_IT_TC);
	/*USART transmit complete interrupt,RXNE enable,and receive register not empty interrupt*/
	USART_ITConfig(COM1_USART1,USART_IT_RXNE,ENABLE);

    /* USART configuration */
    USART_Init(COM1_USART1,&USART_InitStructure);
    
    /* Enable USART */
	USART_Cmd(COM1_USART1, ENABLE);
}

/******************************************
* �������ƣ�����1���ж����ú���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void NVIC_COM1_config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = (u8) USART1_IRQn;  /*USART1_IRQn = 37, in typedef IRQn_Type*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)USART1_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)USART1_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************************************
*�������ƣ�uart1_write
*�������ܣ�UART1д
*��ڲ�����len -->д���ݳ���	
           p_data_buf->�������ݵ�ַ
*���ڲ�������
*�� �� ֵ��TRUE/FLASE
*ȫ�ֱ�������
*���ú�������
******************************************/
u8 uart1_write(u8 *pdata,u16 data_len)
{
	if(USART1_complete_flag != 1)
		return FALSE;
	else
	{	
//		USART_ClearITPendingBit(COM1_USART1,USART_IT_TXE);
				
		USART1_complete_flag = 0;		
		p_USART1_TX_buf = pdata;		
		USART1_data_number = data_len;	
		USART1_complete_number = 0;	

		USART_ITConfig(COM1_USART1,USART_IT_TXE,ENABLE);

		return TRUE;
	}
}


/******************************************
* �������ƣ�����3��ʼ������
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void COM3_init(void)
{
	NVIC_COM3_config();

	COM3_config();
}

/******************************************
* �������ƣ�����3�ĳ�ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void COM3_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate            = USART3_BaudRate;
	USART_InitStructure.USART_WordLength          = USART3_WordLength;
	USART_InitStructure.USART_StopBits   		  = USART3_StopBits;
	USART_InitStructure.USART_Parity     		  = USART3_Parity;
	USART_InitStructure.USART_HardwareFlowControl = USART3_HardwareFlowControl;
	USART_InitStructure.USART_Mode                = USART3_Mode;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(COM3_TX_GPIO_CLK | COM3_RX_GPIO_CLK, ENABLE);

    /* Enable UART clock */
    RCC_APB1PeriphClockCmd(COM3_CLK, ENABLE);

    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(COM3_TX_GPIO_PORT, COM3_TX_SOURCE, COM3_TX_AF);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(COM3_RX_GPIO_PORT, COM3_RX_SOURCE, COM3_RX_AF);

    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin   = COM3_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM3_TX_GPIO_PORT, &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin  = COM3_RX_PIN;
    GPIO_Init(COM3_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_ClearFlag(USART3,USART_FLAG_CTS | USART_FLAG_LBD | USART_IT_RXNE | USART_IT_TC);
	/*USART transmit complete interrupt,RXNE enable,and receive register not empty interrupt*/
	USART_ITConfig(COM3_USART3,USART_IT_RXNE,ENABLE);

    /* USART configuration */
    USART_Init(COM3_USART3,&USART_InitStructure);
    
    /* Enable USART */
	USART_Cmd(COM3_USART3, ENABLE);
}

/******************************************
* �������ƣ�����3���ж����ú���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void NVIC_COM3_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	/* Enable the USARTx Interrupt */
	/*USART3_IRQn = 39, in typedef IRQn_Type*/
	NVIC_InitStructure.NVIC_IRQChannel                   = (u8) USART3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)USART3_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = (u8)USART3_PRI & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************
*�������ƣ�uart3_write
*�������ܣ�UART3д
*��ڲ�����len -->д���ݳ���	
           p_data_buf->�������ݵ�ַ
*���ڲ�������
*�� �� ֵ��TRUE/FLASE
*ȫ�ֱ�������
*���ú�������
******************************************/
u8 uart3_write(u8 *pdata,u16 data_len)
{
	if(USART3_complete_flag != 1)
		return FALSE;
	else
	{	
//		USART_ClearITPendingBit(COM3_USART3,USART_IT_TXE);

		USART3_complete_flag = 0;		
		p_USART3_TX_buf = pdata;		
		USART3_data_number = data_len;	
		USART3_complete_number = 0;

		USART_ITConfig(COM3_USART3,USART_IT_TXE,ENABLE);	

		return TRUE;
	}
}


/******************************************
* �������ƣ�����4��ʼ������
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void COM4_init(void)
{
	NVIC_COM4_config();

	COM4_config();
}

/******************************************
* �������ƣ�����4���ú���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void COM4_config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate            = USART4_BaudRate;
	USART_InitStructure.USART_WordLength          = USART4_WordLength;
	USART_InitStructure.USART_StopBits   		  = USART4_StopBits;
	USART_InitStructure.USART_Parity     		  = USART4_Parity;
	USART_InitStructure.USART_HardwareFlowControl = USART4_HardwareFlowControl;
	USART_InitStructure.USART_Mode                = USART4_Mode;
	
	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(COM4_CLK, ENABLE); 
	
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(COM4_TX_GPIO_CLK | COM4_RX_GPIO_CLK, ENABLE);
	
	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(COM4_TX_GPIO_PORT, COM4_TX_SOURCE, COM4_TX_AF);
	
	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(COM4_RX_GPIO_PORT, COM4_RX_SOURCE, COM4_RX_AF);
	
	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	
	GPIO_InitStructure.GPIO_Pin   = COM4_TX_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(COM4_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin  = COM4_RX_PIN;
	GPIO_Init(COM4_RX_GPIO_PORT,&GPIO_InitStructure);
	
	
	USART_ClearFlag(UART4,USART_FLAG_CTS | USART_FLAG_LBD | USART_IT_RXNE | USART_IT_TC);
	/*USART transmit complete interrupt,RXNE enable,and receive register not empty interrupt*/
	USART_ITConfig(COM4_USART4,USART_IT_RXNE,ENABLE);
	
	/* USART configuration */
	USART_Init(COM4_USART4,&USART_InitStructure);
	
	/* Enable USART */
	USART_Cmd(COM4_USART4,ENABLE);
}

/******************************************
* �������ƣ�����4���ж����ú���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
static void NVIC_COM4_config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	/* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;	  /*UART4_IRQn = 52, in typedef IRQn_Type*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)USART4_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)USART4_PRI & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/******************************************
*�������ƣ�uart4_write
*�������ܣ�UART4д
*��ڲ�����len -->д���ݳ���	
           p_data_buf->�������ݵ�ַ
*���ڲ�������
*�� �� ֵ��TRUE/FLASE
*ȫ�ֱ�������
*���ú�������
******************************************/
u8 uart4_write(u8 *pdata,u16 data_len)
{
	if(USART4_complete_flag != 1)
		return FALSE;
	else
	{		
//		USART_ClearITPendingBit(COM4_USART4,USART_IT_TXE);
				
		USART4_complete_flag = 0;
		p_USART4_TX_buf = pdata;		
		USART4_data_number = data_len;	
		USART4_complete_number = 0;
		/*make sure at last enable the usart TXE interrupt*/
		USART_ITConfig(COM4_USART4,USART_IT_TXE,ENABLE);
		
		return TRUE;	  
	}
}
