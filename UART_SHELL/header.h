#ifndef HEADER_H
#define HEADER_H

void usart_init();
void send_str(char *);
void send_ch(char );
char rec_ch(void);
void recv_str(char*);
void process_cmd(char *);
void led_init();
void red_led_on();
void red_led_off();
void green_led_on();
void green_led_off();
void delay();
#endif