#include "stm32f429xx.h"
#include "spi_header.h"

void spi_init()
{	
	//Enable clk for master spi 
	RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN;
	
	//Enable peripheral clock
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	
	//Clear the bits
	GPIOA->MODER &= ~((3 << 5*2) | (3 << 6*2) | (3 << 7*2));
	
	//Set the gpio pins to alternate function mode
	GPIOA->MODER |= ((2 << 5*2) | (2 << 6*2) | (2 << 7*2));
	
	GPIOA->OSPEEDR |= ((3 << (5*2)) |
                   (3 << (6*2)) |
                   (3 << (7*2)));
	
//	GPIOB->PUPDR &= ~(3 << (14*2));
//GPIOB->PUPDR |=  (1 << (14*2));
	
	GPIOA->AFR[0] &= ~((0xF << (5*4)) |
                   (0xF << (6*4)) |
                   (0xF << (7*4)));
	
	//Set alfternate function values
	GPIOA->AFR[0] |= ((5 << (5*4) )| (5 << (6*4)) | (5 << (7*4)));
	
	SPI1->CR1 = 0;  
	
	//Set SP11 to master mode
	SPI1->CR1 |= SPI_CR1_MSTR;
	
	//Set SSM, to ignore external nss pin
	SPI1->CR1 |= SPI_CR1_SSM;
	
	//Set NSS high internally
	SPI1->CR1 |= SPI_CR1_SSI;
	
	SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA); // SPI mode 0

	
	//Enable SPI2 Peripheral
	SPI1->CR1 |= SPI_CR1_SPE;
	
	

}
