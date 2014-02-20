/*********************************************
文件名：fsmc.h
版本号：v1.0
版权说明：
作者：蔡和
编写日期：2011/09/13
简要描述：fsmc	外部接口声明
修改人：
修改日期:
修改记录：
********************************************/

#ifndef _FSMC_H
#define _FSMC_H

#include "public.h"

/**the command of 128M nand flash------------------**/
/**|     Function     |1st cycle|2nd cycle|**/ 
/**|     Read         |   00h   |  30h    |**/ 
/**|Read for Copy Back|   00h   |  35h    |**/ 
/**|     Read ID      |   90h   |  -      |**/ 
/**|     Reset        |   FFh   |  -      |**/ 
/**|   Page Program   |   80h   |  10h    |**/ 
/**|Copy-Back Program |   85h   |  10h    |**/ 
/**|   Block Erase    |   60h   |  D0h    |**/ 
/**|Random Data Input |   85h   |  -      |**/ 
/**|Random Data Output|   05h   |  E0h    |**/ 
/**|    Read Status   |   70h   |         |**/ 
/**|  Read EDC Status |   7Bh   |         |**/ 
/**end of nand flash command     **/

/*----------------------------------------宏定义------------------------------------------*/
#define NAND_BASE_ADDRESS   0x70000000UL
#define CMD_BIT				(0x00000001UL << 16)
#define ADDRESS_BIT         (0x00000001UL << 17)
#define DATA_BIT            0x00000000UL
#define FLASH_PAGE_SIZE     0x800 
#define BLOCK_SIZE			0x20000

/*--------------------------------------命令宏定义----------------------------------------*/
#define   READ_CMD  				((u16) 0x0030)
#define	  COPY_BACK_READ_CMD    	((u16) 0x0035)
#define	  READ_ID_CMD				(0x90)
#define	  RESET_CMD 				(0xFF)
#define	  PAGE_PROGRAM_CMD			(0x8010)
#define	  COPY_BACK_PROGRAM_CMD		(0x8510)
#define	  BLOCK_ERASE_CMD			(0x60D0)
#define	  RANDOM_DATA_INPUT_CMD		(0x85)
#define	  RANDOM_DATA_OUTPUT_CMD	(0x05E0)
#define	  READ_STATUS_CMD			(0x70)
#define	  READ_EDC_STATUS			(0x7B)

/*--------------------------------------函数宏定义----------------------------------------*/
#define NAND_WRITE_COMMAND(cmd) do{*(u8 *)(NAND_BASE_ADDRESS | CMD_BIT) = (u8) (cmd);}while(0)

#define NAND_WRITE_ADDRESS_BYTE(address) do{*(u8 *)(NAND_BASE_ADDRESS | ADDRESS_BIT) = ((u8) (address));}while(0)

#define NAND_WRITE_DATA_BYTE(data) do{*(u8 *)(NAND_BASE_ADDRESS | DATA_BIT) = (u8) (data);}while(0)
/* 读数据举例：data = NAND_READ_DATA_BYTE(); */
#define NAND_READ_DATA_BYTE() 	(*(u8 *)(NAND_BASE_ADDRESS | DATA_BIT))	 

/*--------------------------------------函数声明----------------------------------------*/
/* 初始化函数 */
void FSMC_init(void);
/* 读取nand ID */
void nand_read_ID(u8 *buf);
/* nand 复位 */
u8 nand_reset(void);
/* 等待准备好，约为8条指令，每条指令约为0.01us */
u8 nand_wait(u16 time_out);
/* 写四字节地址 */
void nand_write_4bytes_address(u32 address);
/* 写列地址 */
void nand_write_column_address(u16 address);
/* 写行地址 */
void nand_write_row_address(u16 address);
/* 读返回状态 */
u8 nand_read_status(void);
/* 擦除块 */
u8 nand_erase_block(u32 address);
/* 写页函数（后半部分） */
u8 nand_write_page_end(void);
/* 读坏块标志 */
u8 check_invalid_block(u16 block_num);
/* 标记坏块 */
void invalid_block_mark(u16 block_num); 
/* 回写编程 */
u8 nand_copy_back(u32 source_address,u32 destination_address);

#endif 
