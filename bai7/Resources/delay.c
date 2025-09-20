/*
 * @file       delay library
 * @board      STM32F10x
 * @author     Tong Sy Tan
 * @date       Sun, 09/02/2025
*/

#include "delay.h"

static volatile uint32_t ms = 0;

void Delay_Init(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

__inline uint32_t millis(void){
    return ms;
}

__inline uint32_t micros(void){
	uint32_t reload = SysTick->LOAD + 1;
	return ms * 1000 + (reload - SysTick->VAL) * 1000 / reload;
}

void delay_ms(uint32_t time_delay){
	uint32_t cur_time = millis();
	while(millis() - cur_time < time_delay);
}

void delay_us(uint32_t time_delay){
	uint32_t cur_time = micros();
	while(micros() - cur_time < time_delay);
}

void SysTick_Handler(void)
{
	ms++;
}
