#ifndef _PUBLISH_H
#define _PUBLISH_H


#include "stm32f2xx_conf.h"
#include "stm32f2xx.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


extern vu32 g_1ms_cnt;

//------------------------------------------------------------------------------
// 宏定义
typedef enum
{
    FALSE,
    TRUE
} BOOL;

#define _DEBUG


// 外设优先级，高四位为PreemptionPriority(可抢占)，低四位为SubPriority(不可抢占)
#define NVIC_PRO_GROUP           NVIC_PriorityGroup_1
#define HIGHEST_PRI              0x00
#define LOWEST_PRI               0x17        // 根据NVIC_PRO_GROUP确定最低优先级
enum std_periph_priority
{
    /* 根据实际需要确定各中断优先级 */
    CAN1_PRI         = LOWEST_PRI,
    EXTI15_10_PRI    = LOWEST_PRI,
    SC_PRI           = LOWEST_PRI,
    SPI1_PRI         = LOWEST_PRI,
    SPI2_PRI         = LOWEST_PRI,
    TIM3_PRI         = LOWEST_PRI,
    TIM4_PRI         = LOWEST_PRI,
    TIM6_PRI         = LOWEST_PRI,
    TIM7_PRI         = LOWEST_PRI,
    USART1_PRI       = LOWEST_PRI,
    USART3_PRI       = LOWEST_PRI,
    USART4_PRI       = LOWEST_PRI
};


//------------------------------------------------------------------------------
#define offsetof(type, member)      ((size_t)(&((type *)0)->member))

/* dimension of (or number of elements) of an array */
#define dimof(array)    (sizeof(array) / sizeof(array[0]))

/* Maximum & Minimum */
#define MAX(a, b)       ((a) >= (b) ? (a) : (b))
#define MIN(a, b)       ((a) <= (b) ? (a) : (b))

#define get_current_1ms_cnt(time)  {(time) = (g_1ms_cnt);}  // 获得当前1ms定时值

#define bcd2str(bcd, n, s)      do {                                            \
    const u8 *_bcd = (bcd);                                                     \
    u8 _n = (n);                                                                \
    char *_s = (s);                                                             \
                                                                                \
    if (is_valid_bcd(_bcd, _n))                                                 \
    {                                                                           \
        return;                                                                 \
    }                                                                           \
    hex2str(_bcd, _n, _s);                                                      \
} while (0)

//------------------------------------------------------------------------------
BOOL check_timeout(u32 start_time, u32 timeout);
void std_periph_init(void);
void delay_us(vu32 count);
u8 checksum8(u8 cs, const void *buf, u16 n);
u8 is_valid_bcd(const u8 array[], u8 n);
void uint2bcd(u32 a_uint, u8 bcd[], u8 n);
u32 bcd2uint(const u8 bcd[], u8 n);
void bcd_plus_plus(u8 bcd[], u8 len);
s8 arrcmp(const u8 a[], const u8 b[], s8 n);
void hex2str(const u8 hex[], u8 n, char *s);
void bin2str(const u8 bin[], u8 n, char *s);
void dec2str(s32 dec, u8 n_bytes_of_BCD, char *s);


#endif //~ _PUBLISH_H
