#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_i2c.h"              // Keil::Device:StdPeriph Drivers:I2C
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_usart.h"            // Keil::Device:StdPeriph Drivers:USART
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "string.h"
#include "stdio.h"
/*
cac chan BH1750 theo datasheet:
VCC – nguon cung cap
GND – mass
SCL – chan clock I2C
SDA – chan data I2C
ADDR – chon dia chi I2C (GND = 0x23, VCC = 0x5C)

*/
// cam ADDR vào VCC
/*
 tai sao phai dich trai 1(<<1) ?
 vì I2C gui 8bit gom 7 bit dia chi + 1 bit R/W (doc/ ghi)
 nên cân dich phai 1 dê dây 7 bit dia chi len vi tri [7-1], còn vi trí [0] de bit R/W
*/
#define BH1750_ADDR  (0x5C<<1)

void Gpio_init(void);
void i2c_init(void);
void I2C_WriteByte(uint8_t addr, uint8_t data);
void I2C_ReadMulti(uint8_t addr, uint8_t *buf, uint8_t len) ;
void BH1750_Init(void);
uint16_t BH1750_ReadLight(void);
void config_usart(void);
void uart_SendChar(char ch);
int fputc(int ch, FILE *f);

struct __FILE { int dummy; };
FILE __stdout;

