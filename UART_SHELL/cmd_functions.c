#include "stm32f429xx.h"
#include "header.h"
#include <stdio.h>
#include <string.h>

void led_init()
{
	//Enable clk for port G
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
	
	//RED Led
	GPIOG->MODER &= ~(3 << (14*2));
	GPIOG->MODER |=(1<<(14*2));
	
	//GREEN LED
	GPIOG->MODER &= ~(3 << (13*2));
	GPIOG->MODER |=(1<<(13*2));
	
}
void red_led_on()
{
	GPIOG->ODR &= ~(1<<14);
	GPIOG->ODR |= (1<<14);
	delay();
}
void red_led_off()
{
	GPIOG->ODR &= ~(1<<14);
}
void green_led_on()
{
		GPIOG->ODR &= ~(1<<13);
	GPIOG->ODR |= (1<<13);
	delay();
}
void green_led_off()
{
	GPIOG->ODR &= ~(1<<13);
}