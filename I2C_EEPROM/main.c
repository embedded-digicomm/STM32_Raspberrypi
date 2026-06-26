#include "stm32f429xx.h"
#include "i2c_header.h" 

#define EEPROM_ADDR 0x50
void init()
{
	gpio_init();
	i2c_init();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER &= ~(3<<13*2);
	GPIOG->MODER |= (1<<13*2);
	GPIOG->MODER &= ~(3<<14*2);
	GPIOG->MODER |= (1<<14*2);

}

int main()
{
	init();
		eeprom_write(0x0010, 0xAB);
	for(volatile int i=0;i<100000;i++);
	uint8_t val=eeprom_read(0x0010);
	
	while(1)
	{
  		if(val==0XAB)
			{
			GPIOG->ODR ^= (1<<13);
			for(volatile int i=0;i<1000000;i++);
			GPIOG->ODR ^= (1<<14);
			}
	}
	
}