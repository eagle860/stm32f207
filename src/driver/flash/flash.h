#ifndef _FLASH_H
#define _FLASH_H


#include "public.h"


// ����STM32F207ZE���ڲ�FLASH 512 Kbytes
#define FLASH_START_ADDR        FLASH_BASE        // �ڲ�flash��ʼ��ַ
#define FLASH_SIZE              ((u32)0x00080000) // �ڲ�flash��С
#define FLASH_ERASE_BUSY_TIME   ((u32)0x00004000) // �ڲ�flash�������ʱ��
#define FLASH_WRITE_BUSY_TIME   ((u32)0x00000001) // �ڲ�flash���д��ʱ��

#define PAGE_SIZE               ((u8)0xFF)       // �ڲ�flashҳ��С

/* Base address of the Flash sectors */
#define FLASH_SECTOR_0_START_ADDR   ((u32)FLASH_START_ADDR)
#define FLASH_SECTOR_0_SIZE         ((u32)( 16 * 1024UL))                                    // 16 Kbytes
#define FLASH_SECTOR_1_START_ADDR   ((u32)(FLASH_SECTOR_0_START_ADDR + FLASH_SECTOR_0_SIZE))
#define FLASH_SECTOR_1_SIZE         ((u32)( 16 * 1024UL))                                    // 16 Kbytes
#define FLASH_SECTOR_2_START_ADDR   ((u32)(FLASH_SECTOR_1_START_ADDR + FLASH_SECTOR_1_SIZE))
#define FLASH_SECTOR_2_SIZE         ((u32)( 16 * 1024UL))                                    // 16 Kbytes
#define FLASH_SECTOR_3_START_ADDR   ((u32)(FLASH_SECTOR_2_START_ADDR + FLASH_SECTOR_2_SIZE))
#define FLASH_SECTOR_3_SIZE         ((u32)( 16 * 1024UL))                                    // 16 Kbytes
#define FLASH_SECTOR_4_START_ADDR   ((u32)(FLASH_SECTOR_3_START_ADDR + FLASH_SECTOR_3_SIZE))
#define FLASH_SECTOR_4_SIZE         ((u32)( 64 * 1024UL))                                    // 16 Kbytes
#define FLASH_SECTOR_5_START_ADDR   ((u32)(FLASH_SECTOR_4_START_ADDR + FLASH_SECTOR_4_SIZE))
#define FLASH_SECTOR_5_SIZE         ((u32)(128 * 1024UL))                                    // 16 Kbytes
#define FLASH_SECTOR_6_START_ADDR   ((u32)(FLASH_SECTOR_5_START_ADDR + FLASH_SECTOR_5_SIZE))
#define FLASH_SECTOR_6_SIZE         ((u32)(128 * 1024UL))                                    // 16 Kbytes
#define FLASH_SECTOR_7_START_ADDR   ((u32)(FLASH_SECTOR_6_START_ADDR + FLASH_SECTOR_6_SIZE))
#define FLASH_SECTOR_7_SIZE         ((u32)(128 * 1024UL))                                    // 16 Kbytes
#define FLASH_END_ADDR              ((u32)(FLASH_SECTOR_7_START_ADDR + FLASH_SECTOR_7_SIZE))

#define param_struct_START_ADDR     FLASH_SECTOR_6_START_ADDR      // ������
#define param_struct_bak_START_ADDR FLASH_SECTOR_7_START_ADDR      // ����������

#define UNIQUE_DEVICE_ID            ((u32)0x1FFF7A10)              // MCUΨһ�豸ID�ŵ�ַ


void flash_read_data(u32 addr, u8* data, u32 data_len);
BOOL flash_write_data(u32 addr, const u8* data, u32 data_len);
BOOL flash_erase(u32 start_address, u32 data_len);


#endif //~ _FLASH_H
