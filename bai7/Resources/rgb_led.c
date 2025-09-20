#include "rgb_led.h"

uint8_t period = 1000;


void RGBLed_Init(void)
{
  GPIO_InitTypeDef  				GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef   TIM_InitStruct;
	TIM_OCInitTypeDef   			TIM_OC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = RED_PIN | GREEN_PIN | BLUE_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(RGB_PORT, &GPIO_InitStruct);
	
	
	TIM_InitStruct.TIM_Period = period-1;
	TIM_InitStruct.TIM_Prescaler = 72-1;
	TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitStruct.TIM_ClockDivision = 0;
	TIM_InitStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &TIM_InitStruct);
	
	
	TIM_OC_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC_InitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OC_InitStruct.TIM_Pulse = 0;
	
	TIM_OC1Init(TIM2, &TIM_OC_InitStruct);
	TIM_OC2Init(TIM2, &TIM_OC_InitStruct);
	TIM_OC3Init(TIM2, &TIM_OC_InitStruct);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
}


void RGBLed_Show(RGB_Led  rgb_led)
{
	rgb_led.red_value 	= (rgb_led.red_value > period) 		? 	period : rgb_led.red_value;
	rgb_led.green_value = (rgb_led.green_value > period) 	? 	period : rgb_led.green_value;
	rgb_led.blue_value 	= (rgb_led.blue_value > period) 	? 	period : rgb_led.blue_value;
	
	TIM2->CCR1 = rgb_led.red_value;
	TIM2->CCR2 = rgb_led.green_value;
	TIM2->CCR3 = rgb_led.blue_value;
}