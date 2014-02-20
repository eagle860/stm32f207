/*********************************************
文件名：fsmc.c
版本号：v1.0
版权说明：
作者：蔡和
编写日期：2011/09/13
简要描述：fsmc初始化及驱动
修改人：
修改日期:
修改记录：
********************************************/

#include "fsmc.h"

static void NAND_init(void);


/******************************************
* 函数名称：nand flash 提供给外部的初始化函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void FSMC_init(void)
{
    NAND_init();    
}

/******************************************
* 函数名称：stm32 nand flash初始化配置函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void NAND_init(void)
{
    FSMC_NANDInitTypeDef  FSMC_NANDInitStructure;
    FSMC_NAND_PCCARDTimingInitTypeDef  p;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
    
    /*-- GPIO Configuration ------------------------------------------------------*/
    /* OneNAND Data lines configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);    
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    
    /* GPIOD initial */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
	/* GPIOE initial */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    /* ALE, CLE, CE, RE, WE, R/B and WP configuration */    
    /* ALE configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* CLE configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* CE Configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* RE Configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* WE Configuration */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
      
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* R/B Configuration */   							 
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FSMC);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       
    GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* WP Configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* set WP high */
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
    
    /*-- FSMC Configuration ----------------------------------------------------*/
    p.FSMC_SetupTime = 0x01;
    p.FSMC_WaitSetupTime = 0x03;
    p.FSMC_HoldSetupTime = 0x02;
    p.FSMC_HiZSetupTime = 0x01;
    
    FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND;
    FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Enable;
    FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
    FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Disable;
    FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes;   
    FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;
    FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;

    FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;
    FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;
    
    FSMC_NANDInit(&FSMC_NANDInitStructure);
    FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}

/******************************************
* 函数名称：nand flash 等待函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 nand_wait(u16 time_out)
{
    /* 等待写完成  为低表示忙 */
    /* 8 instructions */
    while((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6) == (u8) Bit_RESET) && (time_out > 0))
    {
        time_out --;
    }

    if(0 == time_out)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/******************************************
* 函数名称：nand flash 写4字节地址函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void nand_write_4bytes_address(u32 address)
{
    static u16 i;
	    
    /* 注意：该FLASH的地址中间有5bit必须置0 */
    /* 并将高16位左移5位 */
    address = ((address << 5) & (0xFFFF0000)) | (address & 0x07FF);
    
    for(i = 0;i < 4;i ++) /* 写入四字节的地址 */
    {
        NAND_WRITE_ADDRESS_BYTE(address);
        address >>= 8;
    }
}

/******************************************
* 函数名称：nand flash 写2字节列地址函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void nand_write_column_address(u16 address)
{
    u16 i;
    address = address & 0x07FF;

    for(i = 0;i < 2;i ++)
    {
        NAND_WRITE_ADDRESS_BYTE((u8) address);
        address >>= 8;
    }
}

/******************************************
* 函数名称：nand flash 写入行地址函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void nand_write_row_address(u16 address)
{

    NAND_WRITE_ADDRESS_BYTE(address);
    address >>= 8;
    NAND_WRITE_ADDRESS_BYTE(address);
}

/******************************************
* 函数名称：nand flash 复位函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 nand_reset(void)
{
    /* 复位命令 */
    NAND_WRITE_COMMAND(RESET_CMD); 
    /* 等待命令完成，最大500us */
    return nand_wait(6250);  
}

/******************************************
* 函数名称：nand flash 读状态寄存器函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 nand_read_status(void)
{
    u8 status;
    NAND_WRITE_COMMAND(READ_STATUS_CMD);  /*写查询命令 */
    status  = *(u8 *)(NAND_BASE_ADDRESS);  /* 读回状态 */   
    return status;
}

/******************************************
* 函数名称：nand flash 擦除块函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 nand_erase_block(u32 address)
{
    address = address/BLOCK_SIZE;
	address <<= 6;

	/* 擦除命令第一字节命令 */
    NAND_WRITE_COMMAND((BLOCK_ERASE_CMD >> 8) & 0xFF);
    /* 写入两字节的页地址 */
    nand_write_row_address((u16) address);    
    /* 写入擦除命令第二字节 */
    NAND_WRITE_COMMAND(BLOCK_ERASE_CMD & 0xFF);
    /* 等待擦除完成 ,2ms*/
    (void) nand_wait(35000);
    /* 返回操作状态 */
    return nand_read_status();
}

