# Nộp bài tập số 4:

## Nội dung bài tập số 4

- Cấu hình một timer ở chế độ cơ bản  
- Sử dụng timer để tạo hàm delay với độ chính xác cao  
- Cấu hình timer để tạo ngắt mỗi 500 ms  
- Trong ISR của timer, đảo trạng thái của LED  
- Kết quả: LED nháy với chu kỳ 1000 ms  

## Tiến độ và kết quả

- Sau khi nghiên cứu Datasheet và Reference Manual của STM32F103, nhóm đã nắm được cách sử dụng Timer (TIM2) để vừa tạo hàm delay chính xác vừa tạo ngắt định kỳ. Các bước thực hiện:

	+ Cấu hình clock và GPIO:
	
			+ Bật clock cho GPIOA để điều khiển LED
			+ LED gắn vào PA1, cấu hình Output Push-Pull, tốc độ 50 MHz
	
	+ Cấu hình Timer (TIM2):
	
			+ Bật clock cho TIM2
			+ Đặt Prescaler và ARR để Timer đếm với chu kỳ 0.1 ms
			+ Từ đó dùng giá trị counter để tạo hàm delay theo ms
	
	+ Cấu hình ngắt Timer:
	
			+ Cho phép ngắt update event (TIM_IT_Update)
			+ Bật ngắt TIM2 trong NVIC
			+ Cứ 500 ms Timer phát sinh ngắt → LED đảo trạng thái
	
	+ Lập trình hoạt động:
	
			+ Hàm delay_ms() dùng counter của TIM2, không dùng vòng for thủ công
			+ Trong ISR TIM2_IRQHandler, xóa cờ ngắt và toggle LED
			+ LED nhấp nháy với chu kỳ 1000 ms (500 ms sáng, 500 ms tắt)

- Kết quả thực nghiệm:

	+ LED PA1 nhấp nháy đều đặn với chu kỳ 1000 ms  
	+ Hàm delay chính xác hơn so với dùng vòng for  

- Link tới video nộp bài: [Link]()

- File code: [Link]()
