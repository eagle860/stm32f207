/*********************************************
文件名：tcp_ip.h
版本号：v1.0
版权说明：
作者：蔡和
编写日期：2011/08/30
简要描述：tcp发送函数外部接口
修改人：
修改日期:
修改记录：
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

/*-------------------------------------宏定义-----------------------------------------*/
#define TCP_PORT1		    3001			  /* 网口端口3001 */
#define TCP_PORT2		    3002			  /* 网口端口3002 */

#define NONE_PORT           0				  /* 非法端口 */

#define KEEP_LIVE_INTERVAL  500

/*-----------------------------------类型声明-----------------------------------------*/
struct new_connect
{
    u8_t   new_flag;
    u32_t  settime;
    struct tcp_pcb *pcb; 
};

/*---------------------------------全局变量声明---------------------------------------*/
extern volatile u32_t LocalTime;

/*-----------------------------------函数声明-----------------------------------------*/
/* tcp轮询函数，必须在主循环中调用 */
void tcp_ip_process(void);

void TCP_init(void);

u8_t tcp_ip_write(const	u8_t *tcp_send_buf, u16_t send_len, u16_t TCP_PORT);

u16_t pcb_to_port(const struct tcp_pcb * tpcb);

u8_t tcp_ip_out(struct tcp_pcb *send_tcp_pcb, const u8_t *tcp_send_buf, u16_t send_len);

#ifdef USER_DEFINE_NET
void get_net_settings(const u8_t *mac,const u8_t *local_ip,const u8_t *gate_way);
#endif

#endif
