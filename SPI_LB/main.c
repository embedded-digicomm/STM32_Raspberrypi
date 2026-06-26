#include "stm32f429xx.h"
#include <stdio.h>
#include "spi_header.h"
int main()
{
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER &= ~(3<<(13*2));//CONFIG LED AS OUTP
	GPIOG->MODER |= (1<<(13*2));//CONFIG LED AS OUTPUT
	spi_init();
	
	uint32_t moder = GPIOB->MODER;
uint32_t afr = GPIOB->AFR[1];
	
	/*uint8_t data=0xA0, recv;
	
	while(!(SPI2->SR & SPI_SR_TXE));
	*((volatile uint8_t*)&SPI2->DR)=data;

	
	while(!(SPI2->SR & SPI_SR_RXNE));
	recv=*((volatile uint8_t*)&SPI2->DR);
	
	while(SPI2->SR & SPI_SR_BSY);
		
uint8_t sr = SPI2->SR;
	while(1)
	{
		if(recv==data)
			GPIOG->ODR |= (1<<13);
	}
	*/
	
	 while (1)
    {
        uint8_t data = 0xA0;
        uint8_t recv;

        /* send */
        while (!(SPI1->SR & SPI_SR_TXE));
        *((volatile uint8_t*)&SPI1->DR) = data;

        /* wait receive */
        while (!(SPI1->SR & SPI_SR_RXNE));
        recv = *((volatile uint8_t*)&SPI1->DR);

        /* wait completion */
        while (SPI1->SR & SPI_SR_BSY);

        /* LED check */
        if (recv == data)
            GPIOG->ODR |= (1 << 13);
        else
            GPIOG->ODR &= ~(1 << 13);
    }
	
}