#include "phy.h"
#include "stm32f2x7_eth_conf.h"
#include "tcp_ip.h"
#include "tcp_server.h"

/*---------------------------内部函数声明-------------------------------*/
static void ETH_Stop(void);

static uint32_t ETH_Reinit(ETH_InitTypeDef* ETH_InitStruct, uint16_t PHYAddress);

static void pb14_exti(void);

static void ETH_MACDMA_Reconfig(void);

/******************************************
* 函数名称：PHY中断引脚初始化
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void phy_interrupt_init(void)
{
  /* 初始化外部中断14 PB14 */
  pb14_exti();
}

/******************************************
* 函数名称：重设MAC
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void Restart_ETH(void)
{
  ETH_Stop();
    
  ETH_MACDMA_Reconfig();

  /* 清除PHY的中断标志位 */
  ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_INT_LIN_EN_ADD);
    
  ETH_Start();
  
  /* 清除历史连接 */
  if(NULL != p_tcp_pcb_server1)
  {
    tcp_abort(p_tcp_pcb_server1);
    p_tcp_pcb_server1 = NULL;
  }

  if(TRUE == server1_new_connect.new_flag)
  {
    tcp_abort(server1_new_connect.pcb);
    server1_new_connect.pcb = NULL;
    server1_new_connect.new_flag = FALSE;
  }

  if(NULL != p_tcp_pcb_server2)
  {
    tcp_abort(p_tcp_pcb_server2);
    p_tcp_pcb_server2 = NULL;
  }
  
  if(TRUE == server2_new_connect.new_flag)
  {
    tcp_abort(server2_new_connect.pcb);
    server2_new_connect.pcb = NULL;
    server2_new_connect.new_flag = FALSE;
  }         
}

/******************************************
* 函数名称：停止MAC
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void ETH_Stop(void)
{
  /* Flush Transmit FIFO */
  ETH_FlushTransmitFIFO();

  /* Stop DMA transmission */
  ETH_DMATransmissionCmd(DISABLE); 
  /* Stop DMA reception */
  ETH_DMAReceptionCmd(DISABLE);
        
  /* Disable transmit state machine of the MAC for transmission on the MII */  
  ETH_MACTransmissionCmd(DISABLE);  
  /* Disable receive state machine of the MAC for reception from the MII */  
  ETH_MACReceptionCmd(DISABLE);   
}


/******************************************
* 函数名称：重新初始化MAC
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
uint32_t ETH_Reinit(ETH_InitTypeDef* ETH_InitStruct, uint16_t PHYAddress)
{
  __IO uint32_t RegValue = 0; 
 
  /*-------------------- PHY initialization and configuration ----------------*/  
  /* Read the result of the auto-negotiation */
  RegValue = ETH_ReadPHYRegister(PHYAddress, PHY_SR);
  
  /* Configure the MAC with the Duplex Mode fixed by the auto-negotiation process */
  if((RegValue & PHY_DUPLEX_STATUS) != (uint32_t)RESET)
  {
    /* Set Ethernet duplex mode to Full-duplex following the auto-negotiation */
    ETH_InitStruct->ETH_Mode = ETH_Mode_FullDuplex;  
  }
  else
  {
    /* Set Ethernet duplex mode to Half-duplex following the auto-negotiation */
    ETH_InitStruct->ETH_Mode = ETH_Mode_HalfDuplex;           
  }

  /* Configure the MAC with the speed fixed by the auto-negotiation process */
  if(RegValue & PHY_SPEED_STATUS)
  {  
    /* Set Ethernet speed to 10M following the auto-negotiation */    
    ETH_InitStruct->ETH_Speed = ETH_Speed_10M; 
  }
  else
  {   
    /* Set Ethernet speed to 100M following the auto-negotiation */ 
    ETH_InitStruct->ETH_Speed = ETH_Speed_100M;      
  }    

  /*------------------------ ETHERNET MACCR Configuration --------------------*/ 
  /* ETH_Speed_100M  0x00004000  ETH_Mode_FullDuplex  0x00000800 */  
  ETH->MACCR &= ((~ ETH_Speed_100M) & (~ ETH_Mode_FullDuplex));

  ETH->MACCR |= (ETH_InitStruct->ETH_Speed | ETH_InitStruct->ETH_Mode);
  
  return ETH_SUCCESS;
}

