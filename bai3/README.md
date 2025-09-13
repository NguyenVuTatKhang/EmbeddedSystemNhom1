# Bài 3 - Điều khiển ngắt ngoài (EXTI)

## Nội dung bài tập

_Bài tập có 2 yêu cầu chính như sau:_

- Cấu hình ngắt ngoài cho 1 nút nhấn. Khi nhấn thì 1 Led đảo trạng thái

- Trong khi nhấn nút thì 1 Led khác vẫn nháy với tần số 1Hz (Tức chu kỳ là 1 giây)

***File code kết quả: [Link](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/blob/main/bai3/user/main.c)***

## Các bước thực thi

1. Cấu hình cho 2 Led (Cụ thể ở đây sử dụng Led tại PA0 và PA2)

_Để cấu hình Led cho PA0 và PA2, chúng ta sẽ thực hiện các bước sau:_

- Bật clock cho GPIOA

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
```

- Cấu hình cho chân ở trạng thái Output - Pushpull, tốc độ là 50MHz

```c
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
```

- Trỏ đến vị trí chân PA0 và PA2 là vị trí điều khiển cho output trong function này, sau đó xác nhận sử dụng các cấu hình trên cho GPIOA:

```c
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
GPIO_Init(GPIOA, &GPIO_InitStruct);
```

2. Cấu hình cho nút bấm (Cụ thể ở đây sử dụng PA1)

_Để cấu hình cho nút bấm, chúng ta thực hiện các bước sau:_

- Tương tự với bước cấu hình cho Led, sẽ chỉ khác ở phần trạng thái cho chân sẽ là Input - PullUp/PullDown 

```c
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
```

- Do cấu hình cho trạng thái là Input, nên trong trường hợp này sẽ không cấu hình cho tốc độ nữa.

3. Cấu hình cho ngắt ngoài (EXTI 1, dùng chân PA1 tương ứng với nút bấm đã configure tại PA1)

_Để cấu hình cho ngắt ngoài, chúng ta thực hiện như function dưới đây:_

```c
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
```

***Lưu ý: Ngắt ngoài trong phần trên sử dụng sườn xuống, tức EXTI_Trigger_Falling***

4. Thực hiện tác vụ cho 2 Led chạy

_Tác vụ cho 2 Led chạy sẽ nằm trong 2 phần riêng biệt, với Led PA0 tại EXTI còn Led PA2 trong while(1)_

- Tác vụ với Led PA0: Sẽ đảo trạng thái bằng cách dùng dấu XOR mỗi khi nhận được nút bấm

	```c
	void EXTI1_IRQHandler(void){
		if(EXTI_GetITStatus(EXTI_Line1) != RESET){
					GPIOA->ODR ^= GPIO_Pin_0;    
			EXTI_ClearITPendingBit(EXTI_Line1); 
		}
	}
	```

	+ Sau khi đảo trạng thái xong, sẽ đặt lại trạng thái cờ cho EXTI để tránh trường hợp chie thựuc hiện được ngắt 1 lần duy nhất.

- Tác vụ với Led PA2: Nhấp nháy liên tục tại while (1), sử dụng function delay cơ bản đã giới thiệu ở bài trước.

```c
	while(1){
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		delay(500);
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		delay(500);
	}
```

## Video demo của nhóm 

_Video demo kết quả của nhóm sẽ được đặt tại link sau:_
[Link](https://drive.google.com/file/d/1pvP23hquZI2nOCYmjnZWq1IdNXOiVtYn/view?usp=sharing)