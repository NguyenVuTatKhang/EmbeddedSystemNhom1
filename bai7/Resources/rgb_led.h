#ifndef __RGB_LED__
#define __RGB_LED__
#ifdef __cplusplus
extern "C"{
#endif
#include "stm32f10x.h"                  // Device header

#define RGB_PORT    GPIOA

#define RED_PIN     GPIO_Pin_0
#define GREEN_PIN   GPIO_Pin_1
#define BLUE_PIN    GPIO_Pin_2

typedef struct 
{
	uint16_t red_value;
	uint16_t green_value;
	uint16_t blue_value;
} RGB_Led;

void RGBLed_Init(void);
void RGBLed_Show(RGB_Led  rgb_led);


#ifdef __cplusplus
}
#endif
#endif