/******************************************
* 函数名称：外部中断PortB_pin14初始化
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void pb14_exti(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Enable GPIOB clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure PA0 pin as input floating */
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Connect EXTI Line15 to PB14 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	EXTI_ClearFlag(EXTI_Line14);	
	NVIC_InitStructure.NVIC_IRQChannel = (u8) EXTI15_10_IRQn;		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ((u8)EXTI15_10_PRI >> 4) & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = (u8)EXTI15_10_PRI & 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}

/******************************************
* 函数名称：以太网参数得确定
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
static void ETH_MACDMA_Reconfig(void)
{
  ETH_InitTypeDef ETH_InitStructure;

  ETH_Reinit(&ETH_InitStructure, DP83848_PHY_ADDRESS);
}

/******************************************
* 函数名称：以太网参数初始确定
* 功能描述：
* 入口参数：
* 出口参数：
* 返 回 值：
* 全局变量：
* 调用函数：
* 描述 ：
*******************************************/
void set_init_mac(ETH_InitTypeDef* ETH_InitStruct)
{
  __IO uint32_t tmpreg;
  RCC_ClocksTypeDef  rcc_clocks;
  uint32_t hclk = 60000000;

  /*---------------------- ETHERNET MACMIIAR Configuration -------------------*/
  /* Get the ETHERNET MACMIIAR value */
  tmpreg = ETH->MACMIIAR;
  /* Clear CSR Clock Range CR[2:0] bits */
  tmpreg &= MACMIIAR_CR_MASK;
  /* Get hclk frequency value */
  RCC_GetClocksFreq(&rcc_clocks);
  hclk = rcc_clocks.HCLK_Frequency;
  /* Set CR bits depending on hclk value */
  if((hclk >= 20000000)&&(hclk < 35000000))
  {
    /* CSR Clock Range between 20-35 MHz */
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div16;
  }
  else if((hclk >= 35000000)&&(hclk < 60000000))
  {
    /* CSR Clock Range between 35-60 MHz */ 
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div26;    
  }  
  else if((hclk >= 60000000)&&(hclk < 100000000))
  {
    /* CSR Clock Range between 60-100 MHz */ 
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div42;    
  }  
  else /* ((hclk >= 100000000)&&(hclk <= 120000000)) */
  {
    /* CSR Clock Range between 100-120 MHz */ 
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div62;    
  }
  
  /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
  ETH->MACMIIAR = (uint32_t)tmpreg;

  /*------------------------ ETHERNET MACCR Configuration --------------------*/
  /* Get the ETHERNET MACCR value */  
  tmpreg = ETH->MACCR;
  /* Clear WD, PCE, PS, TE and RE bits */
  tmpreg &= MACCR_CLEAR_MASK;

  /* Set the WD bit according to ETH_Watchdog value */
  /* Set the JD: bit according to ETH_Jabber value */
  /* Set the IFG bit according to ETH_InterFrameGap value */ 
  /* Set the DCRS bit according to ETH_CarrierSense value */  
  /* Set the FES bit according to ETH_Speed value */ 
  /* Set the DO bit according to ETH_ReceiveOwn value */ 
  /* Set the LM bit according to ETH_LoopbackMode value */ 
  /* Set the DM bit according to ETH_Mode value */ 
  /* Set the IPCO bit according to ETH_ChecksumOffload value */                   
  /* Set the DR bit according to ETH_RetryTransmission value */ 
  /* Set the ACS bit according to ETH_AutomaticPadCRCStrip value */ 
  /* Set the BL bit according to ETH_BackOffLimit value */ 
  /* Set the DC bit according to ETH_DeferralCheck value */                          
  tmpreg |= (uint32_t)(ETH_InitStruct->ETH_Watchdog | 
                  ETH_InitStruct->ETH_Jabber | 
                  ETH_InitStruct->ETH_InterFrameGap |
                  ETH_InitStruct->ETH_CarrierSense |
                  ETH_InitStruct->ETH_Speed | 
                  ETH_InitStruct->ETH_ReceiveOwn |
                  ETH_InitStruct->ETH_LoopbackMode |
                  ETH_InitStruct->ETH_Mode | 
                  ETH_InitStruct->ETH_ChecksumOffload |    
                  ETH_InitStruct->ETH_RetryTransmission | 
                  ETH_InitStruct->ETH_AutomaticPadCRCStrip | 
                  ETH_InitStruct->ETH_BackOffLimit | 
                  ETH_InitStruct->ETH_DeferralCheck);
  /* Write to ETHERNET MACCR */
  ETH->MACCR = (uint32_t)tmpreg;
  
  /*----------------------- ETHERNET MACFFR Configuration --------------------*/ 
  /* Set the RA bit according to ETH_ReceiveAll value */
  /* Set the SAF and SAIF bits according to ETH_SourceAddrFilter value */
  /* Set the PCF bit according to ETH_PassControlFrames value */
  /* Set the DBF bit according to ETH_BroadcastFramesReception value */
  /* Set the DAIF bit according to ETH_DestinationAddrFilter value */
  /* Set the PR bit according to ETH_PromiscuousMode value */
  /* Set the PM, HMC and HPF bits according to ETH_MulticastFramesFilter value */
  /* Set the HUC and HPF bits according to ETH_UnicastFramesFilter value */
  /* Write to ETHERNET MACFFR */  
  ETH->MACFFR = (uint32_t)(ETH_InitStruct->ETH_ReceiveAll | 
                          ETH_InitStruct->ETH_SourceAddrFilter |
                          ETH_InitStruct->ETH_PassControlFrames |
                          ETH_InitStruct->ETH_BroadcastFramesReception | 
                          ETH_InitStruct->ETH_DestinationAddrFilter |
                          ETH_InitStruct->ETH_PromiscuousMode |
                          ETH_InitStruct->ETH_MulticastFramesFilter |
                          ETH_InitStruct->ETH_UnicastFramesFilter); 
  /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
  /* Write to ETHERNET MACHTHR */
  ETH->MACHTHR = (uint32_t)ETH_InitStruct->ETH_HashTableHigh;
  /* Write to ETHERNET MACHTLR */
  ETH->MACHTLR = (uint32_t)ETH_InitStruct->ETH_HashTableLow;
  /*----------------------- ETHERNET MACFCR Configuration --------------------*/
  /* Get the ETHERNET MACFCR value */  
  tmpreg = ETH->MACFCR;
  /* Clear xx bits */
  tmpreg &= MACFCR_CLEAR_MASK;
  
  /* Set the PT bit according to ETH_PauseTime value */
  /* Set the DZPQ bit according to ETH_ZeroQuantaPause value */
  /* Set the PLT bit according to ETH_PauseLowThreshold value */
  /* Set the UP bit according to ETH_UnicastPauseFrameDetect value */
  /* Set the RFE bit according to ETH_ReceiveFlowControl value */
  /* Set the TFE bit according to ETH_TransmitFlowControl value */  
  tmpreg |= (uint32_t)((ETH_InitStruct->ETH_PauseTime << 16) | 
                   ETH_InitStruct->ETH_ZeroQuantaPause |
                   ETH_InitStruct->ETH_PauseLowThreshold |
                   ETH_InitStruct->ETH_UnicastPauseFrameDetect | 
                   ETH_InitStruct->ETH_ReceiveFlowControl |
                   ETH_InitStruct->ETH_TransmitFlowControl); 
  /* Write to ETHERNET MACFCR */
  ETH->MACFCR = (uint32_t)tmpreg;
  /*----------------------- ETHERNET MACVLANTR Configuration -----------------*/
  /* Set the ETV bit according to ETH_VLANTagComparison value */
  /* Set the VL bit according to ETH_VLANTagIdentifier value */  
  ETH->MACVLANTR = (uint32_t)(ETH_InitStruct->ETH_VLANTagComparison | 
                             ETH_InitStruct->ETH_VLANTagIdentifier); 
       
  /*-------------------------------- DMA Config ------------------------------*/
  /*----------------------- ETHERNET DMAOMR Configuration --------------------*/
  /* Get the ETHERNET DMAOMR value */  
  tmpreg = ETH->DMAOMR;
  /* Clear xx bits */
  tmpreg &= DMAOMR_CLEAR_MASK;
  
  /* Set the DT bit according to ETH_DropTCPIPChecksumErrorFrame value */
  /* Set the RSF bit according to ETH_ReceiveStoreForward value */
  /* Set the DFF bit according to ETH_FlushReceivedFrame value */
  /* Set the TSF bit according to ETH_TransmitStoreForward value */
  /* Set the TTC bit according to ETH_TransmitThresholdControl value */
  /* Set the FEF bit according to ETH_ForwardErrorFrames value */
  /* Set the FUF bit according to ETH_ForwardUndersizedGoodFrames value */
  /* Set the RTC bit according to ETH_ReceiveThresholdControl value */
  /* Set the OSF bit according to ETH_SecondFrameOperate value */
  tmpreg |= (uint32_t)(ETH_InitStruct->ETH_DropTCPIPChecksumErrorFrame | 
                  ETH_InitStruct->ETH_ReceiveStoreForward |
                  ETH_InitStruct->ETH_FlushReceivedFrame |
                  ETH_InitStruct->ETH_TransmitStoreForward | 
                  ETH_InitStruct->ETH_TransmitThresholdControl |
                  ETH_InitStruct->ETH_ForwardErrorFrames |
                  ETH_InitStruct->ETH_ForwardUndersizedGoodFrames |
                  ETH_InitStruct->ETH_ReceiveThresholdControl |                                   
                  ETH_InitStruct->ETH_SecondFrameOperate); 
  /* Write to ETHERNET DMAOMR */
  ETH->DMAOMR = (uint32_t)tmpreg;
  
  /*----------------------- ETHERNET DMABMR Configuration --------------------*/ 
  /* Set the AAL bit according to ETH_AddressAlignedBeats value */
  /* Set the FB bit according to ETH_FixedBurst value */
  /* Set the RPBL and 4*PBL bits according to ETH_RxDMABurstLength value */
  /* Set the PBL and 4*PBL bits according to ETH_TxDMABurstLength value */
  /* Set the DSL bit according to ETH_DesciptorSkipLength value */
  /* Set the PR and DA bits according to ETH_DMAArbitration value */         
  ETH->DMABMR = (uint32_t)(ETH_InitStruct->ETH_AddressAlignedBeats | 
                          ETH_InitStruct->ETH_FixedBurst |
                          ETH_InitStruct->ETH_RxDMABurstLength | /* !! if 4xPBL is selected for Tx or Rx it is applied for the other */
                          ETH_InitStruct->ETH_TxDMABurstLength | 
                         (ETH_InitStruct->ETH_DescriptorSkipLength << 2) |
                          ETH_InitStruct->ETH_DMAArbitration |
                          ETH_DMABMR_USP); /* Enable use of separate PBL for Rx and Tx */
                          
  #ifdef USE_ENHANCED_DMA_DESCRIPTORS
    /* Enable the Enhanced DMA descriptors */
    ETH->DMABMR |= ETH_DMABMR_EDE;
  #endif /* USE_ENHANCED_DMA_DESCRIPTORS */

  /*---------------------------------初始化PHY--------------------------------*/
  /* Delay to assure PHY power up */
  delay_us(PHY_POWER_UP_DELAY);
  
  if(!(ETH_WritePHYRegister(DP83848_PHY_ADDRESS, PHY_BCR, PHY_Reset)))
  {
    /* Return ERROR in case of write timeout */
    return;
  }
  /* delay wait PHY reset */
  delay_us(PHY_POWER_UP_DELAY);

  if(!(ETH_WritePHYRegister(DP83848_PHY_ADDRESS, PHY_BCR, PHY_Reset)))
  {
    /* Return ERROR in case of write timeout */
    return;
  }
  /* delay wait PHY reset */
  delay_us(PHY_POWER_UP_DELAY);

  /* 清除PHY的中断标志位 */
  ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_INT_LIN_EN_ADD);

  /* 使能PHY的interrupt */
  if(!(ETH_WritePHYRegister(DP83848_PHY_ADDRESS, PHY_INT_EN_ADD, PHY_INT_EN)))
  {
    /* Return ERROR in case of write timeout */
    return;
  }

  /* auto negotiation complete interrupt enable */
  if(!(ETH_WritePHYRegister(DP83848_PHY_ADDRESS, PHY_ANC_INT_EN_ADD, PHY_ANC_INT_EN)))
  {
    /* Return ERROR in case of write timeout */
    return;
  }

  /* enalbe phy auto-negatiation */
  ETH_WritePHYRegister(DP83848_PHY_ADDRESS, PHY_BCR, PHY_AutoNegotiation);
}
