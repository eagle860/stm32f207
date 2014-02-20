/*********************************************
文件名：spi.c
版本号：v1.0
版权说明：
作者：蔡和
编写日期：2011/08/30
简要描述：spi初始化及驱动
修改人：
修改日期:
修改记录：
********************************************/
#include "spi.h"


/*test parameter*/
u8 g_SPI1_send_buf[] = {0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38, 
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,0xE3,0x8E,0x38,
                        0xE3,0x8E,0x38};
/*end of test parameter*/

/*SPI1 初始化相关函数*/
static void SPI1_Init(void);

static void SPI1_Config(void);

static void SPI1_DMA_Config(void);

static u8 SPI1_DMA_Eable(void);

/*SPI2 初始化相关函数*/
static void SPI2_Init(void);

static void SPI2_Config(void);

static void SPI2_DMA_Config(void);

static u8 SPI2_DMA_Eable(void);

/******************************************
*函数名称：SPI_init
*函数功能：spi1, spi2 initial
*入口参数：无	
*出口参数：无		   
*返 回 值：无
*全局变量：无
*调用函数：无
******************************************/
void SPI_init(void)
{
    SPI1_Init();
    SPI2_Init();
}

/******************************************
*函数名称：spi1_send
*函数功能：通道1 FM0数据通过SPI1发送
*入口参数：len->数据长度
*出口参数：无		   
*返 回 值：数据放入DMA返回TRUE，DMA忙返回FLASE
*全局变量：无
*调用函数：无
******************************************/
u8 spi1_send(u16 len)
{
    /*if the DMA is running, return false*/
    if((DMA_GetCmdStatus(DMA2_Stream3) == ENABLE))
    {
    	return FALSE;
    }
    /*disable the dma to set new data length*/
    /*if disable failed, return false*/
    if(FALSE == SPI1_DMA_disable())
    {
    	return FALSE;
    }
    /*set new data length*/
    DMA_SetCurrDataCounter(DMA2_Stream3,len);
    /*enable dma, if failed, return false*/
    if(FALSE == SPI1_DMA_Eable())
    {
    	return FALSE;
    }
    /*return true*/
    return TRUE;
}

/******************************************
*函数名称：spi2_send
*函数功能：通道2 FM0数据通过SPI2发送
*入口参数：len->数据长度
*出口参数：无		   
*返 回 值：数据放入DMA返回TRUE，DMA忙返回FLASE
*全局变量：无
*调用函数：无
******************************************/
u8 spi2_send(u16 len)
{
    /*if the DMA is running, return false*/
    if((DMA_GetCmdStatus(DMA1_Stream4) == ENABLE))
    {
    	return FALSE;
    }
    /*disable the dma to set new data length*/
    /*if disable failed, return false*/
    if(FALSE == SPI2_DMA_disable())
    {
    	return FALSE;
    }
    /*set new data length*/
    DMA_SetCurrDataCounter(DMA1_Stream4,len);
    /*enable dma, if failed, return false*/
    if(FALSE == SPI2_DMA_Eable())
    {
    	return FALSE;
    }
    /*return true*/
    return TRUE;
}

/******************************************
* 函数名称：SPI1 DMA方式初始化函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void SPI1_Init(void)
{
    /*SPI1 setting config*/
    SPI1_Config();
    /*SPI1 DMA setting config*/
    SPI1_DMA_Config();
    /*Enable SPI1*/
    SPI_Cmd(SPI1,ENABLE);
}

/******************************************
* 函数名称：SPI1 配置函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void SPI1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    /* Enable the SPI clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    
    /* Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(SPI1_SCK_GPIO_CLK | SPI1_MISO_GPIO_CLK | SPI1_MOSI_GPIO_CLK, ENABLE);
    
    /* Connect SPI pins to AF5 */  
    GPIO_PinAFConfig(SPI1_SCK_GPIO_PORT, SPI1_SCK_SOURCE, SPI1_SCK_AF);
    GPIO_PinAFConfig(SPI1_MOSI_GPIO_PORT, SPI1_MOSI_SOURCE, SPI1_MOSI_AF);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /* SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = SPI1_SCK_PIN;
    GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI  MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  SPI1_MOSI_PIN;
    GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI configuration -------------------------------------------------------*/
    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    /**/
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    /*set the prescale of spi, if the sysclock has changed, it should be modified*/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    
    SPI_Init(SPI1,&SPI_InitStructure);
}

/******************************************
* 函数名称：SPI1 DMA配置函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void SPI1_DMA_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;
    
    /* Enable DMA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);	
    
      /* Disable the DMA stream */
    DMA_Cmd(DMA2_Stream3,DISABLE);
    
    /* Reset DMA Stream registers (for debug purpose) */
    DMA_DeInit(DMA2_Stream3);
    
    /* Check if the DMA Stream is disabled before enabling it.
     Note that this step is useful when the same Stream is used multiple times:
     enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     will be effective only at the end of the ongoing data transfer and it will 
     not be possible to re-configure it before making sure that the Enable bit 
     has been cleared by hardware. If the Stream is used only once, this step might 
     be bypassed. */
    while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE)
    {
    }
    
    /* Configure DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_3;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR));// ((uint32_t)SPI1 + 0x0C);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_SPI1_send_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    
    DMA_Init(DMA2_Stream3, &DMA_InitStructure); 
    
    /*Clear the flags*/
    DMA_ClearFlag(DMA2_Stream3,DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3 | DMA_FLAG_TEIF3 | DMA_FLAG_DMEIF3 | DMA_FLAG_FEIF3);
    /* Enable DMA Stream Transfer Complete interrupt */
    DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);   
    
    /* Enable the DMA Stream IRQ Channel */
    /*set the NVIC number of DMA2_Stream3_IRQn*/
    NVIC_InitStructure.NVIC_IRQChannel = (u8) DMA2_Stream3_IRQn;   //DMA2_Stream3_IRQn = 59,IRQn_Type
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)SPI1_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)SPI1_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}

