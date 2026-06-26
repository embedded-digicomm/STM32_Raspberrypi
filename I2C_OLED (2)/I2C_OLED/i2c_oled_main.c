#include "stm32f429xx.h"
#include "oled_header.h"
#include<stdint.h>
#include<stdio.h>
void init_config()
{
	gpio_init();
	i2c_init();
	OLED_init();
	OLED_clear();
}
int main()
{
	init_config();
	//pass col and page to set cursor
	 
	//for (uint8_t i = 0; i < 128; i++) {
    //    OLED_DATA(0xAA);  // Pattern: 10101010
    //}
	//OLED_display_char('1')
	//char buf[100];
	//printf("Enter a str:\n");
	OLED_SetCursor(0, 0);
	OLED_display_str("Enter str:");
//	scanf(" %s",buf);
	//OLED_display_str(buf);
  OLED_SetCursor(0, 2);
	//OLED_display_str(buf);	
//	OLED_SetCursor(0, 4);
	
    while (1);
}