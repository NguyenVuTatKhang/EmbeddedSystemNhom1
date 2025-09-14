# Bài 4 - Hẹn giờ bằng Timer (P2-Ngắt Timer)

## Nội dung chính

- Cấu hình Timer để tạo ngắt. Trong ISR của timer đảo trạng thái mỗi 500ms.

- Kết quả sẽ được led nhấp nháy với chu kì 1000ms.

***File code kết quả: [Link](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/blob/main/bai4/timer_irq/user/main.c)***

## Các bước thực thi

### 1. Cấu hình Led tại PA0

_Để cấu hình Led cho PA0, ta thực hiện các bước sau:_

- Bật clock cho GPIOA

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
```

- Cấu hình chân PA0 ở chế độ Output - PushPull, tốc độ 50 MHz

```c
GPIO_InitTypeDef GPIO_InitStruct;
GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStruct);
```

### 2. Cấu hình Timer2

_Ta cấu hình Timer2 để tạo ngắt định kỳ như sau:_

- Bật clock cho Timer2 (nằm ở bus APB1)

	+ Cấu hình bộ đếm TIM2 chạy lên (TIM_CounterMode_Up)

	+ Chọn giá trị Period và Prescaler để xác định tần số ngắt

	+ Kích hoạt ngắt TIM_IT_Update

	+ Bật ngắt trong NVIC và cấu hình ưu tiên

```c
TIM_TimeBaseInitTypeDef Timer_Config;
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

Timer_Config.TIM_CounterMode = TIM_CounterMode_Up;
Timer_Config.TIM_Period      = period - 1;
Timer_Config.TIM_Prescaler   = prescaler - 1;
Timer_Config.TIM_ClockDivision = 0;
Timer_Config.TIM_RepetitionCounter = 0;

TIM_TimeBaseInit(TIM2, &Timer_Config);

TIM_Cmd(TIM2, ENABLE);	
TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

NVIC_SetPriority(TIM2_IRQn, 0);
NVIC_EnableIRQ(TIM2_IRQn);
```

### 3. Hàm xử lý ngắt của Timer2

Mỗi khi Timer2 tràn, sẽ kích hoạt ngắt và ta đảo trạng thái của Led PA0.

```c
void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
		GPIOA->ODR ^= GPIO_Pin_0;   // Đảo trạng thái Led
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
```

### 4. Hàm main()

Tích hợp các cấu hình và chạy vòng lặp chính.

```c
#include "stm32f10x.h"    // Device header

void Led_Config();
void Timer_Config(uint16_t period, uint16_t prescaler);

int main(){
	Led_Config();
	Timer_Config(5000, 7200);
	while(1){
		// Main loop rỗng, mọi việc do ngắt Timer xử lý
	}
}
```

## Video demo của nhóm 

_Video demo kết quả của nhóm sẽ được đặt tại link sau:_
[Link](https://drive.google.com/file/d/1EgeH4ssygYtohvyeIW6RTsmqJtF0CMg-/view?usp=sharing)