/******************************************
* 函数名称：SPI1 DMA使能
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static u8 SPI1_DMA_Eable(void)
{
   u16 time_out;
   time_out = 10000;
   /*Enable and configure the peripheral to be connected to the DMA*/
   SPI_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
   /*Enable DMA2 stream3*/
   DMA_Cmd(DMA2_Stream3, ENABLE);
   
   /*wait until DMA enable*/
   while ((DMA_GetCmdStatus(DMA2_Stream3) != ENABLE) && time_out > 0)
   {
       time_out --;
   }
   /*if the DMA can not enable, return false*/
   if(time_out == 0)
   {
       return FALSE;
   }
   else
   { 
       return TRUE;
   }	
}

/******************************************
* 函数名称：SPI1 DMA关闭
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 SPI1_DMA_disable(void)
{
    u16 time_out;
    time_out = 10000;
    
    SPI_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
    
    DMA_Cmd(DMA2_Stream3,DISABLE);
    /*wait until DMA disable*/
    while ((DMA_GetCmdStatus(DMA2_Stream3) != DISABLE) && time_out > 0)
    {
        time_out --;
    }
    /*if the DMA can not disable, return false*/
    if(time_out == 0)
    {
        return FALSE;
    }
    else
    { 
        return TRUE;
    }
}

/******************************************
* 函数名称：SPI1 DMA方式初始化函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void SPI2_Init(void)
{
    /*SPI2 setting config*/
    SPI2_Config();
    /*SPI2 DMA setting config*/
    SPI2_DMA_Config();
    /*Enable SPI2*/
    SPI_Cmd(SPI2,ENABLE);
}

/******************************************
* 函数名称：SPI2 配置函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void SPI2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    /* Enable the SPI clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    
    /* Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(SPI2_SCK_GPIO_CLK | SPI2_MISO_GPIO_CLK | SPI2_MOSI_GPIO_CLK, ENABLE);
    
    /* Connect SPI pins to AF5 */  
    GPIO_PinAFConfig(SPI2_SCK_GPIO_PORT, SPI2_SCK_SOURCE, SPI2_SCK_AF);
    GPIO_PinAFConfig(SPI2_MOSI_GPIO_PORT, SPI2_MOSI_SOURCE, SPI2_MOSI_AF);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /* SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = SPI2_SCK_PIN;
    GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI  MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  SPI2_MOSI_PIN;
    GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI configuration -------------------------------------------------------*/
    SPI_I2S_DeInit(SPI2);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
    /*it is for CRC if enable*/
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    
    SPI_Init(SPI2,&SPI_InitStructure);
}

/******************************************
* 函数名称：SPI2 DMA配置函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void SPI2_DMA_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;
    
    /* Enable DMA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	
    
      /* Disable the DMA stream */
    DMA_Cmd(DMA1_Stream4,DISABLE);
    
    /* Reset DMA Stream registers (for debug purpose) */
    DMA_DeInit(DMA1_Stream4);
    
    /* Check if the DMA Stream is disabled before enabling it.
     Note that this step is useful when the same Stream is used multiple times:
     enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     will be effective only at the end of the ongoing data transfer and it will 
     not be possible to re-configure it before making sure that the Enable bit 
     has been cleared by hardware. If the Stream is used only once, this step might 
     be bypassed. */
    while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE)
    {
    }
    
    /* Configure DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI2->DR));// ((uint32_t)SPI1 + 0x0C);
    /*the address of memory, should modify*/
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)g_SPI1_send_buf;  //need to modify
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)BUFFER_SIZE;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    
    DMA_Init(DMA1_Stream4, &DMA_InitStructure); 
    
    /*Clear the flags*/
    DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4 | DMA_FLAG_TEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_FEIF4);
    /* Enable DMA Stream Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Stream4, DMA_IT_TC, ENABLE);   
    
    /* Enable the DMA Stream IRQ Channel */
      /*set the NVIC number of DMA1_Stream4_IRQn*/
    NVIC_InitStructure.NVIC_IRQChannel = (u8) DMA1_Stream4_IRQn;   //DMA1_Stream4_IRQn = IRQn_Type
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)SPI2_PRI >> 4) & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)SPI2_PRI & 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
}

/******************************************
* 函数名称：SPI2 DMA使能
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static u8 SPI2_DMA_Eable(void)
{
    u16 time_out;
    time_out = 10000;
    /*Enable and configure the peripheral to be connected to the DMA*/
    SPI_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
    /*Enable DMA2 stream3*/
    DMA_Cmd(DMA1_Stream4, ENABLE);
    
    /*wait until DMA enable*/
    while ((DMA_GetCmdStatus(DMA1_Stream4) != ENABLE) && time_out > 0)
    {
        time_out --;
    }
    /*if the DMA can not enable, return false*/
    if(time_out == 0)
    {
        return FALSE;
    }
    else
    { 
        return TRUE;	
    }
}

/******************************************
* 函数名称：SPI2 DMA关闭
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 SPI2_DMA_disable(void)
{
    u16 time_out;
    time_out = 10000;
    
    DMA_Cmd(DMA1_Stream4,DISABLE);
    SPI_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,DISABLE);
    /*wait until DMA disable*/
    while ((DMA_GetCmdStatus(DMA1_Stream4) != DISABLE) && time_out > 0)
    {
        time_out --;
    }
    /*if the DMA can not disable, return false*/
    if(time_out == 0)
    {
        return FALSE;
    }
    else
    { 
        return TRUE;   
    }
}
