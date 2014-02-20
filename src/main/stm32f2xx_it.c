/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "public.h"
#include "tim.h"
#include "spi.h"
#include "usart.h"
#include "smartcard.h"
#include "can.h"
#include "main.h"
#include "tcp_ip.h"
#include "phy.h"
#include "ucos_ii.h"


/*�����ñ���*/
//static u8 ACK[2] = "ok";
/*�������ȫ�ֱ���������*/

/*һ���������*/
vu32 g_1ms_cnt = 0;

/*spi1 dma test parameter end*/

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    OSIntEnter();
	OSTimeTick();
	OSIntExit();
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/
/*******************************************************************************
* �������ƣ�TIM3_IRQHandler
* ����������TIM3�����ж�
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�����TIM3_capture_buf
* ���ú�������
* ���� ��
*******************************************************************************/
void TIM3_IRQHandler(void)
{
    if(SET == TIM_GetITStatus(TIM3, TIM3_IT_SOURCE)) 
    {
        /* Clear TIM3 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM3, TIM3_IT_SOURCE);
        TIM3_capture_buf[TIM3_captured++] = (u16)TIM3_GetCapture(TIM3);
        if (TIM3_CAPTURE_SIZE == TIM3_captured)
        {
            TIM3_captured = 0;
        }
    }
}
/*******************************************************************************
* �������ƣ�TIM4_IRQHandler
* ����������TIM4�����ж�
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�����TIM4_capture_buf
* ���ú�������
* ���� ��
*******************************************************************************/
void TIM4_IRQHandler(void)
{
    if(SET == TIM_GetITStatus(TIM4, TIM4_IT_SOURCE)) 
    {
        /* Clear TIM4 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM4, TIM4_IT_SOURCE);
        TIM4_capture_buf[TIM4_captured++] = (u16)TIM4_GetCapture(TIM4);
        if (TIM4_CAPTURE_SIZE == TIM4_captured)
        {
            TIM4_captured = 0;
        }
    }
}
/*******************************************************************************
* �������ƣ�TIM6_DAC_IRQHandler
* ����������1ms�ж�
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�������
* ���� ���ڴ��жϺ��������1msִ�д���
*******************************************************************************/
void TIM6_DAC_IRQHandler(void)
{
    if(SET == TIM_GetITStatus(TIM6, TIM_IT_Update))
    {
        /* Clear TIM6 interrupt pending bit */
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        /*add your codes here*/

		g_1ms_cnt++;
    }  
}
/*******************************************************************************
* �������ƣ�TIM7_IRQHandler
* ����������10ms�ж�
* ��ڲ�������
* ���ڲ�������
* �� �� ֵ����
* ȫ�ֱ�������
* ���ú�������
* ���� ���ڴ��жϺ��������10msִ�д���
*******************************************************************************/
void TIM7_IRQHandler(void)
{
    if(SET == TIM_GetITStatus(TIM7, TIM_IT_Update))
    {
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
        /*add your codes here*/
		LocalTime += SYSTEMTICK_PERIOD_MS;
    }
}
/******************************************
* �������ƣ�SPI1 DMA�жϺ���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void DMA2_Stream3_IRQHandler(void)
{
	if(RESET != DMA_GetITStatus(DMA2_Stream3,DMA_IT_TCIF3))
	{
		/*Clear the pending bit*/
		DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_TCIF3);		
		/*have sent all the data, disable the SPI1 DMA*/
		(void) SPI1_DMA_disable();				
		/*it is test codes*/
		/*spi1_send(1023);*/
	}
}
/******************************************
* �������ƣ�SPI2 DMA�жϺ���
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void DMA1_Stream4_IRQHandler(void)
{
	if(RESET != DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4))
	{
		/*Clear the pending bit*/
		DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);		
		/*have sent all the data, disable the SPI1 DMA*/
		SPI2_DMA_disable();
	}
}
/******************************************
* �������ƣ�SC_USART_IRQHandler
* ����������smartcard�жϴ�����
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void SC_USART_IRQHandler(void)
{
    if(USART_GetITStatus(SC_USART, USART_IT_RXNE) != RESET)
    {
        /* Clear SC_USART interrupt pending bit */
        USART_ClearITPendingBit(SC_USART, USART_IT_RXNE);
        SC_rx_buffer[p_SC++] = USART_ReceiveData(SC_USART);
        if (p_SC == SC_RX_BUFFER_SIZE)
        {
          p_SC = 0;
        }
    }
}
/******************************************
* �������ƣ�CAN1_RX0_IRQHandler
* ����������smartcard�жϴ�����
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void CAN1_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CANx, CAN_IT_FMP0) != RESET)
    {
        CAN_ClearITPendingBit(CANx, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
    }
}
/******************************************
* �������ƣ�����1���жϴ�����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void USART1_IRQHandler(void)
{
	/*transmit complete interrupt*/
	if(RESET != USART_GetITStatus(COM1_USART1,USART_IT_TXE))
	{
		if(USART1_complete_number < USART1_data_number)
		{
			USART_SendData(COM1_USART1,p_USART1_TX_buf[USART1_complete_number]);
		}
		else
		{
			p_USART1_TX_buf = NULL;
			USART1_complete_flag = 1;
			USART_ITConfig(COM1_USART1,USART_IT_TXE,DISABLE);
		} 
		
		USART1_complete_number ++;
	}
	/*receive register not empty interrupt*/
	if(RESET != USART_GetITStatus(COM1_USART1,USART_IT_RXNE))
	{
        USART_ClearITPendingBit(COM1_USART1,USART_IT_RXNE);
		
		/*add your own application codes here*/
		/*��ȡ���ݵ��ú���USART_ReceiveData(USART1)*/
		/*rx_data = (u8) USART_ReceiveData(USART1);*/
			
	}	
	
	if(RESET != USART_GetFlagStatus(COM1_USART1,USART_FLAG_ORE))
	{
		(void) USART_ReceiveData(COM1_USART1);
	}		
}
/******************************************
* �������ƣ�����3���жϴ�����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void USART3_IRQHandler(void)
{
	/*transmit complete interrupt*/
	if(RESET != USART_GetITStatus(COM3_USART3,USART_IT_TXE))
	{
		if(USART3_complete_number < USART3_data_number)
		{
			USART_SendData(COM3_USART3,p_USART3_TX_buf[USART3_complete_number]);
		}
		else
		{
			p_USART3_TX_buf = NULL;
			USART3_complete_flag = 1;
			USART_ITConfig(COM3_USART3,USART_IT_TXE,DISABLE);
		} 
		
		USART3_complete_number ++;
	}

	/*receive register not empty interrupt*/
	if(RESET != USART_GetITStatus(COM3_USART3,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(COM3_USART3,USART_IT_RXNE);
		
		/*add your own application codes here*/
		/*��ȡ���ݵ��ú���USART_ReceiveData(USART3)*/
		/*rx_data = (u8) USART_ReceiveData(USART3);*/	
	}
	
	if(RESET != USART_GetFlagStatus(COM3_USART3,USART_FLAG_ORE))
	{
		(void) USART_ReceiveData(COM3_USART3);
	}			
}

