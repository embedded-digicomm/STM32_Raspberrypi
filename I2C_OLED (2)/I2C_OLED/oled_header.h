#ifndef OLED_HEADER_H
#define OLED_HEADER_H
void delay_ms(__UINT32_TYPE__ ms);
void OLED_SetCursor(__UINT8_TYPE__ , __UINT8_TYPE__ );
void gpio_init();
void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write_addr(__UINT8_TYPE__ addr);
void i2c_write(__UINT8_TYPE__ data);
void OLED_init();
void OLED_clear();
void OLED_CMD(__UINT8_TYPE__);
void OLED_DATA(__UINT8_TYPE__);
void OLED_display_char(__UINT8_TYPE__);
void OLED_display_str(char *str);
#endif