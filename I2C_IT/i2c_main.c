#include "stm32f429xx.h"
#include "i2c_h.h"
uint8_t data, recv;
void init_config()
{
 gpio_init();
 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	GPIOG->MODER &= ~(3<<(13*2));
	GPIOG->MODER |= (1<<(13*2));

	GPIOG->MODER &= ~(3<<(14*2));
	GPIOG->MODER |= (1<<(14*2));
}
int main()
{
	init_config();
	i2c2_init();
	for(volatile int i=0;i<500000;i++);
	i2c1_init();
	
  
	data='B';
	while(1)
{
	i2c1_tx(data);
	i2c2_slave_poll();
	for(volatile int i=0;i<500000;i++);
}
	
}