#ifndef i2c_header_h
#define i2c_header_h
#define EEPROM_ADDR 0x50
void gpio_init();
void i2c_init();
void i2c_start();
void i2c_stop();
void send_addr(__UINT8_TYPE__, __UINT8_TYPE__);
void i2c_write(__UINT8_TYPE__);
__UINT8_TYPE__ eeprom_read(__UINT16_TYPE__ mem_addr);
void eeprom_write(__UINT16_TYPE__ , __UINT8_TYPE__);
#endif