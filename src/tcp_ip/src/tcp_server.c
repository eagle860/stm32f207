/*********************************************
文件名：tcp_ip.c
版本号：v1.0
版权说明：
作者：蔡和
编写日期：2011/08/30
简要描述：tcp发送函数外部接口
修改人：
修改日期:
修改记录：
********************************************/

#include "tcp_server.h"
#include "public.h"
#include "tcp_ip.h"

/* tcp pcb global pointer */
struct tcp_pcb *p_tcp_pcb_server1 = NULL;	 /* 与PC交换业务数据用 */
struct tcp_pcb *p_tcp_pcb_server2 = NULL;	 /* 升级用 */ 

/******************************************
* 函数名称：tcp初始化
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void tcp_server_init(void)
{	
	/* tcp server1 initial */
	/* 与PC业务数据交换用服务器 */
	tcp_server1_init();
	/* tcp server2 initial */
	/* 升级用服务器 */
	tcp_server2_init();
}

/******************************************
* 函数名称：tcp 关闭连接控制块地址释放
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void release_tcp_pcb(struct tcp_pcb *tpcb)
{
	if(tpcb == p_tcp_pcb_server1)
	{
		p_tcp_pcb_server1 = NULL;

        if(TRUE == server1_new_connect.new_flag)
        {
            p_tcp_pcb_server1 = server1_new_connect.pcb;
            server1_new_connect.pcb = NULL;
            server1_new_connect.new_flag = FALSE;
        }
	}

	if(tpcb == p_tcp_pcb_server2)
	{
		p_tcp_pcb_server2 = NULL;

        if(TRUE == server2_new_connect.new_flag)
        {
            p_tcp_pcb_server2 = server2_new_connect.pcb;
            server2_new_connect.pcb = NULL;
            server2_new_connect.new_flag = FALSE;
        }
	}
}
