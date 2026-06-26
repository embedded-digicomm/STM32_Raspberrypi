#include "stm32f429xx.h"
#include "i2c_h.h"
volatile uint8_t temp;
extern uint8_t data, recv;
uint8_t isr_flag=0,isr1=0,isr=0;
void gpio_init()
{
	//i2c1 : pb6 - scl pb7 - sda 
	//i2c2 : pb10 - scl pb11 - sda
	
	//clk enable for port B
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	
	/*  i2c1 gpio */
	
	//set GPIO mode for PB6 and PB7
	GPIOB->MODER &= ~(3<<(6*2) | 3<<(7*2));
	GPIOB->MODER |= (2<<(6*2) | 2<<(7*2));
	
	//set output type to open drain
	GPIOB->OTYPER |= 1<<6 | 1<<7;
	
	//set the speed
	//GPIOB->OSPEEDR |= (1<<(9*2) | 1<<(8*2));
	GPIOB->OSPEEDR |= (3 << (6*2)) | (3 << (7*2));  // High speed

	
	//set the pull up register
	GPIOB->PUPDR |= (1<<(6*2) | 1<<(7*2));
	
	//set alternate function value
	GPIOB->AFR[0] |= (4<<(6*4) | 4<<(7*4));
	
	/*  i2c2 gpio */
	
	//set GPIO mode for PB10 and PB11
	GPIOB->MODER &= ~(3<<(11*2) | 3<<(10*2));
	GPIOB->MODER |= (2<<(11*2) | 2<<(10*2));
	
	//set output type to open drain
	GPIOB->OTYPER |= 1<<11 | 1<<10;
	
	//set the speed
	GPIOB->OSPEEDR |= (1<<(11*2) | 1<<(10*2));
	
	//set the pull up register
	GPIOB->PUPDR |= (1<<(11*2) | 1<<(10*2));
	
	//set alternate function value
	GPIOB->AFR[1] &= ~((0xF << ((11-8)*4)) | (0xF << ((10-8)*4))); // Clear
GPIOB->AFR[1] |=  (4 << ((11-8)*4)) | (4 << ((10-8)*4));       // Set AF4


}
void i2c1_init()
{
	//i2c1 as master
	
		//clk enable for peripheral
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	
 //reset the Control Register
	I2C1->CR1=(1<<15);
	I2C1->CR1=~(1<<15);
	//set the clk frequency
	I2C1->CR2 =16;
	I2C1->CCR=80;
	I2C1->TRISE=17;
	I2C1->CR1 |=I2C_CR1_PE;	
	
}


void i2c1_tx(char data)
{
	I2C1->CR1 |=I2C_CR1_START;
	while(!(I2C1->SR1 & I2C_SR1_SB));
	temp=I2C1->SR1;
	
	I2C1->DR=(ADD<<1);
	
//	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	//isr=1;
	//temp=I2C1->SR2;

while (!(I2C1->SR1 & (I2C_SR1_ADDR | I2C_SR1_AF)));  // Wait for either

if (I2C1->SR1 & I2C_SR1_AF) {
    // Slave did not ACK
    I2C1->SR1 &= ~I2C_SR1_AF;  // Clear flag
    GPIOG->ODR |= (1 << 14);   // Turn on Red LED or similar
	temp=I2C1->SR2;
    return;
}

	while(!(I2C1->SR1&I2C_SR1_TXE));
	I2C1->DR=data;
	
	while(!(I2C1->SR1&I2C_SR1_BTF));
	I2C1->CR1|=I2C_CR1_STOP;
		
}
void i2c2_init()
{
	//i2c2 as slave
	__disable_irq();
		//clk enable for peripheral
	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	
 //reset the Control Register
	I2C2->CR1=(1<<15);
	I2C2->CR1=~(1<<15);
	//set the clk frequency
	I2C2->CR2 =16;
	I2C2->CCR=80;
	I2C2->TRISE=17;
	
	//give the address
	I2C2->OAR1 = (ADD<<1);
	//7-bit slave address
	I2C2->OAR1 &= ~(1<<15);

	I2C2->CR1 |= I2C_CR1_ACK;
	
	//enable event,error, buffer interrupt 
	//I2C2->CR2 |= (I2C_CR2_ITBUFEN| I2C_CR2_ITERREN |I2C_CR2_ITEVTEN);
	
	I2C2->CR1 |=I2C_CR1_PE;	
	
	
	NVIC_EnableIRQ(I2C2_EV_IRQn);
	__enable_irq();
//	NVIC_SetPriority(I2C2_EV_IRQn, 0);
}
void I2C2_EV_IRQHandler()
{
	isr_flag=2;
	if(I2C2->SR1 & I2C_SR1_ADDR)
	{
		//temp=I2C2->SR2;
		//temp=I2C2->SR1;
		(void)I2C2->SR2;
	}
	if(I2C2->SR1 & I2C_SR1_RXNE)
	{
		recv=I2C2->DR;
		if (recv == 'A')
    {
        GPIOG->ODR |= (1 << 13);  // Turn on LED
    }
	}
	if(I2C2->SR1 & I2C_SR1_STOPF)
	{ 
		//temp=I2C2->SR1;
		(void)I2C2->SR1;
		I2C2->CR1 |= I2C_CR1_ACK;
	}
}


void i2c2_slave_poll()
{
    // 1. Enable ACK
    I2C2->CR1 |= I2C_CR1_ACK;

    // 2. Wait for address match
    while (!(I2C2->SR1 & I2C_SR1_ADDR));
    (void)I2C2->SR1;           // MUST READ SR1
    (void)I2C2->SR2;           // THEN READ SR2 to clear ADDR

    // 3. Wait for data reception
    while (!(I2C2->SR1 & I2C_SR1_RXNE));
    recv = I2C2->DR;           // Now DR has fresh data
}
