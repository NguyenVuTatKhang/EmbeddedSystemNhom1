#include "uart.h"
#include <stdio.h>
#include <stdarg.h>

#define UART_RECEIVE_DELAY_FACTOR 10.0

#define MAX_LENGTH 0x420

// UART structure definition
UART_TypedefStruct uart1;
UART_TypedefStruct uart2;
UART_TypedefStruct uart3;

typedef struct
{
	int16_t length;
	int16_t push_index;
	int16_t pop_index;
	uint8_t data[MAX_LENGTH+1];

}UART_Queue;

static volatile UART_Queue 		 uart_queue[3];

// UARTx enumeration for selecting UART instances
// This enumeration is used to differentiate between UART instances in the code.
typedef enum 
{
	U1 = 0x00,
	U2, 
	U3
	
}UARTx_e;

// Global variables for UART data reception
uint32_t delay_receiving_data[3];
static volatile uint32_t data_timeout[3] = {0, 0, 0};
char number[50];

static __inline uint8_t UART_Queue_Push(volatile UART_Queue *queue, uint8_t data)
{
	if(queue->length < MAX_LENGTH)
	{
		queue->data[queue->push_index] = data;
		queue->push_index = (queue->push_index + 1) % MAX_LENGTH;
		queue->length = queue->length + 1;
		return 1;
	}
	return 0;
}

uint8_t UART_Queue_Pop(volatile UART_Queue *queue, uint8_t *data)
{
	if(queue->length > 0)
	{
		*data = queue->data[queue->pop_index];
		queue->pop_index = (queue->pop_index + 1) % MAX_LENGTH;
		queue->length = queue->length - 1;
		return 1;
	}
	return 0;
}

void UARTx_Config(USART_TypeDef *UART, uint32_t baudrate, uint8_t remap);
void UARTx_SendData(USART_TypeDef *UART, const char *str, va_list args);
uint16_t UARTx_ReceiveData(UARTx_e Ux, uint8_t *data);

// Function prototypes for UART initialization and operations
void UART1_Config(uint32_t baudrate, uint8_t remap);
void UART2_Config(uint32_t baudrate, uint8_t remap);
void UART3_Config(uint32_t baudrate, uint8_t remap);
void UART1_Print(const char *str, ...);
void UART2_Print(const char *str, ...);
void UART3_Print(const char *str, ...);
uint16_t UART1_Scan(uint8_t *data);
uint16_t UART2_Scan(uint8_t *data);
uint16_t UART3_Scan(uint8_t *data);

/**
	*************************************************************************
	* @brief 	Initialize Callback Functions
	* @param 	None
	* @retval None
	*************************************************************************
*/

void UART_FirstInit(void) __attribute__ ((constructor));			// Ham nay se duoc chay truoc khi vao ham main

void UART_FirstInit(void)
{
	uart1.config  = UART1_Config;
	uart1.scan  = UART1_Scan;
	uart1.print = UART1_Print;
  
	uart2.config  = UART2_Config;
	uart2.scan  = UART2_Scan;
	uart2.print = UART2_Print;
  
	uart3.config  = UART3_Config;
	uart3.scan  = UART3_Scan;
	uart3.print = UART3_Print;
}

/**
	*************************************************************************
	* @brief 	UARTx Initialization
	* @param 	USART_TypeDef
	* @param  baudrate
	* @param  remap: remap or not
	* @retval None
	*************************************************************************
*/

void UARTx_Config(USART_TypeDef *UART, uint32_t baudrate, uint8_t remap)
{
	GPIO_InitTypeDef 		GPIO_InitStruct;
	USART_InitTypeDef 	UART_InitStruct;
	
	if(UART == USART1)
	{	
		delay_receiving_data[U1] = (uint32_t)(72e7 * UART_RECEIVE_DELAY_FACTOR / 5 / baudrate);
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		
		if(remap)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
			
			GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
			
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
			
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;
			GPIO_Init(GPIOB, &GPIO_InitStruct);
		}
		else
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
			
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
			GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
			GPIO_Init(GPIOA, &GPIO_InitStruct);
		}
		
		NVIC_SetPriority(USART1_IRQn, 0);
		NVIC_EnableIRQ(USART1_IRQn);
	}
	
	else if(UART == USART2)
	{	
		delay_receiving_data[U2] = (uint16_t)(72e7 * UART_RECEIVE_DELAY_FACTOR / 5 / baudrate);
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		NVIC_SetPriority(USART2_IRQn, 0);
		NVIC_EnableIRQ(USART2_IRQn);
	}
	
	else if(UART == USART3)
	{	
		delay_receiving_data[U3] = (uint16_t)(72e7 * UART_RECEIVE_DELAY_FACTOR / 5 / baudrate);
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_11;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		NVIC_SetPriority(USART3_IRQn, 0);
		NVIC_EnableIRQ(USART3_IRQn);
	}
	
	UART_InitStruct.USART_BaudRate = baudrate;											/* toc do truyen: baud_rates */
	UART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UART_InitStruct.USART_Parity = USART_Parity_No;
	UART_InitStruct.USART_StopBits = USART_StopBits_1;
	UART_InitStruct.USART_WordLength = USART_WordLength_8b;
	UART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART, &UART_InitStruct);
	
	USART_Cmd(UART,ENABLE);																/* cho phep USART hoat dong */
	
	USART_ITConfig(UART,USART_IT_RXNE,ENABLE);						/* ngat nhan USART					*/
}

