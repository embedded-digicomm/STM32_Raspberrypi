#include "stm32f429xx.h"
#include "i2c_header.h"

uint8_t eeprom_read(uint16_t mem_addr)
{
	uint8_t data;
	
	i2c_start();
	send_addr(EEPROM_ADDR, 0);
	
	i2c_write(mem_addr>>8);
	i2c_write(mem_addr&0xFF);
	
	//Repeated start
	i2c_start();
	send_addr(EEPROM_ADDR,1);
	
	//I2C1->CR1 &= ~I2C_CR1_ACK;
	
	while(!(I2C1->SR1 & I2C_SR1_RXNE));
	data=I2C1->DR;
	i2c_stop();
	
	return data;
	
}

void eeprom_write(uint16_t mem_addr, uint8_t data)
{
	i2c_start();
	
	//select eeprom, write mode
	send_addr(EEPROM_ADDR,0);
	
	//after ack, send memory address
	i2c_write((uint8_t)mem_addr>>8);
	i2c_write((uint8_t)mem_addr & 0xFF);
	
	i2c_write(data);
	
	i2c_stop();
	
	
	
}