void delay_ms(int t){
	int i,j;
	for(i=0;i<t;i++){
		for(j=0;j<0x2aff;j++){
		}
	}
}
int main(void){
    uint16_t lux;

    Gpio_init(); // config chan I2C1
    i2c_init();  // config I2C1
		config_usart(); // config uart 1
		BH1750_Init();  // config che do BH1750
    delay_ms(180);  // delay de BH1750 do du lieu
	  /* 
	   Trong dieu kien binh thuong, sau khoang 120ms la BH1750 co san du lieu de doc.
     Nhung neu gap truong hop xau (nhiet do, anh sang yeu, do sai lech cua IC…), thi BH1750 co the can toi da 180ms moi xong.
	  */ 

    while(1)
    {
      lux = BH1750_ReadLight();
      printf("Lux = %u lx\r\n", lux);
      delay_ms(500);
    }
}
void BH1750_Init(void)
{
	  /*
		 cac lenh cua BH1750
		 * lenh nguon
	     + 0x00 : power down, khong do du lieu
	     + 0x01 : power on , cho lenh tiep theo
	     + 0x07 : Reset, xoa thanh ghi du lieu
	   * Chê dô do
	     * do lien tuc
	       + 0x10 : Continuous H-Resolution Mode , do phan giai 1 lx, thoi gian hoan thanh phep do 120 ms
	       + 0x11 : Continuous H-Resolution 2 , do phan giai 0.5 lx , thoi gian hoan thanh phep do 120 ms
	       + 0x13 : Continuous L-Resolution, do phan giai 4 lx, thoi gian hoan thanh phep do 16 ms
			 * do 1 lan -> power down
	       + 0x20 : One-Time H-Resolution, do phan giai 1 lx, thoi gian hoan thanh phep do 120 ms
	       + 0x21 : One-Time H-Resolution 2 , do phan giai 0.5 lx , thoi gian hoan thanh phep do 120 ms
	       + 0x23 : One-Time L-Resolution, do phan giai 4 lx, thoi gian hoan thanh phep do 16 ms
		*/
    // Power ON
    I2C_WriteByte(BH1750_ADDR, 0x01);
    // Reset
    I2C_WriteByte(BH1750_ADDR, 0x07);
    // Che do do lien tuc (1 lx, thoi gian ~120ms)
    I2C_WriteByte(BH1750_ADDR, 0x10);
}
uint16_t BH1750_ReadLight(void) {
		/*
	   sau do master (STM32) doc du lieu qua I2C, slave (BH1750) se tra ve 2 byte lien tuc:
			MSB truoc (buf[0])
			LSB sau (buf[1])
	  */ 
    uint8_t buf[2];
    uint16_t raw;

    I2C_ReadMulti(BH1750_ADDR, buf, 2); // doc 2 byte liên tuc

    raw = (buf[0] << 8) | buf[1];
		/*
	   cam bien BH1750 se tra ve so raw 16 bit (du lieu thô), chua phai don vi lux.
		 de doi sang lux (don vi anh sang thuc te) thi datasheet chi ro cong thuc:
            lux = raw/1.2
	   trong do
      + Raw = so 16 bit doc duoc tu BH1750 (MSB << 8 | LSB)
      + 1.2 = he so chuan hoa do nha san xuat quy dinh, tuong ung voi thoi gian do mac dinh (MTreg = 69, ~120ms)
	  */
    return (uint16_t)(raw / 1.2f); // lux
}
void Gpio_init(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	// bat clock cho I2c1 - PB6, PB7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	
	// cau hinh 2 chan PB6-SCL, PB7-SDA

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;   
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}
void i2c_init(void){
	// khai báo bien struct cho I2c
	I2C_InitTypeDef I2C_InitStruct;
	/* Bat clock cho i2c
		clock cho I2c nam o thanh ghi RCC_APB1ENR trang 148
		bat clock cho i2c1 	-> 0x00200000	
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	/*
	 Reset I2c tren thanh ghi RCC_APB1RSTR trang 142
	 bat bit reset cho i2c và thoat reset
	*/
	I2C_DeInit(I2C1); // reset I2c ? 
	// ---------------- cau hinh cho chan i2c --------------
	/*
		chon toc do truyen cua bus I²C, tuc là tan so xung trên chân SCL.
		+ che do standard mode (100khz)
		+ che do Fast Mode (400 kHz)
	*/
	I2C_InitStruct.I2C_ClockSpeed = 100000;     
	/*
	chon che do:
	 + I2C_Mode_I2C: hoat dong o che do i2c thuong
	 + I2C_Mode_SMBusDevice / I2C_Mode_SMBusHost (SMBus) : chuyen dung cho he thong quan li nang luong
	*/
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;         
  
	/*
	 ti le thoi gian low/high
	 + I2C_DutyCycle_2 : ty le xung SCL low/high = 2, thuong dung
	 + I2C_DutyCycle_16_9 : ty le xung SCL low/high = 16/9, chi dung fast mode
	*/
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;    
	/* 
		dia chi cua STM32 khi lam slave
		khi stm32 làm matter thi gia tri nay khong quan trong
	*/
	I2C_InitStruct.I2C_OwnAddress1 = 0x00; 
	/*
		 cho phep gui ACK sau khi nhan du lieu
		 Enable : sau moi byte nhan duoc, STM32 se tu dong kéo SDA xuong 0 (ACK) báo cho master biet da nhan dc du lieu
		 Disable : sau moi lan nhan byte se khong gui ACK (tuc gui NACK de ket thuc viec nhan du lieu)
	*/
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;           // phat ACK sau khi nhan dc giu lieu
	/*
		chon so bit dia chi cho slave
		I2C_AcknowledgedAddress_7bit : chon dia chi 7 bit, hau het cac ngoai vi pho bien
		I2C_AcknowledgedAddress_10bit : chon dia chi 10 bit, ngoai vi dac biet 
	*/
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // so  bit dia chi cho slave
	
	I2C_Init(I2C1, &I2C_InitStruct);
	I2C_Cmd(I2C1, ENABLE); // enable I2C1

}
void I2C_WriteByte(uint8_t addr, uint8_t data)
{
		/*
		 Start
		 Bat dau gui du lieu SDA(high-low) khi SCL(high) bao hieu bat dau truyen dia chi
		*/
    I2C_GenerateSTART(I2C1, ENABLE); 
		/*
		 sau khi start thanh công, I2c se bat co SB(start Bit) trong thanh ghi SR1 trang 777
		 vòn while se cho sau khi bit SB dc bat de bat dau gui dia chi 
		*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /*
	   gui dia chi cua slave va chon che do send hoac receive
	   Transmitter : matter muon ghi du lieu
					+ gui dia chi cua slave vao thanh ghi DR trang 777
	        + Hardware se tu dong dua các bit vào shift register , tín hieu SCL/SDA xuat ra bus.
	        + Slave phù hop se nhan dia chi này và tra ACK.
	   Receiver : matter muon doc du lieu 
	  */
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
		/*
		 cho slave tra ve ACK tuong ung ADDR trong SR1 = 1 trang 777
		*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		/*
     Send data
		 viet byte by lieu vao thanh ghi DR
		*/
    I2C_SendData(I2C1, data);
		/*
		 cho du lieu gui xong hoan toan và slave tra ACK/NACK
		 tuong ung BTF (Byte Transfer Finished) = 1 trong thanh ghi SR1/SR2 trang 777
		*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		/*
     Stop
		 kep SDA tu low - high khi SCL dang high
		 ket thuc phien truyen
		*/
    I2C_GenerateSTOP(I2C1, ENABLE);
}
void I2C_ReadMulti(uint8_t addr, uint8_t *buf, uint8_t len) {
    // Start
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /*
	   gui dia chi cua slave va chon che do send hoac receive
	   Transmitter : matter muon ghi du lieu
	   Receiver : matter muon doc du lieu 
					+ gui dia chi cua slave vao thanh ghi DR trang 777
					+ Hardware se tu dong dua các bit vào shift register , tín hieu SCL/SDA xuat ra bus.
					+ Slave phù hop se nhan dia chi này và tra ACK.
	
	  */
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
		/*
		 cho slave tra ve ACK tuong ung ADDR trong SR1 = 1 trang 777
		*/
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

		/*
	   len : so byte can doc tu slave
		*/
    while(len) {
        if(len == 1) {
            // Byte cuoi -> gui NACK + STOP
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
        }
				/*
					cho RXNE = 1 trong SR1 trang 777
					-> DR có du lieu tu slave gui
				*/
        if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {
						/*
					   b1: ghi lai du lieu tu I2C1->DR vào buf
					   b2 : tang dia chi cua buf
					   b3 : giam do dai len
						*/
            *buf = I2C_ReceiveData(I2C1);
            buf++;
            len--;
        }
    }

    /*
	   bat ACK cho lan gui du lieu sau
		*/
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}



// -------- UART printf support ----------

void uart_SendChar(char ch) {
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

int fputc(int ch, FILE *f) {
    uart_SendChar((char)ch);
    return ch;
}

// -------- UART init ----------
void config_usart(void) {
    // Bat clock cho GPIOA và USART1
		USART_InitTypeDef usart;
		GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // PA2 = TX
    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    // PA3 = RX
    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    // C?u hình USART1
    usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &usart);

    USART_Cmd(USART1, ENABLE);
}
