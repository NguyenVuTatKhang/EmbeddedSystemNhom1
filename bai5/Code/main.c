#include "stm32f10x.h"
#include <string.h>

void UART1_Init(uint32_t baud);
void UART1_SendChar(char c);
void UART1_SendString(char *str);
char UART1_GetChar(void);
void LED_Init(void);
void LED_On(void);
void LED_Off(void);

int main(void) {
    RCC->APB2ENR |= 0x00000010;   // GPIOC (LED PC13)
    RCC->APB2ENR |= 0x00000004;   // GPIOA (USART1)
    RCC->APB2ENR |= 0x00004000; // USART1

    LED_Init();
    UART1_Init(9600);

    UART1_SendString("Hello from STM32!\r\n");

    char buffer[10];
    uint8_t idx = 0;

    while (1) {
        char c = UART1_GetChar();
        UART1_SendChar(c);

        if (c == '\r' || c == '\n') {
            buffer[idx] = '\0';
            if (strcmp(buffer, "ON") == 0) {
                LED_On();
                UART1_SendString("LED ON\r\n");
            } else if (strcmp(buffer, "OFF") == 0) {
                LED_Off();
                UART1_SendString("LED OFF\r\n");
            }
            idx = 0;
        } else {
            if (idx < sizeof(buffer)-1) {
                buffer[idx++] = c;
            }
        }
    }
}

void LED_Init(void) {
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |=  (0x2 << 20);
    GPIOC->BSRR = (1 << 13);
}

void LED_On(void) {
    GPIOC->BSRR = (1 << (13 + 16));
}

void LED_Off(void) {
    GPIOC->BSRR = (1 << 13);
}

void UART1_Init(uint32_t baud) {
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0x2 << 4);
    GPIOA->CRH |=  (0x2 << 6);

    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x1 << 10);

    USART1->BRR = SystemCoreClock / baud;
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_UE;
}

void UART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
    while (!(USART1->SR & USART_SR_TC));
}

void UART1_SendString(char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}

char UART1_GetChar(void) {
    while (!(USART1->SR & USART_SR_RXNE));
    return (char)(USART1->DR & 0xFF);
}