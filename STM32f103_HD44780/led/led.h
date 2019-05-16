#ifndef __LED_DEFINE__
#define __LED_DEFINE__

#include <stm32f10x_gpio.h>

#define LED_RCC_PORT		RCC_APB2Periph_GPIOA
#define LED_PORT				GPIOA
#define LED_1						GPIO_Pin_8
#define LED_2						GPIO_Pin_9
#define LED_3						GPIO_Pin_10
#define LED_4						GPIO_Pin_11

void led_on_off_one(uint8_t num, uint8_t isOn);
void led_on_off_all(uint8_t mask_leds);
void led_init_pins(void);
void led_init(void);

#endif
