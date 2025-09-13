#include "stm32f10x.h"                  // Device header

void Led_Config();
void Timer_Config(uint16_t period, uint16_t prescaler);

int main(){
	Led_Config();
	Timer_Config(5000, 7200);
	while(1){
		
	}
}

void Led_Config(){
	GPIO_InitTypeDef 		GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin= GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
void Timer_Config(uint16_t period, uint16_t prescaler){
	TIM_TimeBaseInitTypeDef 		Timer_Config;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	Timer_Config.TIM_CounterMode= TIM_CounterMode_Up;
	Timer_Config.TIM_Period= period-1;
	Timer_Config.TIM_Prescaler= prescaler-1;
	Timer_Config.TIM_ClockDivision=0;
	Timer_Config.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2, &Timer_Config);
	
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
	
	NVIC_SetPriority(TIM2_IRQn,0);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		GPIOA->ODR ^=GPIO_Pin_0;
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
