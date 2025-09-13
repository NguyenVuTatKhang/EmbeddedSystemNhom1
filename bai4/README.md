# Bài 4 - Hẹn giờ bằng TIMER

## Nội dung bài tập

***Bài tập được chia làm 2 yêu cầu chính như sau:***

_Lưu ý: ISR là viết tắt của Interrupt Status Register_

- Cấu hình Timer để tạo một hàm delay với độ chính xác cao.

- Cấu hình Timer để tạo ngắt. Trong ISR của timer đảo trạng thái mỗi 500ms

## Các bước thực thi

- Để dễ thực hiện, bài tập đã được làm thành 2 phần: 

	+ Phần 1: Cấu hình timer: [P1](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/tree/main/bai4/delay_timer)
	
	+ Phần 2: Cấu hình timer kèm ngắt: [P2](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/tree/main/bai4/timer_irq)