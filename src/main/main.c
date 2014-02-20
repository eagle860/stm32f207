/*******************************************************************************
文件名：main.c
版本号：v1.0
版权说明：
作者：王占颍
编写日期：2011/08/29
简要描述：系统主函数
修改人：
修改日期:
修改记录：
*******************************************************************************/
#include "public.h"
#include "app.h"


OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];


s32 main(void)
{
	//SystemInit();                  // 初始化系统时钟，系统进main函数前自动调用
    std_periph_init();               // 初始化标准外设
    
    OSInit();
	OSTaskCreate(startup_task,
	             (void *)0,
				 &startup_task_stk[STARTUP_TASK_STK_SIZE - 1],
				 STARTUP_TASK_PRIO);
	OSStart();
}