/**
	*************************************************************************
	* @brief 	UARTx Initialization
	* @param  baudrate
	* @param  remap: remap or not
	* @retval None
	*************************************************************************
*/

void UART1_Config(uint32_t baudrate, uint8_t remap)
{
	UARTx_Config(USART1, baudrate, remap);
}

void UART2_Config(uint32_t baudrate, uint8_t remap)
{
	UARTx_Config(USART2, baudrate, remap);
}

void UART3_Config(uint32_t baudrate, uint8_t remap)
{
	UARTx_Config(USART3, baudrate, remap);
}

/**
	*************************************************************************
	* @brief 	UART Print
	* @param  str: format string
	* @param  ...: arguments
	* @retval None
	*************************************************************************
*/

void UART1_Print(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	UARTx_SendData(USART1, str, args);
	va_end(args);
}

void UART2_Print(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	UARTx_SendData(USART2, str, args);
	va_end(args);
}

void UART3_Print(const char *str, ...)
{
	va_list args;
	va_start(args, str);
	UARTx_SendData(USART3, str, args);
	va_end(args);
}

/**
	*************************************************************************
	* @brief 	UART Scan
	* @param  data: received string
	* @retval true if there is a new string
	*************************************************************************
*/

uint16_t UART1_Scan(uint8_t *data)
{
	return UARTx_ReceiveData(U1, data);
}

uint16_t UART2_Scan(uint8_t *data)
{
	return UARTx_ReceiveData(U2, data);
}

uint16_t UART3_Scan(uint8_t *data)
{
	return UARTx_ReceiveData(U3, data);
}


static __inline void UART_SendChar(USART_TypeDef *UART, const char str)
{
	while(USART_GetFlagStatus(UART, USART_FLAG_TXE)==RESET);
	USART_SendData(UART, str);
}

static __inline void UART_SendStr(USART_TypeDef *UART, const char *str)
{
	do
	{
		UART_SendChar(UART, *str);
		
	}while(*(++str));
}

void UARTx_SendData(USART_TypeDef *UART, const char *str, va_list args)
{
	do{
		if(*str == '%')
		{
			switch(*(++str))
			{
				case 's':
				{
					char *temp_str = va_arg(args, char*);
					UART_SendStr(UART, temp_str);
					continue;
					break;
				}
				
				case 'd':
				{
					int temp_num = va_arg(args, int);
					sprintf(number, "%d", temp_num);
					UART_SendStr(UART, number);
					continue;
					break;
				}
				
				case 'h':
				{
					int temp_num = va_arg(args, int);
					sprintf(number, "%02X", temp_num);
					UART_SendStr(UART, number);
					continue;
					
					break;
				}
				
				case 'f':
				{
					float temp_num = va_arg(args, double);
					sprintf(number, "%f", temp_num);
					UART_SendStr(UART, number);
					continue;
					break;
				}
			}
		}
		
		UART_SendChar(UART, *str);
	}while(*(++str));
}

uint16_t UARTx_ReceiveData(UARTx_e Ux, uint8_t *data)
{
	uint16_t length = 0;
	if(uart_queue[Ux].length) 
	{	
		while(data_timeout[Ux] <= delay_receiving_data[Ux]) data_timeout[Ux] = data_timeout[Ux] + 1;
		while(UART_Queue_Pop(&uart_queue[Ux], data++)) length++;
	}
	*(data - 1) = '\0';
	return length;
}

/**
	*************************************************************************
	* @brief 	USARTxIRQn
	* @param 	None
	* @retval None
	*************************************************************************
*/

void USART1_IRQHandler(void)
{
	UART_Queue_Push(&uart_queue[U1], USART1->DR);
	data_timeout[U1] = 0;
}
void USART2_IRQHandler(void)
{
	UART_Queue_Push(&uart_queue[U2], USART2->DR);
	data_timeout[U2] = 0;
}
void USART3_IRQHandler(void)
{
	UART_Queue_Push(&uart_queue[U3], USART3->DR);
	data_timeout[U3] = 0;
}