/******************************************
* 函数名称：nand flash 页写命令的后半部分
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 nand_write_page_end(void)
{
    /* 页写命令 */
    NAND_WRITE_COMMAND(PAGE_PROGRAM_CMD & 0xFF);  
    /* 等待写完成,800us */
    (void) nand_wait(10000);  
    return nand_read_status();
}

/******************************************
* 函数名称：nand flash 回写命令
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 nand_copy_back(u32 source_address,u32 destination_address)
{
    NAND_WRITE_COMMAND((COPY_BACK_READ_CMD >> 8) & 0xFF);
    nand_write_4bytes_address(source_address);
    NAND_WRITE_COMMAND(COPY_BACK_READ_CMD & 0xFF);
    /* 等待写完，估计为600us */
    (void) nand_wait(7500);

    NAND_WRITE_COMMAND((COPY_BACK_PROGRAM_CMD >> 8) & 0xFF);
    nand_write_4bytes_address(destination_address);
    NAND_WRITE_COMMAND(COPY_BACK_PROGRAM_CMD & 0xFF);
    /* 等待写完，800us */
    (void) nand_wait(10000);
    return nand_read_status();  
}


/******************************************
* 函数名称：nand flash 读取ID函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void nand_read_ID(u8 *buf)
{
	u8 i;

	NAND_WRITE_COMMAND(READ_ID_CMD);

	NAND_WRITE_ADDRESS_BYTE(0x00); 

	/* 读取5字节ID */
	for(i = 0;i < 5;i ++)
	{
		buf[i] = NAND_READ_DATA_BYTE();
	}
}

/******************************************
* 函数名称：nand flash 检查坏块
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8 check_invalid_block(u16 block_num)
{
	u8 data;
	u16 i;
	u32 address;

	/* 检查第一页，第2048个字节 */
    address = ((u32)block_num << 22) | FLASH_PAGE_SIZE;

    NAND_WRITE_COMMAND((READ_CMD >> 8) & 0xFF);
	/* 写入四字节的地址 */
	for(i = 0;i < 4;i ++)
    {
        NAND_WRITE_ADDRESS_BYTE(address);
        address >>= 8;
    }
    NAND_WRITE_COMMAND(READ_CMD & 0xFF);

    (void) nand_wait(350);

	data = NAND_READ_DATA_BYTE();
	
	if(0xFF != data)
	{
		return FALSE;
	}
	
	/* 检查第二页，第2048个字节 */
	address = ((u32) block_num << 22) | (1 << 16) | FLASH_PAGE_SIZE;

    NAND_WRITE_COMMAND((READ_CMD >> 8) & 0xFF);
	/* 写入四字节的地址 */
	for(i = 0;i < 4;i ++)
    {
        NAND_WRITE_ADDRESS_BYTE(address);
        address >>= 8;
    }
    NAND_WRITE_COMMAND(READ_CMD & 0xFF);

    (void) nand_wait(350);

	data = NAND_READ_DATA_BYTE();
	
	if(0xFF != data)
	{
		return FALSE;
	}
	
	return TRUE;	
}

/******************************************
* 函数名称：nand flash 标记坏块
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void invalid_block_mark(u16 block_num)
{
	u8 data;
	u16 i;
	u32 address;
	
	data = 0;

	/* 写第一页，第2048个字节 */
    address = ((u32)block_num << 22) | FLASH_PAGE_SIZE;

	NAND_WRITE_COMMAND((PAGE_PROGRAM_CMD >> 8) & 0xFF);
	/* 写入四字节的地址 */
	for(i = 0;i < 4;i ++)
    {
        NAND_WRITE_ADDRESS_BYTE(address);
        address >>= 8;
    }
	NAND_WRITE_DATA_BYTE(data);
	NAND_WRITE_COMMAND(PAGE_PROGRAM_CMD & 0xFF);
	/* max 700us */
    (void) nand_wait(9000);
	
	/* 写第二页，第2048个字节 */
	address = ((u32) block_num << 22) | (1 << 16) | FLASH_PAGE_SIZE;

	NAND_WRITE_COMMAND((PAGE_PROGRAM_CMD >> 8) & 0xFF);
	/* 写入四字节的地址 */
	for(i = 0;i < 4;i ++)
    {
        NAND_WRITE_ADDRESS_BYTE(address);
        address >>= 8;
    }
	NAND_WRITE_DATA_BYTE(data);
	NAND_WRITE_COMMAND(PAGE_PROGRAM_CMD & 0xFF);
	/* max 700us */
    (void) nand_wait(9000);        
}
