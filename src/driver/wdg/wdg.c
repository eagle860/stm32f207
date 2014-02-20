/*******************************************************************************
文件名：wdg.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/10/24
简要描述：看门狗初始化
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "wdg.h"


static void IWDG_init(void)
{
    /* Enable the LSI oscillator ************************************************/
    RCC_LSICmd(ENABLE);
    
    /* Wait till LSI is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {}

    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    
    /* IWDG counter clock */
    IWDG_SetPrescaler(IWDG_PRESCALER);
    
    /* Set counter reload value to obtain 250ms IWDG TimeOut. */
    IWDG_SetReload(IWDG_RELOAD_TIM);
    
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}
/*******************************************************************************
* 函数名称：CAN_init
* 功能描述：CAN初始化
* 入口参数：无
* 出口参数：无
* 返 回 值：无
* 全局变量：无
* 调用函数：无
*******************************************************************************/
void WDG_init(void)
{
    IWDG_init();
}
