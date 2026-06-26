#include "usart1_driver.h"

#define REG32(addr) (*(volatile uint32_t *)(addr))

#define PERIPH_BASE        0x40000000UL
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE    (PERIPH_BASE + 0x00020000UL)

#define GPIOA_BASE         (AHB1PERIPH_BASE + 0x00000000UL)
#define RCC_BASE           (AHB1PERIPH_BASE + 0x00003800UL)
#define USART1_BASE        (APB2PERIPH_BASE + 0x00001000UL)

#define RCC_AHB1ENR        REG32(RCC_BASE + 0x30UL)
#define RCC_APB2ENR        REG32(RCC_BASE + 0x44UL)

#define GPIOA_MODER        REG32(GPIOA_BASE + 0x00UL)
#define GPIOA_OTYPER       REG32(GPIOA_BASE + 0x04UL)
#define GPIOA_OSPEEDR      REG32(GPIOA_BASE + 0x08UL)
#define GPIOA_PUPDR        REG32(GPIOA_BASE + 0x0CUL)
#define GPIOA_AFRH         REG32(GPIOA_BASE + 0x24UL)

#define USART1_SR          REG32(USART1_BASE + 0x00UL)
#define USART1_DR          REG32(USART1_BASE + 0x04UL)
#define USART1_BRR         REG32(USART1_BASE + 0x08UL)
#define USART1_CR1         REG32(USART1_BASE + 0x0CUL)
#define USART1_CR2         REG32(USART1_BASE + 0x10UL)
#define USART1_CR3         REG32(USART1_BASE + 0x14UL)

#define RCC_AHB1ENR_GPIOAEN  (1UL << 0)
#define RCC_APB2ENR_USART1EN (1UL << 4)

#define USART_SR_RXNE      (1UL << 5)
#define USART_SR_TXE       (1UL << 7)

#define USART_CR1_RE       (1UL << 2)
#define USART_CR1_TE       (1UL << 3)
#define USART_CR1_UE       (1UL << 13)

#define GPIO_PIN_9         9U
#define GPIO_PIN_10        10U
#define GPIO_AF7_USART     7UL

static void USART1_GPIOInit(void)
{
    RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    (void)RCC_AHB1ENR;

    GPIOA_MODER &= ~((3UL << (GPIO_PIN_9 * 2U)) | (3UL << (GPIO_PIN_10 * 2U)));
    GPIOA_MODER |=  ((2UL << (GPIO_PIN_9 * 2U)) | (2UL << (GPIO_PIN_10 * 2U)));

    GPIOA_OTYPER &= ~((1UL << GPIO_PIN_9) | (1UL << GPIO_PIN_10));

    GPIOA_OSPEEDR &= ~((3UL << (GPIO_PIN_9 * 2U)) | (3UL << (GPIO_PIN_10 * 2U)));
    GPIOA_OSPEEDR |=  ((3UL << (GPIO_PIN_9 * 2U)) | (3UL << (GPIO_PIN_10 * 2U)));

    GPIOA_PUPDR &= ~((3UL << (GPIO_PIN_9 * 2U)) | (3UL << (GPIO_PIN_10 * 2U)));
    GPIOA_PUPDR |=  (1UL << (GPIO_PIN_10 * 2U));

    GPIOA_AFRH &= ~((0xFUL << ((GPIO_PIN_9 - 8U) * 4U)) |
                    (0xFUL << ((GPIO_PIN_10 - 8U) * 4U)));
    GPIOA_AFRH |=  ((GPIO_AF7_USART << ((GPIO_PIN_9 - 8U) * 4U)) |
                    (GPIO_AF7_USART << ((GPIO_PIN_10 - 8U) * 4U)));
}

void USART1_Init(uint32_t pclk_hz, uint32_t baudrate)
{
    USART1_GPIOInit();

    RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
    (void)RCC_APB2ENR;

    USART1_CR1 = 0;
    USART1_CR2 = 0;
    USART1_CR3 = 0;

    USART1_BRR = (pclk_hz + (baudrate / 2UL)) / baudrate;
    USART1_CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void USART1_WriteChar(char ch)
{
    while ((USART1_SR & USART_SR_TXE) == 0U) {
    }

    USART1_DR = (uint32_t)(uint8_t)ch;
}

char USART1_ReadChar(void)
{
    while ((USART1_SR & USART_SR_RXNE) == 0U) {
    }

    return (char)(USART1_DR & 0xFFU);
}

void USART1_WriteString(const char *text)
{
    while (*text != '\0') {
        if (*text == '\n') {
            USART1_WriteChar('\r');
        }

        USART1_WriteChar(*text);
        text++;
    }
}

uint8_t USART1_IsRxReady(void)
{
    return (USART1_SR & USART_SR_RXNE) != 0U;
}

