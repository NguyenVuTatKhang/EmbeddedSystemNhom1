# Nộp bài tập số 2:

## Nội dung bài tập số 2

- Nạp code và cho led PC13 blink thành công

## Tiến độ và kết quả

- Bằng cách tìm hiểu DataSheet từ nhà sản xuất của STM32 dòng F103, nhóm đã hiểu được cách sử dụng thanh ghi để code cho vi điều khiển STM32F103C8T6 gồm có các bước sau:

	+ Cấu hình cho chân PC13 của vi điều khiển:
	
			+ Ở đây, đầu tiền cần phải cấp xung clock cho IOPC bằng cách cho RCC trỏ tới APB2RSTR (nơi chứa thanh C theo Datasheet) và bật phần tương ứng trong bảng trang 141 của Datasheet lên 1, ứng với số 0x00000010 theo mã hex
			
			+ Sau đó, cấu hình cho chân số 13, chân số 13 nằm trong CRH (từ chân số 8 đến 15 theo Datasheet) và cho bit tương ứng với chân đó bật lên với số tương ứng là 0x00300000, tức bật mode là output, max speed 50MHz.
			
			+ Tiếp theo, cho GPIOC trỏ tới BSRR, và lặp lại 2 số (1 << 13) và ~(1 << 13) liên tục để bật và tắt led, tạo thành blink led.

- Link tới video nộp bài: [Link](https://drive.google.com/file/d/1c33MMcpQipCsHPuv1BZ6K9sDsr9eDK4l/view?usp=sharing)
	
- File code: [Link](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/blob/main/bai2/Code/main.c)

