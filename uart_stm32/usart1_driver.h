#ifndef USART1_DRIVER_H
#define USART1_DRIVER_H

#include <stdint.h>

#define USART1_DEFAULT_PCLK_HZ 16000000UL
#define USART1_DEFAULT_BAUD    115200UL

void USART1_Init(uint32_t pclk_hz, uint32_t baudrate);
void USART1_WriteChar(char ch);
char USART1_ReadChar(void);
void USART1_WriteString(const char *text);
uint8_t USART1_IsRxReady(void);

#endif

