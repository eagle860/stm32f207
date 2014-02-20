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
#include "tcp_ip.h"
#include "netconf.h"
#include "stm32f2x7_eth.h"
#include "stm32f2x7_eth_bsp.h"
#include "tcp_server.h"
#include "main.h"
#include "public.h"

/*-------------------------------变量定义------------------------------------*/
volatile u32_t LocalTime = 0;

#ifdef OWN_TCP_DEBUG
	static u32_t fail_write = 0;
#endif

u8_t mac_address[6] = {0x08,0x00,0x17,0x00,0x5C,0x90};
u8_t ip_address[4] = {192,168,1,181};
u8_t gate_address[4] = {192,168,1,1};
u8_t mask_address[4] = {255,255,255,0};

/*-----------------------------内部函数声明----------------------------------*/
/*---------------------------内部调用的输出函数------------------------------*/
static void tcp_low_init(void);

static void amount_connect_process(void);

/******************************************
* 函数名称：tcp处理函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：   必须在主循环中调用该函数
*******************************************/
void tcp_ip_process(void)
{
	/* check if any packet received */
	if (ETH_CheckFrameReceived())
	{ 
	  /* process received ethernet packet */
	  LwIP_Pkt_Handle();
	}
	/* handle periodic timers for LwIP */
	LwIP_Periodic_Handle(LocalTime);

    amount_connect_process();
}

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
void TCP_init(void)
{
	tcp_low_init();	

	tcp_server_init();
}

/******************************************
* 函数名称：tcp底层初始化
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：   
*******************************************/
static void tcp_low_init(void)
{
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
    ETH_BSP_Config();   
    /* Initilaize the LwIP stack */
    LwIP_Init();
}

/******************************************
* 函数名称：tcp发送函数
* 功能描述：把指定长度及及起始地址的数据
*			通过指定的服务器端口发送出去
* 入口参数：@tcp_send_buf 发送数据缓存地址
*			@send_len     发送数据长度
*			@TCP_PORT	  服务器端口号
*				只能为TCP_PORT1或TCP_PORT2
* 出口参数：无
* 返 回 值：TRUE， 发送成功
*			FALSE，发送失败
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8_t tcp_ip_write(const	u8_t *tcp_send_buf, u16_t send_len, const u16_t TCP_PORT)
{
	switch(TCP_PORT)
	{
		case TCP_PORT1:
			return tcp_ip_out(p_tcp_pcb_server1,tcp_send_buf,send_len);

		case TCP_PORT2:
			return tcp_ip_out(p_tcp_pcb_server2,tcp_send_buf,send_len);

		default:
			SERVER_DEBUG(("\nthe port is not exit!\n"));
			break;
	}

	return FALSE;
}

/******************************************
* 函数名称：tcp 控制块对应端口
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u16_t pcb_to_port(const struct tcp_pcb * tpcb)
{
	if(tpcb == p_tcp_pcb_server1)
	{
		return TCP_PORT1;
	}
	else if(tpcb == p_tcp_pcb_server2)
	{
		return TCP_PORT2;
	}
	else
	{
		SERVER_DEBUG(("\nthe connection does not exit!\n"));
		return NONE_PORT;
	}
}

/******************************************
* 函数名称：tcp内部调用的发送函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
u8_t tcp_ip_out(struct tcp_pcb *send_tcp_pcb, const u8_t *tcp_send_buf, u16_t send_len)
{
	err_t tcp_write_err  = ERR_OK;
	err_t tcp_out_err = ERR_OK;
    u8_t  tcp_flags;

	/* 连接未建立 */
	if(NULL == send_tcp_pcb) 
	{
		SERVER_DEBUG(("\nconnection has not establis!\n"));
		return FALSE;
	}
	/* 连接已建立 */
	else 
	{
		/* 把数据写入发送队列 */
		tcp_write_err = tcp_write(send_tcp_pcb, tcp_send_buf, send_len, 1);  

		/* 数据写入发送队列是否成功 */
		if(ERR_OK == tcp_write_err) /* 数据成功写入发送队列 */
		{
			tcp_flags = send_tcp_pcb->flags;
            send_tcp_pcb->flags |= TF_NAGLEMEMERR;
            tcp_out_err = tcp_output(send_tcp_pcb);
            send_tcp_pcb->flags = tcp_flags;

			/* 数据发送是否成功 */
			if(ERR_OK != tcp_out_err)
			{
				SERVER_DEBUG(("\nsend failure!\n"));
			}

			return TRUE;
		}
		else /* 数据写入发送队列失败 */
		{
			tcp_flags = send_tcp_pcb->flags;
            send_tcp_pcb->flags |= TF_NAGLEMEMERR;
            (void) tcp_send_ctrl(send_tcp_pcb,TF_NAGLEMEMERR);
            send_tcp_pcb->flags = tcp_flags;

            tcp_flags = send_tcp_pcb->flags;
            send_tcp_pcb->flags |= TF_NAGLEMEMERR;
			(void) tcp_output(send_tcp_pcb);
            send_tcp_pcb->flags = tcp_flags;

			#ifdef OWN_TCP_DEBUG
				fail_write ++;
			#endif

			SERVER_DEBUG(("\nwrite failure, %d\n",fail_write));
			return FALSE;
		}
	}
}

