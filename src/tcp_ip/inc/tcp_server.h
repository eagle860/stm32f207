/*
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
 * This file is part of the lwIP TCP/IP stack.
 * IP地址、MAC地址、子网掩码及网关的修改在main.h中
 * 两个服务器的端口号分别为‘3001’‘3002’,宏定义在本文件中
 */
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"


/*-----------------------------------类型声明-----------------------------------------*/
/* ECHO protocol states */
enum tcp_server_states
{
  ES_NONE = 0,     
  ES_ACCEPTED,
  ES_RECEIVED,
  ES_CLOSING
};

/* structure for maintaing connection infos to be passed as argument 
   to LwIP callbacks*/
struct tcp_server_struct
{
  u8_t state;             /* current connection state */
  struct tcp_pcb *pcb;    /* pointer on the current tcp_pcb */
};


/*-----------------------------------变量声明-----------------------------------------*/
/* tcp pcb global pointer */
extern struct tcp_pcb *p_tcp_pcb_server1;
extern struct tcp_pcb *p_tcp_pcb_server2;
extern struct new_connect server1_new_connect;
extern struct new_connect server2_new_connect;

/*-----------------------------------函数声明-----------------------------------------*/
/* tcp initial */
void tcp_server_init(void);

/* tcp send data function */
u8_t tcp_ip_write(const	u8_t *tcp_send_buf,u16_t send_len,const u16_t TCP_PORT);

/* server1 initial */
/* 与PC业务数据交换用服务器 */	
void tcp_server1_init(void);

/* server2 initial */
/* 升级用服务器 */
void tcp_server2_init(void);

/* 连接关闭进，释放控制块地址 */
void release_tcp_pcb(struct tcp_pcb *tpcb);

#endif /* __TCP_SERVER */
