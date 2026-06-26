#include "stm32f429xx.h"
#include "SPI_header.h"

int main()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	
	GPIOG->MODER &= ~(3<<13*2);
	GPIOG->MODER |= (1<<13*2);
	
	spi_init();
	uint8_t data=0x55, rx;
	
	send_data(data);
	rx=recv_data();
	if (data==rx)
	{
		while(1)
		{
			GPIOG->ODR ^= (1<<13);
			for(volatile int i=0;i<100000;i++);
			//GPIOG->ODR |= (1<<13);
			
		}
	}
}