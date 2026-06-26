#include "stm32f429xx.h"
#include "i2c_header.h" 

void gpio_init()
{
	//I2C1
	//PB6-SCL PB7-SDA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	GPIOB->MODER &= ~((3<<(6*2)) | (3<<(7*2)));
	GPIOB->MODER |= (2<<(6*2))|(2<<(7*2));
	
	//Enable open drain for both the pins
	GPIOB->OTYPER  |= (1 << 6) | (1 << 7);   
	
	//Enable pull-up fot both pins
	GPIOB->PUPDR |= (1<<(6*2)| 1<<(7*2));
	
	//Enable the alternate function 
	GPIOB->AFR[0] &= ~((0XF<<(6*4))|(0XF<<(7*4)));
	GPIOB->AFR[0] |= (4<<(6*4))|(4<<(7*4));
	
}

void i2c_init()
{
	//Enable peripheral clk
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	

  I2C1->CR1 &= ~I2C_CR1_PE;

  I2C1->CR2  = 45;   // 45 MHz APB1
	//CCR=Fpclk/(2xI2C_speed)
	//=45MHz/(2x100kHz)
  I2C1->CCR  = 225;  // 100 kHz
	//Max rise time=> Fpclk+1
	//Fpclk -> peripheral clk freq
  I2C1->TRISE = 46;
	
  //Enable peripheral
  I2C1->CR1 |= I2C_CR1_PE;
}


void send_addr(uint8_t addr, uint8_t rw)
{
	I2C1->DR=(addr<<1)|rw;
	
	//while(!(I2C1->SR1 & I2C_SR1_ADDR));
	while(!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));
	/*if (I2C1->SR1 & I2C_SR1_AF)
    {
        // EEPROM NOT responding
        GPIOG->ODR |= (1 << 13); // turn ON LED permanently
        while(1);
    }*/
	(void)I2C1->SR1;
	(void)I2C1->SR2;
	//clear ADDR flag
}
void i2c_start()
{
	I2C1->CR1 |= I2C_CR1_START;
	//wait for the start generation
	while(!(I2C1->SR1 & I2C_SR1_SB));
}
	
void i2c_stop()
{
	I2C1->CR1 |= I2C_CR1_STOP;
}

void i2c_write(uint8_t data)
{
	I2C1->DR=data;
	//wait for byte transfer finish data bit to be set
	while(!(I2C1->SR1 & I2C_SR1_BTF));
}
