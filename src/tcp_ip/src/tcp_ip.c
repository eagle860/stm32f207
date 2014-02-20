/*********************************************
�ļ�����tcp_ip.c
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/08/30
��Ҫ������tcp���ͺ����ⲿ�ӿ�
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/
#include "tcp_ip.h"
#include "netconf.h"
#include "stm32f2x7_eth.h"
#include "stm32f2x7_eth_bsp.h"
#include "tcp_server.h"
#include "main.h"
#include "public.h"

/*-------------------------------��������------------------------------------*/
volatile u32_t LocalTime = 0;

#ifdef OWN_TCP_DEBUG
	static u32_t fail_write = 0;
#endif

u8_t mac_address[6] = {0x08,0x00,0x17,0x00,0x5C,0x90};
u8_t ip_address[4] = {192,168,1,181};
u8_t gate_address[4] = {192,168,1,1};
u8_t mask_address[4] = {255,255,255,0};

/*-----------------------------�ڲ���������----------------------------------*/
/*---------------------------�ڲ����õ��������------------------------------*/
static void tcp_low_init(void);

static void amount_connect_process(void);

/******************************************
* �������ƣ�tcp������
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��   ��������ѭ���е��øú���
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
* �������ƣ�tcp��ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��   
*******************************************/
void TCP_init(void)
{
	tcp_low_init();	

	tcp_server_init();
}

/******************************************
* �������ƣ�tcp�ײ��ʼ��
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��   
*******************************************/
static void tcp_low_init(void)
{
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
    ETH_BSP_Config();   
    /* Initilaize the LwIP stack */
    LwIP_Init();
}

/******************************************
* �������ƣ�tcp���ͺ���
* ������������ָ�����ȼ�����ʼ��ַ������
*			ͨ��ָ���ķ������˿ڷ��ͳ�ȥ
* ��ڲ�����@tcp_send_buf �������ݻ����ַ
*			@send_len     �������ݳ���
*			@TCP_PORT	  �������˿ں�
*				ֻ��ΪTCP_PORT1��TCP_PORT2
* ���ڲ�������
* �� �� ֵ��TRUE�� ���ͳɹ�
*			FALSE������ʧ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
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
* �������ƣ�tcp ���ƿ��Ӧ�˿�
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
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
* �������ƣ�tcp�ڲ����õķ��ͺ���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
u8_t tcp_ip_out(struct tcp_pcb *send_tcp_pcb, const u8_t *tcp_send_buf, u16_t send_len)
{
	err_t tcp_write_err  = ERR_OK;
	err_t tcp_out_err = ERR_OK;
    u8_t  tcp_flags;

	/* ����δ���� */
	if(NULL == send_tcp_pcb) 
	{
		SERVER_DEBUG(("\nconnection has not establis!\n"));
		return FALSE;
	}
	/* �����ѽ��� */
	else 
	{
		/* ������д�뷢�Ͷ��� */
		tcp_write_err = tcp_write(send_tcp_pcb, tcp_send_buf, send_len, 1);  

		/* ����д�뷢�Ͷ����Ƿ�ɹ� */
		if(ERR_OK == tcp_write_err) /* ���ݳɹ�д�뷢�Ͷ��� */
		{
			tcp_flags = send_tcp_pcb->flags;
            send_tcp_pcb->flags |= TF_NAGLEMEMERR;
            tcp_out_err = tcp_output(send_tcp_pcb);
            send_tcp_pcb->flags = tcp_flags;

			/* ���ݷ����Ƿ�ɹ� */
			if(ERR_OK != tcp_out_err)
			{
				SERVER_DEBUG(("\nsend failure!\n"));
			}

			return TRUE;
		}
		else /* ����д�뷢�Ͷ���ʧ�� */
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
* �������ƣ�tcp������ʱ�Ĵ���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
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
* �������ƣ�tcp��ȡ�������ú���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
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
