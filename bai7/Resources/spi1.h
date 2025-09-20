#ifndef __SPI1__
#define __SPI1__
#ifdef __cplusplus
extern "C"{
#endif
#include "stm32f10x.h"                  // Device header

#define CE_PIN 			GPIO_Pin_3
#define CSN_PIN 		GPIO_Pin_4


#define NRF_CE_HIGH()   GPIOA->BSRR = CE_PIN
#define NRF_CE_LOW()    GPIOA->BRR  = CE_PIN
#define NRF_CSN_HIGH()  GPIOA->BSRR = CSN_PIN
#define NRF_CSN_LOW()   GPIOA->BRR  = CSN_PIN


void SPI1_Config(void);
uint16_t SPI1_Transfer(uint16_t data);

#ifdef __cplusplus
}
#endif
#endif