/******************************************
* �������ƣ�����4���жϴ�����
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void UART4_IRQHandler(void)
{
	/*transmit complete interrupt*/
	if(RESET != USART_GetITStatus(COM4_USART4,USART_IT_TXE))
	{
		if(USART4_complete_number < USART4_data_number)
		{
			USART_SendData(COM4_USART4,p_USART4_TX_buf[USART4_complete_number]);
		}
		else
		{
			p_USART4_TX_buf = NULL;
			USART4_complete_flag = 1;
			USART_ITConfig(COM4_USART4,USART_IT_TXE,DISABLE);
		}
		 
		USART4_complete_number ++;
	}

	/*receive register not empty interrupt*/
	if(RESET != USART_GetITStatus(COM4_USART4,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(COM4_USART4,USART_IT_RXNE);

		/*add your own application codes here*/
		/*��ȡ���ݵ��ú���USART_ReceiveData(UART4)*/
		/*rx_data = (u8) USART_ReceiveData(UART4);*/	
	}	
	
	if(RESET != USART_GetFlagStatus(COM4_USART4,USART_FLAG_ORE))
	{
		(void) USART_ReceiveData(COM4_USART4);
	}		
}

/******************************************
* �������ƣ��ⲿ�ж�12-15t������
* ����������
* ��ڲ�����
* ���ڲ�����
* �� �� ֵ��
* ȫ�ֱ�����
* ���ú�����
* ���� ��
*******************************************/
void EXTI15_10_IRQHandler(void)
{		
    if(SET == EXTI_GetITStatus(EXTI_Line12))
    {
        EXTI_ClearITPendingBit(EXTI_Line12);
        /*add your own codes here*/

    }
    else if(SET == EXTI_GetITStatus(EXTI_Line13))
    {
        EXTI_ClearITPendingBit(EXTI_Line13);
        /*add your own codes here*/

    }
    else if(SET == EXTI_GetITStatus(EXTI_Line14))
    {
        /* ���־ */
        EXTI_ClearITPendingBit(EXTI_Line14);
        
        /* �ظ��ⲿ�ж�ר�á�*/
        if(Bit_RESET == GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14))
        {
            /*add your own codes here*/

        }

        /* ��̫���ж�ר�á�*/
        if(Bit_RESET == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14))
        {
            Restart_ETH(); 
        }   
    }
    else if(SET == EXTI_GetITStatus(EXTI_Line15))
    {
        EXTI_ClearITPendingBit(EXTI_Line15);
        /*add your own codes here*/

    }
    else
    {
    }
}



/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
