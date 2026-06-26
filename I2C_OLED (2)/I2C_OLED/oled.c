#include "stm32f429xx.h"
#include "oled_header.h"
extern const uint8_t font6x8[][6]; 
#define OLED_ADDR 0x78
#define CMD 0x00
#define DATA 0x40
//CMD and DATA are control bytes, which tell the oled 
//the data sent is a cmd or data byte

void delay_ms(uint32_t ms)
{
	for(volatile uint32_t i=0;i< ms*16000;i++);
}
void OLED_SetCursor(uint8_t col, uint8_t page) {
    OLED_CMD(0xB0 | page);                      // Page address
    OLED_CMD(0x00 | (col & 0x0F));                // Lower nibble column
    OLED_CMD(0x10 | ((col >> 4) & 0x0F));         // Higher nibble column
}
void OLED_CMD(uint8_t cmd)
{
	i2c_start();
	i2c_write_addr(OLED_ADDR);
	i2c_write(CMD);//Tell OLED user is sending a cmd, i.e control byte
	i2c_write(cmd);
	i2c_stop();
}
void OLED_DATA(uint8_t data)
{
  i2c_start();
	i2c_write_addr(OLED_ADDR);
	i2c_write(DATA);//Tell OLED user is sending a data, i.e control byte
	i2c_write(data);
	i2c_stop();	
}
void OLED_init()
{
	//delay_ms(100);
	OLED_CMD(0xAE);//Display OFF
	OLED_CMD(0xD5);OLED_CMD(0x80);//Set display clk 
	//The cmd 0xA8 tells oled, it will assign the no.of COM lines, i.e parameter for 0xA8
	//0xA8 is nothing but setting the MUX ratio
	//0x3F=63, (0-63) hence MUX=64
	OLED_CMD(0xA8);OLED_CMD(0x3F);//set 64 mux lines
	//0xD3 set the diplay offset(cusor pos)
	OLED_CMD(0xD3);OLED_CMD(0x00);//starting position
	OLED_CMD(0x40);//set display start line
	
	//0x8D set the internal charge pump
	OLED_CMD(0x8D);OLED_CMD(0x14);//Enable Charge Pump 
	//Horizontal addressing controls how RAM is accessed during display
	OLED_CMD(0x20);OLED_CMD(0x00);//Horizontal addressing
	OLED_CMD(0xA1);//segment re-map (mirror X)
	OLED_CMD(0xC8);//COM scan direction (mirror Y)
	OLED_CMD(0xDA);OLED_CMD(0x12);// set COM pins
	OLED_CMD(0x81);OLED_CMD(0xCF);//Set contrast, 0xCF value suggested by internet
	//Set pre charge period, controls the timing of pixel charge
  //0xXY => X=Phase 2, Y=Phase 1, from datasheet, the values start from 2
	//Hence the value will be 0x22
	OLED_CMD(0xD9);OLED_CMD(0x22);
	
	//0xDB controls how off pixels are managed
	//0x20 is the reset value
	OLED_CMD(0XDB);OLED_CMD(0x20);
	
	OLED_CMD(0xA4);//Set OLED to display RAM content
	OLED_CMD(0xA6);//Set normal display
	OLED_CMD(0xAF);//Display ON
	
}
void OLED_clear()
{
	for(uint8_t pg=0;pg<8;pg++)
	{
		OLED_CMD(0xB0+pg);//set the page start address
		OLED_CMD(0x00);//lower column address
		OLED_CMD(0x10);//higher column address
		for(uint8_t col=0;col<128;col++)
		{
			OLED_DATA(0x00);
		}
	}
}
void OLED_display_char(uint8_t ch)
{
  const uint8_t *bitmap = font6x8[ch-31];
//here the font6x8[ch-32], gives the address  of 1D array,
//of 6 bytes, for a character	
	//hence the base address is stored into a pointer
	for(uint8_t i=0;i<6;i++)
	{
		OLED_DATA(bitmap[i]);
	}
}
void OLED_display_str(char *str)
{
 while(*str)
{
	OLED_display_char(*str++);
}
}