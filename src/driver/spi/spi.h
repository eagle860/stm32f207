/*********************************************
�ļ�����spi.h
�汾�ţ�v1.0
��Ȩ˵����
���ߣ��̺�
��д���ڣ�2011/08/30
��Ҫ������spi�ⲿ�ӿں�������
�޸��ˣ�
�޸�����:
�޸ļ�¼��
********************************************/
#ifndef _SPI_H
#define _SPI_H
#include "public.h"


/*��ʼ��spi1, spi2���ݴ�С*/
#define BUFFER_SIZE  1023


/*SPI1�˿�������Ϣ*******************************/
#define		SPI1_CLK				RCC_APB2Periph_SPI1
/*MOSI��Ӧ����*/
#define     SPI1_MOSI_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SPI1_MOSI_GPIO_PORT		GPIOB
#define		SPI1_MOSI_SOURCE		GPIO_PinSource5
#define 	SPI1_MOSI_PIN			GPIO_Pin_5
#define		SPI1_MOSI_AF			GPIO_AF_SPI1
/*MISO��Ӧ����*/
#define     SPI1_MISO_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		SPI1_MISO_GPIO_PORT		GPIOA
#define		SPI1_MISO_SOURCE		GPIO_PinSource6
#define 	SPI1_MISO_PIN			GPIO_Pin_6
#define		SPI1_MISO_AF			GPIO_AF_SPI1
/*CLK��Ӧ����*/
#define     SPI1_SCK_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		SPI1_SCK_GPIO_PORT		GPIOA
#define		SPI1_SCK_SOURCE		    GPIO_PinSource5
#define 	SPI1_SCK_PIN			GPIO_Pin_5
#define		SPI1_SCK_AF				GPIO_AF_SPI1
/*NSS��Ӧ����*/
#define     SPI1_NSS_GPIO_CLK		RCC_AHB1Periph_GPIOA
#define		SPI1_NSS_GPIO_PORT		GPIOA
#define		SPI1_NSS_SOURCE	     	GPIO_PinSource15
#define 	SPI1_NSS_PIN			GPIO_Pin_15
#define		SPI1_NSS_AF				GPIO_AF_SPI1


/*SPI2�˿�������Ϣ*******************************/
#define		SPI2_CLK				RCC_APB1Periph_SPI2
/*MOSI��Ӧ����*/
#define     SPI2_MOSI_GPIO_CLK		RCC_AHB1Periph_GPIOC
#define		SPI2_MOSI_GPIO_PORT		GPIOC
#define		SPI2_MOSI_SOURCE		GPIO_PinSource3
#define 	SPI2_MOSI_PIN			GPIO_Pin_3
#define		SPI2_MOSI_AF			GPIO_AF_SPI2
/*MISO��Ӧ����*/
#define     SPI2_MISO_GPIO_CLK		RCC_AHB1Periph_GPIOC
#define		SPI2_MISO_GPIO_PORT		GPIOC
#define		SPI2_MISO_SOURCE		GPIO_PinSource2
#define 	SPI2_MISO_PIN			GPIO_Pin_2
#define		SPI2_MISO_AF			GPIO_AF_SPI2
/*CLK��Ӧ����*/
#define     SPI2_SCK_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SPI2_SCK_GPIO_PORT		GPIOB
#define		SPI2_SCK_SOURCE		    GPIO_PinSource10
#define 	SPI2_SCK_PIN			GPIO_Pin_10
#define		SPI2_SCK_AF				GPIO_AF_SPI2
/*NSS��Ӧ����*/
#define     SPI2_NSS_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define		SPI2_NSS_GPIO_PORT		GPIOB
#define		SPI2_NSS_SOURCE	     	GPIO_PinSource9
#define 	SPI2_NSS_PIN			GPIO_Pin_9
#define		SPI2_NSS_AF				GPIO_AF_SPI2


/*spi initial, include spi1 and spi2*/
void SPI_init(void);

/*disable spi1 dma, called by spi1 interrupt handler*/
u8 SPI1_DMA_disable(void);

/*spi1 send function*/
u8 spi1_send(u16 len);

/*disable spi1 dma, called by spi2 interrupt handler*/
u8 SPI2_DMA_disable(void);

/*spi2 send function*/
u8 spi2_send(u16 len);

#endif
