/*********************************************
�ļ�����fsmc.h
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/09/13
��Ҫ������fsmc	�ⲿ�ӿ�����
�޸��ˣ�
�޸�����:
�޸ļ�¼��
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

/*----------------------------------------�궨��------------------------------------------*/
#define NAND_BASE_ADDRESS   0x70000000UL
#define CMD_BIT				(0x00000001UL << 16)
#define ADDRESS_BIT         (0x00000001UL << 17)
#define DATA_BIT            0x00000000UL
#define FLASH_PAGE_SIZE     0x800 
#define BLOCK_SIZE			0x20000

/*--------------------------------------����궨��----------------------------------------*/
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

/*--------------------------------------�����궨��----------------------------------------*/
#define NAND_WRITE_COMMAND(cmd) do{*(u8 *)(NAND_BASE_ADDRESS | CMD_BIT) = (u8) (cmd);}while(0)

#define NAND_WRITE_ADDRESS_BYTE(address) do{*(u8 *)(NAND_BASE_ADDRESS | ADDRESS_BIT) = ((u8) (address));}while(0)

#define NAND_WRITE_DATA_BYTE(data) do{*(u8 *)(NAND_BASE_ADDRESS | DATA_BIT) = (u8) (data);}while(0)
/* �����ݾ�����data = NAND_READ_DATA_BYTE(); */
#define NAND_READ_DATA_BYTE() 	(*(u8 *)(NAND_BASE_ADDRESS | DATA_BIT))	 

/*--------------------------------------��������----------------------------------------*/
/* ��ʼ������ */
void FSMC_init(void);
/* ��ȡnand ID */
void nand_read_ID(u8 *buf);
/* nand ��λ */
u8 nand_reset(void);
/* �ȴ�׼���ã�ԼΪ8��ָ�ÿ��ָ��ԼΪ0.01us */
u8 nand_wait(u16 time_out);
/* д���ֽڵ�ַ */
void nand_write_4bytes_address(u32 address);
/* д�е�ַ */
void nand_write_column_address(u16 address);
/* д�е�ַ */
void nand_write_row_address(u16 address);
/* ������״̬ */
u8 nand_read_status(void);
/* ������ */
u8 nand_erase_block(u32 address);
/* дҳ��������벿�֣� */
u8 nand_write_page_end(void);
/* �������־ */
u8 check_invalid_block(u16 block_num);
/* ��ǻ��� */
void invalid_block_mark(u16 block_num); 
/* ��д��� */
u8 nand_copy_back(u32 source_address,u32 destination_address);

#endif 
