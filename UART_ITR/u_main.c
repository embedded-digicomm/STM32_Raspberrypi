#include "stm32f429xx.h"
#include <stdint.h>

volatile uint8_t rx_char;
volatile uint8_t rx_received = 0;

void uart1_send_ch(uint8_t ch)
{
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = ch;
}

void uart1_send_string(const char *str)
{
    while (*str)
    {
        uart1_send_ch(*str++);
    }
}

void uart1_init(void)
{
    /* Enable clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    /* PA9 -> USART1_TX
       PA10 -> USART1_RX */

    GPIOA->MODER &= ~((3U << (9 * 2)) | (3U << (10 * 2)));
    GPIOA->MODER |=  ((2U << (9 * 2)) | (2U << (10 * 2)));

    GPIOA->AFR[1] &= ~((0xFU << 4) | (0xFU << 8));
    GPIOA->AFR[1] |=  ((7U << 4) | (7U << 8));

    /* Optional: high speed */
    GPIOA->OSPEEDR |= ((3U << (9 * 2)) | (3U << (10 * 2)));

    /* 9600 baud @ 16 MHz */
    //USART1->BRR = 0x0683;
	USART1->BRR = (104 << 4) | 3;

    /* Enable RX, TX, RX interrupt */
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;
    USART1->CR1 |= USART_CR1_RXNEIE;

    /* Enable USART */
    USART1->CR1 |= USART_CR1_UE;

    /* Enable USART1 interrupt in NVIC */
    NVIC_EnableIRQ(USART1_IRQn);
}

void USART1_IRQHandler(void)
{
    if (USART1->SR & USART_SR_RXNE)
    {
        rx_char = (uint8_t)USART1->DR;
        rx_received = 1;

        /* Echo received character */
        uart1_send_ch('[');
        uart1_send_ch(rx_char);
        uart1_send_ch(']');
    }
}

int main(void)
{
    uart1_init();

    uart1_send_string("\r\nUSART1 READY\r\n");

    while (1)
    {
        if (rx_received)
        {
            rx_received = 0;
        }
    }
}