/*********************************************
�ļ�����fsmc.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/09/13
��Ҫ������fsmc��ʼ��������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/

#include "fsmc.h"

static void NAND_init(void);


/******************************************
* �������ƣ�nand flash �ṩ���ⲿ�ĳ�ʼ������
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void FSMC_init(void)
{
    NAND_init();    
}

/******************************************
* �������ƣ�stm32 nand flash��ʼ�����ú���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
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
* �������ƣ�nand flash �ȴ�����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8 nand_wait(u16 time_out)
{
    /* �ȴ�д���  Ϊ�ͱ�ʾæ */
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
* �������ƣ�nand flash д4�ֽڵ�ַ����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void nand_write_4bytes_address(u32 address)
{
    static u16 i;
	    
    /* ע�⣺��FLASH�ĵ�ַ�м���5bit������0 */
    /* ������16λ����5λ */
    address = ((address << 5) & (0xFFFF0000)) | (address & 0x07FF);
    
    for(i = 0;i < 4;i ++) /* д�����ֽڵĵ�ַ */
    {
        NAND_WRITE_ADDRESS_BYTE(address);
        address >>= 8;
    }
}

/******************************************
* �������ƣ�nand flash д2�ֽ��е�ַ����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
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
* �������ƣ�nand flash д���е�ַ����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void nand_write_row_address(u16 address)
{

    NAND_WRITE_ADDRESS_BYTE(address);
    address >>= 8;
    NAND_WRITE_ADDRESS_BYTE(address);
}

/******************************************
* �������ƣ�nand flash ��λ����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8 nand_reset(void)
{
    /* ��λ���� */
    NAND_WRITE_COMMAND(RESET_CMD); 
    /* �ȴ�������ɣ����500us */
    return nand_wait(6250);  
}

/******************************************
* �������ƣ�nand flash ��״̬�Ĵ�������
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8 nand_read_status(void)
{
    u8 status;
    NAND_WRITE_COMMAND(READ_STATUS_CMD);  /*д��ѯ���� */
    status  = *(u8 *)(NAND_BASE_ADDRESS);  /* ����״̬ */   
    return status;
}

/******************************************
* �������ƣ�nand flash �����麯��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8 nand_erase_block(u32 address)
{
    address = address/BLOCK_SIZE;
	address <<= 6;

	/* ���������һ�ֽ����� */
    NAND_WRITE_COMMAND((BLOCK_ERASE_CMD >> 8) & 0xFF);
    /* д�����ֽڵ�ҳ��ַ */
    nand_write_row_address((u16) address);    
    /* д���������ڶ��ֽ� */
    NAND_WRITE_COMMAND(BLOCK_ERASE_CMD & 0xFF);
    /* �ȴ�������� ,2ms*/
    (void) nand_wait(35000);
    /* ���ز���״̬ */
    return nand_read_status();
}

/******************************************
* �������ƣ�nand flash ҳд����ĺ�벿��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8 nand_write_page_end(void)
{
    /* ҳд���� */
    NAND_WRITE_COMMAND(PAGE_PROGRAM_CMD & 0xFF);  
    /* �ȴ�д���,800us */
    (void) nand_wait(10000);  
    return nand_read_status();
}

/******************************************
* �������ƣ�nand flash ��д����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8 nand_copy_back(u32 source_address,u32 destination_address)
{
    NAND_WRITE_COMMAND((COPY_BACK_READ_CMD >> 8) & 0xFF);
    nand_write_4bytes_address(source_address);
    NAND_WRITE_COMMAND(COPY_BACK_READ_CMD & 0xFF);
    /* �ȴ�д�꣬����Ϊ600us */
    (void) nand_wait(7500);

    NAND_WRITE_COMMAND((COPY_BACK_PROGRAM_CMD >> 8) & 0xFF);
    nand_write_4bytes_address(destination_address);
    NAND_WRITE_COMMAND(COPY_BACK_PROGRAM_CMD & 0xFF);
    /* �ȴ�д�꣬800us */
    (void) nand_wait(10000);
    return nand_read_status();  
}


/******************************************
* �������ƣ�nand flash ��ȡID����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void nand_read_ID(u8 *buf)
{
	u8 i;

	NAND_WRITE_COMMAND(READ_ID_CMD);

	NAND_WRITE_ADDRESS_BYTE(0x00); 

	/* ��ȡ5�ֽ�ID */
	for(i = 0;i < 5;i ++)
	{
		buf[i] = NAND_READ_DATA_BYTE();
	}
}

/******************************************
* �������ƣ�nand flash ��黵��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8 check_invalid_block(u16 block_num)
{
	u8 data;
	u16 i;
	u32 address;

	/* ����һҳ����2048���ֽ� */
    address = ((u32)block_num << 22) | FLASH_PAGE_SIZE;

    NAND_WRITE_COMMAND((READ_CMD >> 8) & 0xFF);
	/* д�����ֽڵĵ�ַ */
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
	
	/* ���ڶ�ҳ����2048���ֽ� */
	address = ((u32) block_num << 22) | (1 << 16) | FLASH_PAGE_SIZE;

    NAND_WRITE_COMMAND((READ_CMD >> 8) & 0xFF);
	/* д�����ֽڵĵ�ַ */
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
* �������ƣ�nand flash ��ǻ���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void invalid_block_mark(u16 block_num)
{
	u8 data;
	u16 i;
	u32 address;
	
	data = 0;

	/* д��һҳ����2048���ֽ� */
    address = ((u32)block_num << 22) | FLASH_PAGE_SIZE;

	NAND_WRITE_COMMAND((PAGE_PROGRAM_CMD >> 8) & 0xFF);
	/* д�����ֽڵĵ�ַ */
	for(i = 0;i < 4;i ++)
    {
        NAND_WRITE_ADDRESS_BYTE(address);
        address >>= 8;
    }
	NAND_WRITE_DATA_BYTE(data);
	NAND_WRITE_COMMAND(PAGE_PROGRAM_CMD & 0xFF);
	/* max 700us */
    (void) nand_wait(9000);
	
	/* д�ڶ�ҳ����2048���ֽ� */
	address = ((u32) block_num << 22) | (1 << 16) | FLASH_PAGE_SIZE;

	NAND_WRITE_COMMAND((PAGE_PROGRAM_CMD >> 8) & 0xFF);
	/* д�����ֽڵĵ�ַ */
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
