#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

void Led_Config();
void Timer_Config();
void delay_ms(int time_delay);

int main(){
	Led_Config();
	Timer_Config();

	while(1){
		GPIO_ResetBits(GPIOA, GPIO_Pin_0); 
		delay_ms(1000);
		GPIO_SetBits(GPIOA, GPIO_Pin_0);   
		delay_ms(1000);
	}
}


void Led_Config(){
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Timer_Config(){
	TIM_TimeBaseInitTypeDef 		Timer_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	Timer_InitStruct.TIM_Period      				= 65535;       
	Timer_InitStruct.TIM_Prescaler   				= 72- 1;       
	Timer_InitStruct.TIM_CounterMode 				= TIM_CounterMode_Up;
	Timer_InitStruct.TIM_RepetitionCounter 	= 0;
	Timer_InitStruct.TIM_ClockDivision 			= 0;
	TIM_TimeBaseInit(TIM2, &Timer_InitStruct);   
}

void delay_1ms(){ 	
	TIM_Cmd(TIM2, ENABLE);
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2) < 1000);
	TIM_Cmd(TIM2,DISABLE);
}

void delay_ms(int time_delay){
	while(time_delay--){
		delay_1ms();
	}
}

