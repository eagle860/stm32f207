/*******************************************************************************
文件名：flash.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/10/20
简要描述：stm32内部驱动
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "flash.h"


static u32 get_sector(u32 address);


/*******************************************************************************
*函数名称：flash_read_data
*函数功能：从flash读数据
*入口参数：addr 数据起始地址   data数据存放的首地址   data_len数据长度
*出口参数：无
*返 回 值：无
*全局变量：无
*调用函数：无
*******************************************************************************/
void flash_read_data(u32 addr, u8* data, u32 data_len)
{
    u32 i = 0;

    for(i= 0; i < data_len ; i++)
    {
        *(u8*)(data+i)  =  *(vu8 *)(addr + i);
    }
}
/******************************************
*函数名称：flash_write_data
*函数功能：写FLASH
*入口参数：addr flash地址   data数据首地址   data_len数据长度
*出口参数：无
*返 回 值：TRUE/FALSE
*全局变量：无
*调用函数：无
******************************************/
BOOL flash_write_data(u32 addr, const u8* data, u32 data_len)
{
    u8 i = 0;
    u32 tm_wait_start = 0;
    u32 current_time;
    BOOL wait_started = FALSE;
    BOOL write_status = TRUE;
    u8 write_data_check = 0;

    FLASH_Unlock();                                          // 解锁flash

    while (i < data_len)
    {
        if (FLASH_COMPLETE == FLASH_ProgramByte(addr + i,*(data + i)))
        {
            flash_read_data(addr + i, &write_data_check, 1);
            if (write_data_check == *(data + i))
            {
                i++;
                wait_started = FALSE;
            }
            else
            {
                write_status = FALSE;
                break;
            }
        }
        else
        {
            if (wait_started)
            {
                if (check_timeout(tm_wait_start, FLASH_WRITE_BUSY_TIME))
                {
                    write_status = FALSE;
                    break;
                }
            }
            else
            {
                get_current_1ms_cnt(current_time);
                tm_wait_start = current_time;
                wait_started = TRUE;
            }
        }
    }

    FLASH_Lock();                                           // 锁定flash

    return write_status;
}
/******************************************
*函数名称：flash_erase
*函数功能：擦除指定起始地址及长度的数据所在的块
*入口参数：start_address 起始地址   data_len 数据长度
*出口参数：无
*返 回 值：FLASH_Status
*全局变量：无
*调用函数：无
******************************************/
BOOL flash_erase(u32 start_address, u32 data_len)
{
    u8 start_sector = 0;
    u8 end_sector = 0;
    u8 i = 0;
    u32 tm_wait_start = 0;
    u32 current_time;
    BOOL wait_started = FALSE;
    BOOL erase_status = TRUE;

    FLASH_Unlock();                                          // 解锁flash
    
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

    start_sector = get_sector(start_address);                // 要擦除的起始块
    end_sector = get_sector(start_address + data_len);       // 要擦除的结束块

    i = start_sector;
    while (i <= end_sector)
    {
        if (FLASH_COMPLETE == FLASH_EraseSector(i, VoltageRange_3))
        {
            i += 8;
            wait_started = FALSE;
        }
        else
        {
            if (wait_started)
            {
                if (check_timeout(tm_wait_start, FLASH_ERASE_BUSY_TIME))
                {
                    erase_status = FALSE;
                    break;
                }
            }
            else
            {
                get_current_1ms_cnt(current_time);
                tm_wait_start = current_time;
                wait_started = TRUE;
            }
        }
    }

    FLASH_Lock();                                           // 锁定flash

    return erase_status;
}
/******************************************
*函数名称：get_sector
*函数功能：获取指定起始地址所在的块
*入口参数：address flash地址
*出口参数：无
*返 回 值：sector flash块
*全局变量：无
*调用函数：无
******************************************/
static u32 get_sector(u32 address)
{
    u32 sector = 0;

    if((address < FLASH_SECTOR_1_START_ADDR) && (address >= FLASH_SECTOR_0_START_ADDR))
    {
      sector = FLASH_Sector_0;
    }
    else if((address < FLASH_SECTOR_2_START_ADDR) && (address >= FLASH_SECTOR_1_START_ADDR))
    {
      sector = FLASH_Sector_1;
    }
    else if((address < FLASH_SECTOR_3_START_ADDR) && (address >= FLASH_SECTOR_2_START_ADDR))
    {
      sector = FLASH_Sector_2;
    }
    else if((address < FLASH_SECTOR_4_START_ADDR) && (address >= FLASH_SECTOR_3_START_ADDR))
    {
      sector = FLASH_Sector_3;
    }
    else if((address < FLASH_SECTOR_5_START_ADDR) && (address >= FLASH_SECTOR_4_START_ADDR))
    {
      sector = FLASH_Sector_4;
    }
    else if((address < FLASH_SECTOR_6_START_ADDR) && (address >= FLASH_SECTOR_5_START_ADDR))
    {
      sector = FLASH_Sector_5;
    }
    else if((address < FLASH_SECTOR_7_START_ADDR) && (address >= FLASH_SECTOR_6_START_ADDR))
    {
      sector = FLASH_Sector_6;
    }
    else/*(address < FLASH_END_ADDR) && (address >= ADDR_FLASH_SECTOR_7))*/
    {
      sector = FLASH_Sector_7;
    }

    return sector;
}
