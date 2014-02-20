/*******************************************************************************
�ļ�����flash.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ���ռ�
��д���ڣ�2011/10/20
��Ҫ������stm32�ڲ�����
�޸��ˣ�
�޸�����:
�޸ļ�¼��
*******************************************************************************/
#include "flash.h"


static u32 get_sector(u32 address);


/*******************************************************************************
*�������ƣ�flash_read_data
*�������ܣ���flash������
*��ڲ�����addr ������ʼ��ַ   data���ݴ�ŵ��׵�ַ   data_len���ݳ���
*���ڲ�������
*�� �� ֵ����
*ȫ�ֱ�������
*���ú�������
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
*�������ƣ�flash_write_data
*�������ܣ�дFLASH
*��ڲ�����addr flash��ַ   data�����׵�ַ   data_len���ݳ���
*���ڲ�������
*�� �� ֵ��TRUE/FALSE
*ȫ�ֱ�������
*���ú�������
******************************************/
BOOL flash_write_data(u32 addr, const u8* data, u32 data_len)
{
    u8 i = 0;
    u32 tm_wait_start = 0;
    u32 current_time;
    BOOL wait_started = FALSE;
    BOOL write_status = TRUE;
    u8 write_data_check = 0;

    FLASH_Unlock();                                          // ����flash

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

    FLASH_Lock();                                           // ����flash

    return write_status;
}
/******************************************
*�������ƣ�flash_erase
*�������ܣ�����ָ����ʼ��ַ�����ȵ��������ڵĿ�
*��ڲ�����start_address ��ʼ��ַ   data_len ���ݳ���
*���ڲ�������
*�� �� ֵ��FLASH_Status
*ȫ�ֱ�������
*���ú�������
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

    FLASH_Unlock();                                          // ����flash
    
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

    start_sector = get_sector(start_address);                // Ҫ��������ʼ��
    end_sector = get_sector(start_address + data_len);       // Ҫ�����Ľ�����

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

    FLASH_Lock();                                           // ����flash

    return erase_status;
}
/******************************************
*�������ƣ�get_sector
*�������ܣ���ȡָ����ʼ��ַ���ڵĿ�
*��ڲ�����address flash��ַ
*���ڲ�������
*�� �� ֵ��sector flash��
*ȫ�ֱ�������
*���ú�������
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
