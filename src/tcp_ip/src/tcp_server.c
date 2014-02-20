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

#include "tcp_server.h"
#include "public.h"
#include "tcp_ip.h"

/* tcp pcb global pointer */
struct tcp_pcb *p_tcp_pcb_server1 = NULL;	 /* ��PC����ҵ�������� */
struct tcp_pcb *p_tcp_pcb_server2 = NULL;	 /* ������ */ 

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
void tcp_server_init(void)
{	
	/* tcp server1 initial */
	/* ��PCҵ�����ݽ����÷����� */
	tcp_server1_init();
	/* tcp server2 initial */
	/* �����÷����� */
	tcp_server2_init();
}

/******************************************
* �������ƣ�tcp �ر����ӿ��ƿ��ַ�ͷ�
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
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
