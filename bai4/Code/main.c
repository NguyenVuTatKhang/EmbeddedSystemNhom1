#include "stm32f10x.h"                

void tim2_config(void){
    TIM_TimeBaseInitTypeDef tim;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    tim.TIM_Prescaler = 7200 - 1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = 0;

    tim.TIM_Period = 5000 - 1;
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM2, ENABLE);
}

void delay(uint32_t ms){
    TIM_SetCounter(TIM2, 0);
    while(TIM_GetCounter(TIM2) < (ms * 10)); 
}

void led_config(){
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Pin=GPIO_Pin_1;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init (GPIOA,&gpio);
}
int main(){
	led_config();
	while(1){
		GPIOA->ODR=0xff;
		delay(1000);
		GPIOA->ODR=0x00;
		delay(1000);
	}
}