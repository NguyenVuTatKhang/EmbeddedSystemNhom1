# Nộp bài tập số 3:

## Nội dung bài tập số 3

- Nạp code và cho LED PA0 đổi trạng thái (toggle) khi nhấn nút PB0 (polling)
- Sử dụng pull-up nội tại cho nút, xử lý simple debounce bằng chờ nhả và delay

## Tiến độ và kết quả

- Bằng cách tìm hiểu Datasheet và Reference Manual của STM32F103, nhóm đã hiểu được cách sử dụng thanh ghi và các hàm thư viện StdPeriph (GPIO/RCC) để lập trình STM32F103C8T6; các bước thực hiện chính gồm:

	+ Cấu hình clock và GPIO:
	
			+ Bật clock cho GPIOA để điều khiển LED (RCC_APB2Periph_GPIOA)
			+ Bật clock cho GPIOB để đọc nút nhấn (RCC_APB2Periph_GPIOB)
	
	+ Cấu hình LED:
	
			+ LED gắn vào PA0, cấu hình Output Push-Pull, tốc độ 50 MHz
	
	+ Cấu hình nút nhấn:
	
			+ Nút nhấn gắn vào PB0, cấu hình Input pull-up nội tại (GPIO_Mode_IPU)
			+ Nút được đọc bằng hàm GPIO_ReadInputDataBit; nút nhấn ở trạng thái hoạt động là mức thấp (active low)
	
	+ Lập trình hoạt động:
	
			+ Sử dụng một biến đếm cnt; mỗi lần phát hiện nút nhấn (mức thấp) tăng cnt, chờ tới khi nút nhả (while loop) và delay để giảm hiện tượng rung (debounce)
			+ Khi cnt lẻ: tắt LED (ResetBits PA0), khi cnt chẵn: bật LED (SetBits PA0)
	
- Kết quả thực nghiệm:

	+ Khi nhấn và thả nút PB0: LED PA0 sẽ đổi trạng thái (toggle) theo số lần nhấn
	+ Cách xử lý debounce đơn giản bằng chờ nhả + delay(200) cho hiệu quả với nút cơ bản

- Link tới video nộp bài: [Link](https://drive.google.com/file/d/1rTydeaSxpZ_YcDG3YwTLYiiWPIAC5uHs/view?usp=drive_link)

- File code: [Link](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/blob/main/bai3/Code/main.c)
