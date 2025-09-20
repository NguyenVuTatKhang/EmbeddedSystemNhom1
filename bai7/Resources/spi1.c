#include "spi1.h"


void SPI1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);

	// PA5 -> SCK, PA7 -> MOSI
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// PA6 -> MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// SPI1
	SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial 		= 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);

	// CE, CSN
	GPIO_InitStructure.GPIO_Pin = CE_PIN | CSN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	NRF_CE_LOW();
	NRF_CSN_HIGH();
}

uint16_t SPI1_Transfer(uint16_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, data);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI1);
}
