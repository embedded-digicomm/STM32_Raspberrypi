#include "stm32f429xx.h"
#define i2c_h
#define ADD 0x42

void gpio_init();
void i2c1_init();
void i2c2_init();
void i2c_start();
void i2c1_tx(char c);
void I2C1_EV_IRQHandler();
void I2C2_EV_IRQHandler();
void i2c2_slave_poll();