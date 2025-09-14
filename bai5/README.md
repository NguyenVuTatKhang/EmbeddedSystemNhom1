# Bài 5 - Truyền thông nối tiếp UART

## Nội dung bài tập

_Bài tập yêu cầu:_

- Cấu hình UART, ở đây sử dụng UART1, tức 2 chân (PA9 ứng với TX và PA10 ứng với RX).

- Gửi chuỗi "Hello from STM32" khi khởi động.

- Nhận lệnh từ UART (Nhóm sử dụng chân PA0 ứng với led):

	+ Nếu nhận "ON" → Bật LED.

	+ Nếu nhận "OFF" → Tắt LED.

***File code kết quả: [Link](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/blob/main/bai5/user/main.c)***

## Các bước thực thi

1. Cấu hình GPIO

- Bật clock cho GPIOA.

	+ PA0: Led output (Push-Pull, 50 MHz).

	+ PA9: TX, chế độ AF_PP.

	+ PA10: RX, chế độ IN_FLOATING.

```c
void Config_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// LED PA0
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// TX PA9
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// RX PA10
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```

2. Cấu hình UART1

- Bật clock cho USART1.

```c
Baudrate = 9600.

.....
....
	UART_InitStruct.USART_BaudRate            = baudrate;
...
```

- WordLength nhóm sử dụng là 8bit (bằng hàm gọi của thư viện)

```c
	UART_InitStruct.USART_WordLength          = USART_WordLength_8b;
```

- Cho phép/kích hoạt 2 chân RX + TX ứng với PA10 + PA9, 2 chân đã được cấu hình phía trên.

```c
	UART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
```

- Kích hoạt ngắt RXNE.

```c
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	...
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
```

- Không sử dụng bit phát hiện lỗi, do USART là giao thức không đồng bộ
```c
	UART_InitStruct.USART_Parity              = USART_Parity_No;
```

- Bật stopbits là 1, khi truyền xong một dạng dữ liệu, sẽ cần một bit báo cáo đã truyền xong. Ở đây "USART_StopBits_1" tức sau khi truyền xong sẽ chờ 1 khoảng thời gian để truyền đi 1 bit để báo hiệu đã truyền xong.

```c
UART_InitStruct.USART_StopBits            = USART_StopBits_1;
```

- Không sử dụng HardwareFlowControl, đây là cơ chế điều khiển thêm luồng dữ liệu bằng tín hiệu phần cứng. Thường sẽ có 2 trường hợp là:

	+ Báo hiệu tín hiệu cho phép truyền tín hiệu 
	
	+ Báo rằng thiết bị sẵn sàng nhận dữ liệu 
	
do UART nhóm chỉ sử dụng 2 chân TX và RX nên không sử dụng phần này. Tức sẽ là dòng code sau:

```c
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
```



3. Hàm UART Send/Receive

_Để gửi 1 chuỗi dữ liệu, trước hết cần hiểu rằng trong ngôn ngữ C thông thường, chuỗi được tạo thành từ các mảng nhỏ, tức "string" sẽ được tạo thành từ nhiều "char"và kết thúc bằng "\0". Chính vì thế nhóm đã làm như sau:_

- Tạo 1 function gửi một char:
```c
void UART_SendChar(char chr)
{
	USART_SendData(USART1, chr);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}
```

- Sau đó cho duyệt các phần tử đến khi gặp "\0" thì function sẽ báo hiệu kết thúc gửi chuỗi và lặp lại 

```c
void UART_SendStr(char *str)
{
	while(*str != '\0')
	{
		UART_SendChar(*str++);
	}
}
```

- Tương tự với Function đọc dữ liệu gửi từ UART cũng sẽ làm như vậy, dưới đây là hàm nhận từng mảng dữ liệu "char".
```c
uint16_t UART_GetChar(void)
{
	return USART_ReceiveData(USART1);
}
```


4. Xử lý ngắt USART1

_Khi đọc được dữ liệu từ UART gửi đến cho STM32, function trên chỉ sử dụng để lưu được các mảng "char" đơn lẻ. Trong khi đó yêu cầu đặt ra cần đến str, chính vì thế để đảm bảo sử lý ngắt được suôn sẻ nhóm đã làm như sau:_

- Khi nhận ký tự từ UART → STM sẽ lưu vào buffer res[].

- Nếu gặp ký tự \r (Enter) → đánh dấu stt = 1. (Nhằm mục đích để kiểm tra xem người dùng đã nhập xong dữ liệu chưa, sau đó thêm "\0" vào để tạo thành chuỗi str và sử dụng để chạy lệnh bật/tắt led PA0)

```c
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		pcr = UART_GetChar();
		if(pcr == '\r')
		{
			stt = 1;
			res[cnt] = '\0';
			cnt = 0;
		}
		else
		{
			res[cnt] = pcr;
			cnt++;
		}
	}
}
```

5. Hàm main()

_Sau khi tổng hợp lại các function trên, ta được hàm main thực hiện những yêu cầu sau:_

- Gửi chuỗi "Hello from STM32\n".

- Khi nhận được chuỗi "ON" hoặc "OFF" → điều khiển LED PA0.

```c
int main()
{
	Config_GPIO();
	Config_Uart(9600);

	UART_SendStr("Hello from STM32\n");
	GPIO_SetBits(GPIOA, GPIO_Pin_0);   // LED ban đầu tắt

	while(1)
	{
		if(stt)
		{
			if(strstr(res, "ON") != NULL)
			{
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);  // Bật LED
			}
			else if(strstr(res, "OFF") != NULL)
			{
				GPIO_SetBits(GPIOA, GPIO_Pin_0);    // Tắt LED
			}
			stt = 0;
		}
	}
}
```

## Video demo của nhóm 

_Video demo kết quả của nhóm sẽ được đặt tại link sau:_
[Link](https://drive.google.com/file/d/1A5AaREdO_PpX9Z1CttqRPjDumv6Nus66/view?usp=sharing)