/******************************************
* 函数名称：tcp多连接时的处理
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void amount_connect_process(void)
{
    if(TRUE == server1_new_connect.new_flag)
    {
        if(p_tcp_pcb_server1->keep_cnt_sent > TCP_KEEPCNT_DEFAULT)
        {
            tcp_abort(p_tcp_pcb_server1);
            p_tcp_pcb_server1 = server1_new_connect.pcb;
            server1_new_connect.pcb = NULL;
            server1_new_connect.new_flag = FALSE;
        }
        else if(0 == p_tcp_pcb_server1->keep_cnt_sent)
        {
            tcp_abort(server1_new_connect.pcb);
            server1_new_connect.pcb = NULL;
            server1_new_connect.new_flag = FALSE;
        }
        else
        {
            if(check_timeout(server1_new_connect.settime, KEEP_LIVE_INTERVAL))
            {
                get_current_1ms_cnt(server1_new_connect.settime);
                tcp_keepalive(p_tcp_pcb_server1);
		        p_tcp_pcb_server1->keep_cnt_sent ++;
            }
        }
    }

    if(TRUE == server2_new_connect.new_flag)
    {
        if(p_tcp_pcb_server2->keep_cnt_sent > TCP_KEEPCNT_DEFAULT)
        {
            tcp_abort(p_tcp_pcb_server2);
            p_tcp_pcb_server2 = server2_new_connect.pcb;
            server2_new_connect.pcb = NULL;
            server2_new_connect.new_flag = FALSE;
        }
        else if(0 == p_tcp_pcb_server2->keep_cnt_sent)
        {
            tcp_abort(server2_new_connect.pcb);
            server2_new_connect.pcb = NULL;
            server2_new_connect.new_flag = FALSE;
        }
        else
        {
            if(check_timeout(server2_new_connect.settime, KEEP_LIVE_INTERVAL))
            {
                get_current_1ms_cnt(server2_new_connect.settime);
                tcp_keepalive(p_tcp_pcb_server2);
		        p_tcp_pcb_server2->keep_cnt_sent ++;
            }
        }
    }
}

#ifdef USER_DEFINE_NET
/******************************************
* 函数名称：tcp获取参数配置函数
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void get_net_settings(const u8 *mac,const u8 *local_ip,const u8 *gate_way)
{
	mac_address[0] = mac[0];
	mac_address[1] = mac[1];
	mac_address[2] = mac[2];
	mac_address[3] = mac[3];
	mac_address[4] = mac[4];
	mac_address[5] = mac[5];

	ip_address[0] = local_ip[0];
	ip_address[1] = local_ip[1];
	ip_address[2] = local_ip[2];
	ip_address[3] = local_ip[3];
	
	gate_address[0] = gate_way[0];
	gate_address[1] = gate_way[1];
	gate_address[2] = gate_way[2];
	gate_address[3] = gate_way[3];	
}
#endif
