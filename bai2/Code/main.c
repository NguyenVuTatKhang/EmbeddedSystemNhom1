#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

void delay(unsigned int t){
	unsigned i,j;
	for(i=0;i<t;i++){
		for(j=0;j<0x2aff;j++);
	}
}
	
void gpio_config(){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Pin=GPIO_Pin_13;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (GPIOC, &gpio);
}
int main(){
		gpio_config();
	while(1){
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay(500);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay(500);
	}
}
