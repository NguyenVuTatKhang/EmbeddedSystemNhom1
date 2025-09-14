#include "stm32f10x.h"                  // Device header
#include "string.h"

char ONLED[] = "ON";
char OFFLED[] = "OFF";

volatile char pcr;
volatile uint16_t stt;
volatile uint16_t cnt = 0;
volatile char res[100];

void Config_GPIO();
void Config_Uart(uint32_t baudrate);
void UART_SendStr(char *str);

int main()
{
	Config_GPIO();
	Config_Uart(9600);
	
	UART_SendStr("Hello from STM32\n");
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
	while(1)
	{
		if(stt)
		{
			if(strstr(res, ONLED) != NULL)
			{
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			}
			else if(strstr(res, OFFLED) != NULL)
			{
				GPIO_SetBits(GPIOA, GPIO_Pin_0);
			}
			stt = 0;
		}
	}
}


void Config_GPIO()
{
	GPIO_InitTypeDef 		GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// LED
	GPIO_InitStruct.GPIO_Pin				= GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode				= GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed			= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// TX
	GPIO_InitStruct.GPIO_Pin				= GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode 			= GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// RX
	GPIO_InitStruct.GPIO_Pin				= GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode				= GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Config_Uart(uint32_t baudrate)
{
	USART_InitTypeDef UART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	UART_InitStruct.USART_BaudRate							= baudrate;
	UART_InitStruct.USART_Mode									= USART_Mode_Rx | USART_Mode_Tx;
	UART_InitStruct.USART_Parity								= USART_Parity_No;
	UART_InitStruct.USART_StopBits							= USART_StopBits_1;
	UART_InitStruct.USART_WordLength						= USART_WordLength_8b;
	UART_InitStruct.USART_HardwareFlowControl		= USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &UART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	
	NVIC_SetPriority(USART1_IRQn, 0);
	NVIC_EnableIRQ(USART1_IRQn);
}

void UART_SendChar(char chr)
{
	USART_SendData(USART1, chr);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void UART_SendStr(char *str)
{
	while(*str != '\0')
	{
		UART_SendChar(*str++);
	}
}

uint16_t UART_GetChar(void)
{
	return USART_ReceiveData(USART1);
}

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

