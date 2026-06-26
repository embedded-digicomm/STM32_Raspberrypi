#include "stm32f429xx.h"
#include "oled_header.h"
void gpio_init()
{
	
	//enable clk for gpio port
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;
	
	//clear and set the AF mode for the GPIO port B 
	GPIOB->MODER &=~((3<<(6*2)) | (3<<(7*2)));
	GPIOB->MODER |= (2<<(6*2)) | (2<<(7*2));
	
	//clear and set the type to open drain
	GPIOB->OTYPER &= ~((1<<6) | (1<<7));
	GPIOB->OTYPER |= (1<<6) | (1<<7);
	
	
	//Set the speed to high speed
	//GPIOB->OSPEEDR |=(2<<(6*2) | 2<<(7*2));
	GPIOB->OSPEEDR |=(1<<(6*2) | 1<<(7*2));
	
	//since we are using the oled, we do not need the internal resistance
	GPIOB->PUPDR=0;
//	GPIOB->PUPDR |=(1<<(6*2))|(1<<(7*2));
	
	//Set the alternate function in AFR register
	GPIOB->AFR[0] &=~(0x0F << (6*4) | 0x0F << (7*4));
	GPIOB->AFR[0] |= (4 << (6*4) | 4 << (7*4));
	
	//enable clk for i2c
	RCC->APB1ENR|=RCC_APB1ENR_I2C1EN;
	
//	RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
  //RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
}
void i2c_init()
{
	//reset the Control Register
	I2C1->CR1=(1<<15);
	I2C1->CR1=~(1<<15);
	//set the clk frequency
	I2C1->CR2 =16;
	I2C1->CCR=80;
	I2C1->TRISE=17;
	I2C1->CR1 |=I2C_CR1_PE;	
}
void i2c_start()
{
	//while (I2C1->SR2 & I2C_SR2_BUSY);  // Wait if bus is stuck
	
I2C1->CR1 |= I2C_CR1_START;
while (!(I2C1->SR1 & I2C_SR1_SB));
}
void i2c_stop()
{
	I2C1->CR1|=I2C_CR1_STOP;
}
void i2c_write_addr(uint8_t addr)
{
	
	I2C1->DR=addr;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	volatile uint8_t temp= I2C1->SR1 | I2C1->SR2;
}
void i2c_write(uint8_t data)
{
	while(!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR=data;
	while(!(I2C1->SR1 & I2C_SR1_BTF));
}
//font6x8 array starts from ASCII 32, which is the space character ' '