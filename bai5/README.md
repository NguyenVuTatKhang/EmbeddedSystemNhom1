# Nộp bài tập số 5:

## Nội dung bài tập số 5

- Cấu hình UART trên STM32F1  
- Viết chương trình gửi chuỗi "Hello from STM32!" tới máy tính  
- Sử dụng phần mềm terminal để hiển thị chuỗi này  
- Khi gửi một ký tự từ máy tính, STM32 sẽ phản hồi lại ký tự đó  
- Khi gửi chuỗi "ON" thì bật đèn, "OFF" thì tắt đèn  

## Tiến độ và kết quả

- Nhóm đã tìm hiểu datasheet của STM32F103C8T6 và thực hiện các bước cấu hình như sau:

    + Bật clock cho GPIOA, GPIOC và USART1.  
    + Cấu hình PA9 (TX) ở chế độ Alternate Function Push-Pull, PA10 (RX) ở chế độ Input Floating.  
    + Thiết lập UART1 với baudrate 9600, 8N1.  
    + Sau khi reset, STM32 gửi chuỗi "Hello from STM32!" qua cổng UART tới máy tính.  
    + Mỗi khi gửi một ký tự từ máy tính, STM32 sẽ phản hồi lại đúng ký tự đó (echo).  
    + Khi gửi "ON", LED trên PC13 sáng; khi gửi "OFF", LED tắt.  

- Kết quả:  
    + Chuỗi "Hello from STM32!" đã hiển thị thành công trên terminal.  
    + Chức năng echo hoạt động chính xác.  
    + Lệnh "ON" và "OFF" điều khiển LED đúng yêu cầu.  

- Link tới video nộp bài: [Link](https://drive.google.com/file/d/1LZEF1TRDpj4K7qXtILlsVF9fTOJl0AR3/view?usp=sharing)

- File code: [Link](https://github.com/NguyenVuTatKhang/EmbeddedSystemNhom1/blob/main/bai5/Code/main.c)
