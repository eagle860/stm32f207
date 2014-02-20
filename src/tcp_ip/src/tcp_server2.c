/**
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of and a contribution to the lwIP TCP/IP stack.
 *
 * Credits go to Adam Dunkels (and the current maintainers) of this software.
 *
 * Christiaan Simons rewrote this file to get a more stable echo example.
 *
 **/

 /* This file was modified by ST */


#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "tcp_server.h"
#include "main.h"
#include "tcp_ip.h"
#include <string.h>

#if LWIP_TCP

struct new_connect server2_new_connect = {FALSE, 0, NULL};
static struct tcp_pcb *tcp_server2_pcb = NULL;

static err_t tcp_server2_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server2_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void tcp_server2_error(void *arg, err_t err);
static err_t tcp_server2_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_server2_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_server2_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);

/**
  * @brief  Initializes the tcp echo server
  * @param  None
  * @retval None
  */
void tcp_server2_init(void)
{
  /* create new tcp pcb */
  /* struct tcp_pcb 的大小为160个字节 */
  tcp_server2_pcb = tcp_new();

  if (tcp_server2_pcb != NULL)
  {
    err_t err;
    
    /* bind echo_pcb to port 7 (ECHO protocol) */
    /* 升级服务器绑定端口号为TCP_PORT2 */
    err = tcp_bind(tcp_server2_pcb, IP_ADDR_ANY, TCP_PORT2);
    
    if (err == ERR_OK)
    {
      /* start tcp listening for echo_pcb */
      tcp_server2_pcb = tcp_listen(tcp_server2_pcb);
      
      /* initialize LwIP tcp_accept callback function */
      tcp_accept(tcp_server2_pcb, tcp_server2_accept);
    }
    else 
    {
      SERVER_DEBUG(("\ncan not bind pcb!\n"));
    }
  }
  else
  {
    SERVER_DEBUG(("\ncan not create new pcb!\n"));
  }
}

/**
  * @brief  This function is the implementation of tcp_accept LwIP callback
  * @param  arg: not used
  * @param  newpcb: pointer on tcp_pcb struct for the newly created tcp connection
  * @param  err: not used 
  * @retval err_t: error status
  */
static err_t tcp_server2_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* set priority for the newly accepted tcp connection newpcb */
  tcp_setprio(newpcb, TCP_PRIO_NORMAL);

  /* allocate structure es to maintain tcp connection informations */
  /* struct tcp_server_struct 的大小为0x0C，12个字节 */
  es = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct));

  if (es != NULL)
  {
    es->state = ES_ACCEPTED;
    es->pcb = newpcb;
    
    /* 在此注册的函数有： */
    /* 1、TCP_EVENT_RECV  */
    /* 2、TCP_EVENT_ERR   */
    /* 3、TCP_EVENT_POLL  */
    /* 4、TCP_EVENT_SENT  */
    /* pass newly allocated es structure as argument to newpcb */
    tcp_arg(newpcb, es);
    
    /* initialize lwip tcp_recv callback function for newpcb  */ 
    tcp_recv(newpcb, tcp_server2_recv);
    
    /* initialize lwip tcp_err callback function for newpcb  */
    tcp_err(newpcb, tcp_server2_error);

    /* initialize LwIP tcp_sent callback function */
    tcp_sent(newpcb, tcp_server2_sent);
    
    /* initialize lwip tcp_poll callback function for newpcb */
    tcp_poll(newpcb, tcp_server2_poll, 4);

	/* 建立连接时，保存tcp控制块地址 */
	if(NULL == p_tcp_pcb_server2)
	{
		p_tcp_pcb_server2 = newpcb;

		SERVER_DEBUG(("\nserver2 connected!\n"));
	}
	else
	{	
		if(TRUE != server2_new_connect.new_flag)
        {
            tcp_keepalive(p_tcp_pcb_server2);
    		p_tcp_pcb_server2->keep_cnt_sent++;
    
    		server2_new_connect.new_flag = TRUE;
            get_current_1ms_cnt(server2_new_connect.settime);
            server2_new_connect.pcb = newpcb;
        }
        else
        {
            tcp_abort(newpcb);
        }
				
		SERVER_DEBUG(("\nserver2 has connected!\n"));
	}
    
    ret_err = ERR_OK;
  }
  else
  {
    /* return memory error */
    ret_err = ERR_MEM;
	SERVER_DEBUG(("\nserver2 memory error,cannot allocate memory!\n"));
  }
  return ret_err;  
}


/**
  * @brief  This function is the implementation for tcp_recv LwIP callback
  * @param  arg: pointer on a argument for the tcp_pcb connection
  * @param  tpcb: pointer on the tcp_pcb connection
  * @param  pbuf: pointer on the received pbuf
  * @param  err: error information regarding the reveived pbuf
  * @retval err_t: error code
  */
