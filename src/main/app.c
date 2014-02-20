#include "app.h"
#include "app_cfg.h"
#include "tcp_server.h"
#include "tcp_ip.h"


static OS_STK led_toggle_stk[LED_TOGGLE_STK_SIZE];
static OS_STK tcp_ip_stk[TCP_IP_STK_SIZE];


static void led_toggle(void *p_arg)
{
    for (;;)
    {
        GPIO_ToggleBits(GPIOF, GPIO_Pin_8);
        OSTimeDly(500);
    }
}

static void tcp_ip(void *p_arg)
{
    for (;;)
    {
        tcp_ip_process();
        OSTimeDly(10);
    }
}

static void systick_init(void)
{
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);
}

void startup_task(void *p_arg)
{
    INT8U err;
    systick_init();     /* Initialize the SysTick. */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();      /* Determine CPU capacity. */
#endif
    /* TODO: create application tasks here */
    err = OSTaskCreate(led_toggle, (void *)0,
                       &led_toggle_stk[LED_TOGGLE_STK_SIZE-1], LED_TOGGLE_PRIO);      

    err = OSTaskCreate(tcp_ip, (void *)0,
                       &tcp_ip_stk[TCP_IP_STK_SIZE-1], TCP_IP_PRIO);

    if (OS_ERR_NONE != err)
        while(1)
            ;
    OSTaskDel(OS_PRIO_SELF);
}
