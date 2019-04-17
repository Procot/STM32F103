#ifndef __LCD__
#define __LCD__

#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "../tim3/delay.h"

#define RS 							GPIO_Pin_9
#define EN 							GPIO_Pin_5
#define PORT_SIG				GPIOB
#define RCC_PORT_SIG		RCC_APB2Periph_GPIOB

#define D4							GPIO_Pin_15
#define D5							GPIO_Pin_14
#define D6							GPIO_Pin_13
#define D7							GPIO_Pin_12
#define PORT_DATA				GPIOB
#define RCC_PORT_DATA		RCC_APB2Periph_GPIOB

#define COMMAND 				Bit_RESET
#define DATA 						Bit_SET

void lcd_clear(void);
void lcd_cursor_on_begin(void);
void lcd_send_enter(void);
void lcd_send_str(char* str);
void lcd_send_char(unsigned char c);
void lcd_send_pol_byte(unsigned char c, BitAction bitVal);
void lcd_send_byte(unsigned char p, BitAction bitVal);
void lcd_init_pins(void);
void lcd_init(void);

#endif
