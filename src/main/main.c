/*******************************************************************************
�ļ�����main.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ���ռ�
��д���ڣ�2011/08/29
��Ҫ������ϵͳ������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
*******************************************************************************/
#include "public.h"
#include "app.h"


OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE];


s32 main(void)
{
	//SystemInit();                  // ��ʼ��ϵͳʱ�ӣ�ϵͳ��main����ǰ�Զ�����
    std_periph_init();               // ��ʼ����׼����
    
    OSInit();
	OSTaskCreate(startup_task,
	             (void *)0,
				 &startup_task_stk[STARTUP_TASK_STK_SIZE - 1],
				 STARTUP_TASK_PRIO);
	OSStart();
}
