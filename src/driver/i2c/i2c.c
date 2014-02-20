/*******************************************************************************
文件名：i2c.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/08/30
简要描述：i2c初始化及驱动
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "i2c.h"


//------------------------------------------------------------------------------
// 初始化I2C3
//------------------------------------------------------------------------------
static void I2C3_init(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(I2C_CLK, ENABLE);
    /* I2C_SCL_GPIO_CLK and I2C_SDA_GPIO_CLK Periph clock enable */
    RCC_AHB1PeriphClockCmd(I2C_SCL_GPIO_CLK | I2C_SDA_GPIO_CLK, ENABLE);
    /* Reset I2C CLK */
    RCC_APB1PeriphResetCmd(I2C_CLK, ENABLE);
    RCC_APB1PeriphResetCmd(I2C_CLK, DISABLE);

    /* Configure I2C pins: SCL */
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
    /* Configure I2C pins: SDA */
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_PIN;
    GPIO_Init(I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
    /* Connect PXx to I2C_SCL*/
    GPIO_PinAFConfig(I2C_SCL_GPIO_PORT, I2C_SCL_SOURCE, I2C_SCL_AF);
    /* Connect PXx to I2C_SDA*/
    GPIO_PinAFConfig(I2C_SDA_GPIO_PORT, I2C_SDA_SOURCE, I2C_SDA_AF);

    /* I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
    I2C_Init(I2C_PORT, &I2C_InitStructure);

    I2C_Cmd(I2C_PORT, ENABLE);
}
/*******************************************************************************
* 函数名称：I2C_init
* 功能描述：初始化I2C总线
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：I2C3_init
* 描述 ：
*******************************************************************************/
void I2C_init(void)
{
    I2C3_init();
}
/*******************************************************************************
* 函数名称：I2C_buffer_write
* 功能描述：多字节写入
* 入口参数：p_data_buf数据首地址
*           write_addr写入数据到设备地址
*           size写数据大小，最好控制在256之内
*           slave_device_addr从设备地址
* 出口参数：
* 返 回 值：TRUE/FALSE
* 全局变量：无
* 调用函数：无
*******************************************************************************/
#ifdef ADDRESS8
u8 I2C_buffer_write(const u8* p_data_buf, u8 write_addr,
                    u16 size, u8 slave_device_addr)
#elif defined ADDRESS16
u8 I2C_buffer_write(const u8* p_data_buf, u16 write_addr,
                    u16 size, u8 slave_device_addr)
#else
    #error "you must define ADDRESS8 or ADDRESS16 in i2c.h"
#endif
{
    /* 检查指针 */
    if (NULL == p_data_buf)
    {
        return FALSE;
    }

    /* While the bus is busy */
    if (TRUE != __I2C_status_check(I2C_FLAG_BUSY))
    {
        return FALSE;
    }

    /* Send START condition */
    I2C_GenerateSTART(I2C_PORT, ENABLE);

    /* Test on EV5 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_MODE_SELECT))
    {
        return FALSE;
    }

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_PORT, slave_device_addr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        return FALSE;
    }

#ifdef ADDRESS8
    /* Send the I2C chip's internal address to write to */
    I2C_SendData(I2C_PORT, write_addr);

    /* Test on EV8 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        return FALSE;
    }
#elif defined ADDRESS16
    /* Send the EEPROM's internal MSB address to write to */
    I2C_SendData(I2C_PORT, (u8)((write_addr & 0xFF00) >> 8));  

    /* Test on EV8 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        return FALSE;
    }

    /* Send the EEPROM's internal LSB address to write to */
    I2C_SendData(I2C_PORT, (u8)(write_addr & 0x00FF));  

    /* Test on EV8 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        return FALSE;
    }
#else
    #error "you must define ADDRESS8 or ADDRESS16 in i2c.h"
#endif

    /* While there is data to be written */
    while(size--)
    {
        /* Send the current byte and Point to the next byte */
        I2C_SendData(I2C_PORT, *p_data_buf++);

        /* Test on EV8 and clear it */
        if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            return FALSE;
        }
    }

    /* Send STOP condition */

    I2C_GenerateSTOP(I2C_PORT, ENABLE);
    return TRUE;
}
/*******************************************************************************
* 函数名称：I2C_buffer_read
* 功能描述：多字节读取
* 入口参数：p_resp_buf数据返回首地址
*           read_addr数据读取地址
*           size读取数据大小，最好控制在256之内
*           slave_device_addr从设备地址
* 出口参数：p_resp_buf
* 返 回 值：TRUE/FALSE
* 全局变量：无
* 调用函数：无
*******************************************************************************/
#ifdef ADDRESS8
u8 I2C_buffer_read(u8* p_resp_buf, u8 read_addr,
                   u16 size, u8 slave_device_addr)
#elif defined ADDRESS16
u8 I2C_buffer_read(u8* p_resp_buf, u16 read_addr,
                   u16 size, u8 slave_device_addr)
#else
    #error "you must define ADDRESS8 or ADDRESS16 in i2c.h"
#endif
{
    /* 检查指针不为空 */
    if (NULL == p_resp_buf)
    {
        return FALSE;
    }

    if (TRUE != __I2C_status_check(I2C_FLAG_BUSY))
    {
        return FALSE;
    }

    /* Send START condition */
    I2C_GenerateSTART(I2C_PORT, ENABLE);

    /* Test on EV5 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_MODE_SELECT))
    {
        return FALSE;
    }

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C_PORT, slave_device_addr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        return FALSE;
    }

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C_PORT, ENABLE);

#ifdef ADDRESS8
    /* Send the I2C chip's internal address to read from */
    I2C_SendData(I2C_PORT, read_addr);

    /* Test on EV8 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        return FALSE;
    }
#elif defined ADDRESS16
    /* Send the EEPROM's internal MSB address to read from */
    I2C_SendData(I2C_PORT, (u8)((read_addr & 0xFF00) >> 8));  

    /* Test on EV8 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        return FALSE;
    }

    /* Send the EEPROM's internal LSB address to read from */
    I2C_SendData(I2C_PORT, (u8)(read_addr & 0x00FF));  

    /* Test on EV8 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        return FALSE;
    }
#else
    #error "you must define ADDRESS8 or ADDRESS16 in i2c.h"
#endif

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C_PORT, ENABLE);

    /* Test on EV5 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_MODE_SELECT))
    {
        return FALSE;
    }

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C_PORT, slave_device_addr, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        return FALSE;
    }

    /* While there is data to be read */
    while(size)
    {
        if(1 == size)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C_PORT, DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C_PORT, ENABLE);
        }

        /* Test on EV7 and clear it */
        if (TRUE != __I2C_event_check(I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            return FALSE;
        }

        /* Read a byte from the EEPROM and Point to the next location*/
        *p_resp_buf++ = I2C_ReceiveData(I2C_PORT);

        /* Decrement the read bytes counter */
        size--;
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C_PORT, ENABLE);

    return TRUE;
}

//~ end
