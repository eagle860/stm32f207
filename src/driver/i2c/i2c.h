#ifndef _I2C_H
#define _I2C_H


#include "public.h"


//------------------------------------------------------------------------------
// 宏定义
#define I2C_SPEED                    100000
#define I2C_SLAVE_ADDRESS7           0xA0
#define I2C_TIME_OUT                 500                     // 根据实际芯片确定
#define ADDRESS8

// 管脚映射参考doc目录下的STM32F207VE.pdf
#define I2C_PORT                     I2C3
#define I2C_CLK                      RCC_APB1Periph_I2C3
#define I2C_SCL_PIN                  GPIO_Pin_8
#define I2C_SCL_GPIO_PORT            GPIOA
#define I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define I2C_SCL_SOURCE               GPIO_PinSource8
#define I2C_SCL_AF                   GPIO_AF_I2C3
#define I2C_SDA_PIN                  GPIO_Pin_9
#define I2C_SDA_GPIO_PORT            GPIOC
#define I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define I2C_SDA_SOURCE               GPIO_PinSource9
#define I2C_SDA_AF                   GPIO_AF_I2C3


//------------------------------------------------------------------------------
// 内联函数，检测I2C总线事件
void I2C_init(void);
static __INLINE void I2C_reset(void)
{
    u8 i;
    GPIO_InitTypeDef GPIO_InitStructure;

    I2C_DeInit(I2C_PORT);

    RCC_AHB1PeriphClockCmd(I2C_SCL_GPIO_CLK, ENABLE);
    RCC_AHB1PeriphClockCmd(I2C_SDA_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(I2C_SDA_GPIO_PORT, I2C_SDA_PIN);
    for (i = 0; i < 9; i++)
    {
        GPIO_SetBits(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);
        delay_us(5);
        GPIO_ResetBits(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);
        delay_us(5);
    }
    GPIO_SetBits(I2C_SCL_GPIO_PORT, I2C_SCL_PIN);
    delay_us(5);
    GPIO_ResetBits(I2C_SDA_GPIO_PORT, I2C_SDA_PIN);
    delay_us(5);
    GPIO_SetBits(I2C_SDA_GPIO_PORT, I2C_SDA_PIN);
    
    I2C_init();
    
    delay_us(5000);
}
//------------------------------------------------------------------------------
// 内联函数，检测I2C总线状态
static __INLINE u8 __I2C_status_check(u32 I2C_FLAG)
{
    u32 time_out = 0;
    /* While the bus is busy */
    while(I2C_GetFlagStatus(I2C_PORT, I2C_FLAG))
    {
        if(time_out++ == I2C_TIME_OUT)
        {
            /* STOP condition */    
            I2C_reset();
            return FALSE;
        }
    }
    return TRUE;
}
//------------------------------------------------------------------------------
// 内联函数，检测I2C总线事件
static __INLINE u8 __I2C_event_check(u32 I2C_EVENT)
{
    u32 time_out = 0;
    /* While the bus is busy */
    while(!I2C_CheckEvent(I2C_PORT, I2C_EVENT))
    {
        if(time_out++ == I2C_TIME_OUT)
        {
            /* STOP condition */    
            I2C_reset();
            return FALSE;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
// 接口函数
void I2C_init(void);
#ifdef ADDRESS8
u8 I2C_buffer_write(const u8* p_data_buf, u8 write_addr,
                    u16 size, u8 slave_device_addr);
u8 I2C_buffer_read(u8* p_resp_buf, u8 read_addr,
                   u16 size, u8 slave_device_addr);
#elif defined ADDRESS16
u8 I2C_buffer_write(const u8* p_data_buf, u16 write_addr,
                    u16 size, u8 slave_device_addr);
u8 I2C_buffer_read(u8* p_resp_buf, u16 read_addr,
                   u16 size, u8 slave_device_addr);
#else
    #error "you must define ADDRESS8 or ADDRESS16"
#endif


#endif //~ _I2C_H
