#ifndef _PHY_H
#define _PHY_H
#include "public.h"
#include "netconf.h"
#include "stm32f2x7_eth.h"
#include "stm32f2x7_eth_bsp.h"

/*---------------------------宏定义-------------------------------*/
#define PHY_INT_EN            0x0003
#define PHY_INT_EN_ADD        0x11
#define PHY_INT_LINK_EN       0x0020
#define PHY_INT_LIN_EN_ADD    0x12

#define PHY_ANC_INT_EN        0x0004
#define PHY_ANC_INT_EN_ADD    0x12

#define PHY_MAC_REGISTER1     0x02
#define PHY_MAC_REGISTER2     0x03

#define PHY_POWER_UP_DELAY    170000UL     /* 170ms delay */

/*------------------------外部函数声明----------------------------*/
void phy_interrupt_init(void);

void Restart_ETH(void);

void set_init_mac(ETH_InitTypeDef* ETH_InitStruct);

//void read_phy_mac(void);

#endif
