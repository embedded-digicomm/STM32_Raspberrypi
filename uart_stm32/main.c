#include "usart1_driver.h"

int main(void)
{
    char ch;

    USART1_Init(USART1_DEFAULT_PCLK_HZ, USART1_DEFAULT_BAUD);
    USART1_WriteString("USART1 driver test ready.\n");
    USART1_WriteString("Type any key and it will echo back.\n");

    while (1) {
        ch = USART1_ReadChar();
        USART1_WriteChar(ch);
    }
}
