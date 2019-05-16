#ifndef __KEYBOARD__
#define __KEYBOARD__

#include <stm32f10x_gpio.h>

#define ROW1 						GPIO_Pin_4
#define ROW2 						GPIO_Pin_5
#define ROW3 						GPIO_Pin_6
#define ROW4 						GPIO_Pin_7

#define COL1 						GPIO_Pin_0
#define COL2 						GPIO_Pin_1
#define COL3 						GPIO_Pin_2
#define COL4 						GPIO_Pin_3

#define PORT_ROWS				GPIOA
#define PORT_COLS				GPIOA
#define RCC_PORT_ROWS		RCC_APB2Periph_GPIOA
#define RCC_PORT_COLS		RCC_APB2Periph_GPIOA

void keyboard_pins_init(void);
unsigned char keyboard_key_press(void);
void keyboard_init(void);

#endif