static err_t tcp_server2_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_server_struct *es;
  err_t ret_err;
  u16_t pcb_port;

  pcb_port = pcb_to_port(tpcb);

  LWIP_ASSERT("arg != NULL",arg != NULL);
  
  es = (struct tcp_server_struct *)arg;
  
  /* if we receive an empty tcp frame from client => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    
    /* we're done sending, close connection */
    tcp_server2_connection_close(tpcb, es);

    ret_err = ERR_OK;
  }   
  /* else : a non empty frame was received from client but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      pbuf_free(p);
	  SERVER_DEBUG(("\nunknow error!\n"));
    }
    ret_err = err;
  }
  else if(es->state == ES_ACCEPTED)
  {
    /* first data chunk in p->payload */
    es->state = ES_RECEIVED;
	
    /* 添加解码回调函数 */	

    /* 测试代码 */
    (void) tcp_ip_write(p->payload, p->len, pcb_port);
    
    /* 数据处理完后，释放pbuf */
    /* 应用层已处理接收的数据，须调用该函数，更新窗口 */ 
    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);

    ret_err = ERR_OK;
  }
  else if (es->state == ES_RECEIVED)
  {
    /* 添加解码回调函数 */

    /* 测试代码 */
    (void) tcp_ip_write(p->payload, p->len, pcb_port);
    
    /* 数据处理完后，释放pbuf */
    /* 应用层已处理接收的数据，须调用该函数，更新窗口 */	
    tcp_recved(tpcb, p->tot_len);
    pbuf_free(p);

    ret_err = ERR_OK;
  }  
  /* data received when connection already closed */
  else
  {
    /* Acknowledge data reception */
    tcp_recved(tpcb, p->tot_len);
    
    /* free pbuf and do nothing */
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_err callback function (called
  *         when a fatal tcp_connection error occurs. 
  * @param  arg: pointer on argument parameter 
  * @param  err: not used
  * @retval None
  */
static void tcp_server2_error(void *arg, err_t err)
{
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(err);

  es = (struct tcp_server_struct *)arg;

  /* error detected, release pcb pointer */
  release_tcp_pcb(es->pcb);

  if (es != NULL)
  {
    /*  free es structure */
    mem_free(es);
  }
  
  SERVER_DEBUG(("\nsever2 error detected, connection released!\n"));
}

/**
  * @brief  This function implements the tcp_poll LwIP callback function
  * @param  arg: pointer on argument passed to callback
  * @param  tpcb: pointer on the tcp_pcb for the current tcp connection
  * @retval err_t: error code
  */
static err_t tcp_server2_poll(void *arg, struct tcp_pcb *tpcb)
{
  err_t ret_err;
  struct tcp_server_struct *es;

//  SERVER_DEBUG(("\nserver2 in poll!\n"));
  es = (struct tcp_server_struct *)arg;
  if (es != NULL)
  {

    /* try to send something out */
//    tcp_rexmit_rto(tpcb);
	tcp_output(tpcb);

    /* no remaining pbuf (chain)  */
    if(es->state == ES_CLOSING)
    {
      /*  close tcp connection */
      tcp_server2_connection_close(tpcb, es);
    }

    ret_err = ERR_OK;
  }
  else
  {
    /* nothing to be done */
  	release_tcp_pcb(tpcb);
    tcp_abort(tpcb);
    ret_err = ERR_ABRT;
  }
  return ret_err;
}

/**
  * @brief  This function implements the tcp_sent LwIP callback (called when ACK
  *         is received from remote host for sent data) 
  * @param  None
  * @retval None
  */
static err_t tcp_server2_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(len);

  es = (struct tcp_server_struct *)arg;
  

  /* try to send something out */
  tcp_output(tpcb);

  /* if no more data to send and client closed connection*/
  if(es->state == ES_CLOSING)
  {
    tcp_server2_connection_close(tpcb, es);
  }

  return ERR_OK;
}

/**
  * @brief  This functions closes the tcp connection
  * @param  tcp_pcb: pointer on the tcp connection
  * @param  es: pointer on echo_state structure
  * @retval None
  */
static void tcp_server2_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
  
  /* remove all callbacks */
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  tcp_err(tpcb, NULL);
  tcp_poll(tpcb, NULL, 0);

  /* 关闭连接时，释放tcp控制块地址 */
  release_tcp_pcb(tpcb);
  
  /* delete es structure */
  if (es != NULL)
  {
    mem_free(es);
  }  
  
  /* close tcp connection */
  tcp_close(tpcb);
}

#endif /* LWIP_TCP */
