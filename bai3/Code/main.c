#include "stm32f10x.h"                  
#include "stm32f10x_gpio.h"             
#include "stm32f10x_rcc.h"              

void config_led(){
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_0;         
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
}

void config_button(){
    GPIO_InitTypeDef button;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    button.GPIO_Mode = GPIO_Mode_IPU;  
    button.GPIO_Pin = GPIO_Pin_0;       
    button.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &button);
}

int main(){
    int cnt = 0;   	
    config_led();
    config_button();
    while(1){
        if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)){ 
            cnt++;
            while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0));
						delay(200);
        }
        if(cnt % 2 == 1){
            GPIO_ResetBits(GPIOA,GPIO_Pin_0); 
        }
        else{
            GPIO_SetBits(GPIOA,GPIO_Pin_0);   
        }
    }
}