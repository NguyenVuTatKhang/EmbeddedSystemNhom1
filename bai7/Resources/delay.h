/*
 * @file       delay library
 * @board      STM32F10x
 * @author     Tong Sy Tan
 * @date       Sun, 09/02/2025
*/

#ifndef __DELAY__
#define __DELAY__
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"                  // Device header

extern __inline uint32_t millis(void);
extern __inline uint32_t micros(void);
void Delay_Init(void);
void delay_ms(uint32_t time_delay);
void delay_us(uint32_t time_delay);

#ifdef __cplusplus
}
#endif
#endif