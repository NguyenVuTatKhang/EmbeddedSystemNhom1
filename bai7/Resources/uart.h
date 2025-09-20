/*
 * @file       UART library
 * @board      STM32F10x
 * @author     Tong Sy Tan
 * @date       Sun, 09/02/2025
*/



#ifndef __UART__
#define __UART__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"                  // Device header

#include <stdint.h>

#ifndef REMAP
#define NO_REMAP 0
#define REMAP 1
#endif

typedef struct 
{
	void    (*config)(uint32_t baudrate, uint8_t remap);
	void    (*print)(const char *str, ...);
	uint16_t (*scan)(uint8_t *data);
	
}UART_TypedefStruct;

extern UART_TypedefStruct uart1;
extern UART_TypedefStruct uart2;
extern UART_TypedefStruct uart3;

#ifdef __cplusplus
}
#endif

#endif
