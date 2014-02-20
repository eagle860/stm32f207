/*********************************************
�ļ�����tcp_ip.h
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/08/30
��Ҫ������tcp���ͺ����ⲿ�ӿ�
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/
#ifndef _OWN_TCP_IP_H
#define _OWN_TCP_IP_H

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "main.h"
#include "public.h"


#define OWN_TCP_DEBUG

#ifdef OWN_TCP_DEBUG
	#ifdef SERIAL_DEBUG
		#define SERVER_DEBUG(message) do{ \
							printf message; \
							}while(0)
	#else
		#define	SERVER_DEBUG(message)
	#endif
#else
	#define	SERVER_DEBUG(message)
#endif

  

#define SYSTEMTICK_PERIOD_MS  10

#define USER_DEFINE_NET

#ifndef USER_DEFINE_NET  
/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   9
#define MAC_ADDR1   0
#define MAC_ADDR2   0
#define MAC_ADDR3   0
#define MAC_ADDR4   0
#define MAC_ADDR5   0
 
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   151

/* client destination address */
#define DEST_IP_ADDR0  192
#define DEST_IP_ADDR1  168
#define DEST_IP_ADDR2  1
#define DEST_IP_ADDR3  159

#define DEST_PORT  3005
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1  
#endif /* USER_DEFINE_NET */

#define RMII_MODE 

/*-------------------------------------�궨��-----------------------------------------*/
#define TCP_PORT1		    3001			  /* ���ڶ˿�3001 */
#define TCP_PORT2		    3002			  /* ���ڶ˿�3002 */

#define NONE_PORT           0				  /* �Ƿ��˿� */

#define KEEP_LIVE_INTERVAL  500

/*-----------------------------------��������-----------------------------------------*/
struct new_connect
{
    u8_t   new_flag;
    u32_t  settime;
    struct tcp_pcb *pcb; 
};

/*---------------------------------ȫ�ֱ�������---------------------------------------*/
extern volatile u32_t LocalTime;

/*-----------------------------------��������-----------------------------------------*/
/* tcp��ѯ��������������ѭ���е��� */
void tcp_ip_process(void);

void TCP_init(void);

u8_t tcp_ip_write(const	u8_t *tcp_send_buf, u16_t send_len, u16_t TCP_PORT);

u16_t pcb_to_port(const struct tcp_pcb * tpcb);

u8_t tcp_ip_out(struct tcp_pcb *send_tcp_pcb, const u8_t *tcp_send_buf, u16_t send_len);

#ifdef USER_DEFINE_NET
void get_net_settings(const u8_t *mac,const u8_t *local_ip,const u8_t *gate_way);
#endif

#endif
