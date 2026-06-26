#include "stm32f429xx.h"
#include "header.h"
#include <stdio.h>
#include <string.h>

void delay()
{
		for(volatile int i=50000;i--; );
}
int main()
{
	usart_init();
	delay();
	char s[100];
	send_str("hello\n");
	
	while(1)
	{
		send_str("\n\rEnter a command/ type 'help': ");
		delay();
		recv_str(s);
		delay();
		process_cmd(s);
		delay();
		// send_str(s);
		
	}
}

void process_cmd(char *cmd)
{
	if (!strcmp(cmd, "help") || !strcmp(cmd,"Help"))
  {
    send_str(
        "\r\nType any of these commands:\r\n"
        "1. 'red on'  -> Turns red LED on\r\n"
        "2. 'red off' -> Turns red LED off\r\n"
        "3. 'green on' -> Turns green LED on\r\n"
        "4. 'green off'-> Turns green LED off\r\n"
		    "5. 'both on' or 'both off'-> Turns off both green and red leds\r\n"
    );
  }
	else if(!strcmp(cmd,"red on"))
	{
		led_init();
		red_led_on();
		send_str("\r\nLED ON");
	}
	else if(!strcmp(cmd,"red off"))
	{
		led_init();
		red_led_off();
		send_str("\r\nLED OFF");
	}
	else if(!strcmp(cmd,"green on"))
	{
		led_init();
		green_led_on();
		send_str("\r\nLED ON");
	}
	else if(!strcmp(cmd,"green off"))
	{
		led_init();
		green_led_off();
		send_str("\r\nLED OFF");
	}
	else if(!strcmp(cmd,"both off"))
	{
		led_init();
		green_led_off();
		red_led_off();
		send_str("\r\nBoth leds OFF");
	}
	else if(!strcmp(cmd,"both on"))
	{
		led_init();
		green_led_on();
		red_led_on();
		send_str("\r\nBoth leds are ON");
	}
	else
		send_str("\r\nInvalid cmd\n");	
}