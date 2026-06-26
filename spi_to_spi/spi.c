#include "stm32f429xx.h"
#include "SPI_header.h"

void spi_init()
{
	/* SPI Master GPIO init
	   NSS  - PA4
	   SCK  - PA5
	   MISO - PA6
	   MOSI - PA7
	*/
	
	//Enable GPIO clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	//Enable peripheral clk
	RCC->APB2ENR  |= RCC_APB2ENR_SPI1EN;
	
	//Clear bits before setting the mode
	GPIOA->MODER &= ~((3 << 5*2) | (3 << 6*2) | (3 << 7*2));
	
	//Set MODE to alternate function
	GPIOA->MODER |= ((2 << 5*2) | (2 << 6*2) | (2 << 7*2));
	
	//Clear bits before setting the SPEED
	GPIOA->OSPEEDR &= ~((3 << 5*2) | (3 << 6*2) | (3 << 7*2));
	
	//Set to high speed
	GPIOA->OSPEEDR |= ((2 << 5*2) | (2 << 6*2) | (2 << 7*2));

	//Clear bits before setting the mode
	GPIOA->AFR[0] &= ~((0XF << 5*4) | (0XF << 6*4) | (0XF << 7*4));
	
	//Set to respective alternate fucntioN
	GPIOA->AFR[0] |= ((5 << 5*4) | (5 << 6*4) | (5 << 7*4));
	
	//Manual NSS output
	GPIOA->MODER &= ~(3<<4*2);
	GPIOA->MODER |= (1<<4*2);
	
	//Initially set nss pin high, i.e no slave selected
	GPIOA->BSRR |=(1<<4);
	
	//SPI initialization
	
	SPI1->CR1 =0X00;//Clear any previous bits
	
	//Set master bit
	SPI1->CR1 |= 
	            ( SPI_CR1_MSTR |
							  SPI_CR1_SSM |
								SPI_CR1_SSI );							
	
	//Enable peripheral
	SPI1->CR1 |= SPI_CR1_SPE;
	
	
	/*********************************************************/
	
	
	/* SPI Slave GPIO init, SPI4
	   NSS  - PE4
	   SCK  - PE2
	   MISO - PE5
	   MOSI - PE6
	*/
	
	//Enable GPIO clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	
	//Enable peripheral clk
	RCC->APB2ENR  |= RCC_APB2ENR_SPI4EN;
	
	//Clear bits before setting the mode
	GPIOE->MODER &= ~((3 << 2*2) | (3 << (4*2)) | (3 << 5*2) | (3 << 6*2));
	
	//Set MODE to alternate function
	GPIOE->MODER |= ((2 << 2*2) | (2 << (4*2)) | (2 << 5*2) | (2 << 6*2));

	//Clear bits before setting the SPEED
	GPIOE->OSPEEDR &= ~((3 << 2*2) | (3 << (4*2)) | (3 << 5*2) | (3 << 6*2));
	
	//Set to high speed
	GPIOE->OSPEEDR |= ((2 << 2*2) | (2 << (4*2)) | (2 << 5*2) | (2 << 6*2));



	//Clear bits before setting the mode
	GPIOE->AFR[0] &= ~((0XF << 2*4) | (0xF << (4*4)) | (0XF << 5*4) | (0XF << 6*4));
	
	//Set to respective alternate fucntioN
	GPIOE->AFR[0] |= ((5 << 2*4) | (5 << (4*4)) | (5 << 5*4) | (5 << 6*4));
	
	//SPI initialization
	
	SPI4->CR1 =0X00;//Clear any previous bits
	
	//SPI4->CR1 &= ~SPI_CR1_MSTR;   // ensure SLAVE
//SPI4->CR1 &= ~SPI_CR1_SSM;    // hardware NSS
//SPI4->CR2 = 0;
	
	SPI4->CR1 |= SPI_CR1_SPE;//Enable peripheral
	
}

void send_data(uint8_t d)
{
	//pull nss pin low
	GPIOA->BSRR |= (1<<(4+16));//Bit reset
	
	while(!(SPI1->SR & SPI_SR_TXE));
	*((volatile uint8_t*)&SPI1->DR) = d;
	
	while(SPI1->SR & SPI_SR_BSY);
	
	//set the nss pin to high
	GPIOA->BSRR|= (1<<4);
	
}
	
uint8_t recv_data()
{
	while(!(SPI4->SR & SPI_SR_RXNE));
	
	return *((volatile uint8_t*)&SPI4->DR);
	
}