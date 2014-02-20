/*******************************************************************************
文件名：public.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/08/29
简要描述：公共函数及初始化函数
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "public.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "adc.h"
#include "can.h"
#include "fsmc.h"
#include "exti.h"
#include "smartcard.h"
#include "tcp_ip.h"
#include "phy.h"


/*******************************************************************************
* 函数名称：std_periph_init
* 功能描述：标准外设初始化
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：
* 调用函数：各外设的初始化程序
* 描述 ：
*******************************************************************************/
void std_periph_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PRO_GROUP);
    GPIO_init();
    USART_init();
//    I2C_init();
//    SC_init();
    TIM_init();
//    SPI_init();
//    ADC_init();
//    EXTI_init();
//    FSMC_init();
//    CAN_init();
    phy_interrupt_init();
    TCP_init();
}
/*******************************************************************************
* 函数名称：delay_us
* 功能描述：延时程序
* 入口参数：count 指定的微秒数
* 出口参数：无
* 返 回 值：无
* 全局变量：
* 调用函数：
* 描述 ：当前系统使用晶振频率4.096M，主时钟98.304M，APB1四分频24.576M，
*        APB2二分频49.152M，如更改晶振或时钟配置，需要重新调整此函数
*******************************************************************************/
void delay_us(vu32 count)
{
    vu32 i;
    vu16 j;

    for(i = 0; i < count; i++)
    { 
        for(j = 0; j < 19; j++)
        {
        }
    }
}
/******************************************
* 函数名称：check_timeout
* 入口参数：u32 start_time, u32 timeout
* 返 回 值：
* 全局变量：无
* 调用函数：
* 功能描述：判断是否超时
* 备    注：
*******************************************/
BOOL check_timeout(u32 start_time, u32 timeout)
{
    u32 current_time = 0;

    get_current_1ms_cnt(current_time);

    if (current_time >= start_time)
    {
        if (current_time - start_time >= timeout)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        if (current_time + (0xFFFFFFFF - start_time) >= timeout)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
}
//------------------------------------------------------------------------------
u8 checksum8(u8 cs, const void *buf, u16 n)
{
    u16 i;

    for (cs = ~cs, i = 0; i < n; ++i)
    {
        cs += ((u8 *)buf)[i];
    }

    return ~cs;
}
//------------------------------------------------------------------------------
u8 is_valid_bcd(const u8 array[], u8 n)
{
    u8 i;

    for (i = 0; i < n; ++i)
    {
        u8 dd = array[i];
        u8 _d = dd & (u8)0x0F;
        u8 d_ = dd & (u8)0xF0;
        if (_d > (u8)0x09 || d_ > (u8)0x90)
        {
            return FALSE;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
void uint2bcd(u32 a_uint, u8 bcd[], u8 n)
{
    u8 i;

    for (i = 0; i < n; ++i)
    {
        u8 dd = (u8)(a_uint % 100);
        a_uint /= 100;
        bcd[i] = (u8)((dd / 10) << 4) + (u8)(dd % 10);
    }
}
//------------------------------------------------------------------------------
u32 bcd2uint(const u8 bcd[], u8 n)
{
    u32 a_uint;
    u8 i;

    if (FALSE == is_valid_bcd(bcd, n))
    {
        return FALSE;
    }

    a_uint = 0;
    for (i = n; i > (u8)0; --i)
    {
        u8 dd = bcd[i-1];
        a_uint *= 100;
        a_uint += (dd >> 4) * 10 + (dd & (u8)0x0F);
    }
    return a_uint;
}
//------------------------------------------------------------------------------
void bcd_plus_plus(u8 bcd[], u8 len)
{
    u8 _d, d_;
    u8 carry;
    u8 i;

    for (i = 0; i < len; ++i)
    {
        _d = *(bcd + i) & 0x0F;
        d_ = *(bcd + i) & 0xF0;
        carry = FALSE;
        _d = _d + 1;
        if (_d >= 0x0A)
        {
            _d = 0;
            d_ = d_ + 0x10;
            if (d_ >= 0xA0)
            {
                d_ = 0;
                carry = TRUE;
            }
        }
        *(bcd + i) = d_ + _d;
        if (FALSE == carry)
        {
            break;
        }
    }
}
//------------------------------------------------------------------------------
s8 arrcmp(const u8 a[], const u8 b[], s8 n)
{
    s8 i;

    for (i = n - 1; i >= (s8)0; --i)
    {
        if (a[i] > b[i])
        {
            return 1;
        }
        if (a[i] < b[i])
        {
            return -1;
        }
    }
    return 0;
}
const char hex2char[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'b', 'C', 'd', 'E', 'F',
};
//------------------------------------------------------------------------------
void hex2str(const u8 hex[], u8 n, char *s)
{
    s8 i;

    for (i = (s8)(n - 1); i >= (s8)0; --i)
    {
        *s++ = hex2char[hex[i] >> 4];
        *s++ = hex2char[hex[i] & (u8)0x0F];
    }
    *s = '\0';
}
//------------------------------------------------------------------------------
void bin2str(const u8 bin[], u8 n, char *s)
{
    s8 i;

    for (i = (s8)(n - 1); i >= (s8)0; --i)
    {
        u8 msk;

        for (msk = 0x80; msk != (u8)0x00; msk >>= 1)
        {
            if (bin[i] & msk)
            {
                *s = '1';
            }
            else
            {
                *s = '0';
            }
            ++s;
        }
    }
    *s = '\0';
}
//------------------------------------------------------------------------------
void dec2str(s32 dec, u8 n_bytes_of_BCD, char *s)
{
    u32 a_uint;
    u8 bcd[5];
    u8 negative;

    if (dec < 0)
    {
        a_uint = (u32)(-dec);
        negative = TRUE;
    }
    else
    {
        a_uint = (u32)dec;
        negative = FALSE;
    }
    if(n_bytes_of_BCD > (u8)sizeof(bcd))
    {
        return;
    }
    uint2bcd(a_uint, bcd, n_bytes_of_BCD);
    if (negative)
    {
        *s = '-';
        bcd2str(bcd, n_bytes_of_BCD, s + 1);
    }
    else
    {
        bcd2str(bcd, n_bytes_of_BCD, s);
    }
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
