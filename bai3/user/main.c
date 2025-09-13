#include "stm32f10x.h"                  // Device header

void delay(int time);
void Led_Config();
void Button_Config();
void EXTI_Config();

int main(){
	Led_Config();
	Button_Config();
	EXTI_Config();
	
	while(1){
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		delay(500);
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		delay(500);
	}
}

void delay(int time){
	int i,j;
	for(i = 0; i<time;i++){
		for(j =0; j<0x2aff;j++);
	}
}

void Led_Config(){
	GPIO_InitTypeDef 		GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Button_Config(){
	GPIO_InitTypeDef 		GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void EXTI_Config(){
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn; 	
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);  
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_Init(&EXTI_InitStruct);
}

void EXTI1_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line1) != RESET){
				GPIOA->ODR ^= GPIO_Pin_0;    
        EXTI_ClearITPendingBit(EXTI_Line1); 
    }
}
