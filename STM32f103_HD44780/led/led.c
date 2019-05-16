#include "led.h"

const uint16_t LEDS[] = { LED_1, LED_2, LED_3, LED_4 };

void led_init_pins(void)
{
	GPIO_InitTypeDef port;
	
	RCC_APB2PeriphClockCmd(LED_RCC_PORT, ENABLE);
  port.GPIO_Mode = GPIO_Mode_Out_PP;
  port.GPIO_Pin = LED_1 | LED_2 | LED_3 | LED_4;
  port.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED_PORT, &port);
}
	
void led_init(void)
{
	led_init_pins();
}

void led_on_off_one(uint8_t num, uint8_t isOn)
{
	if (isOn)
	{
		GPIO_SetBits(LED_PORT, LEDS[num]);
	}
	else
	{
		GPIO_ResetBits(LED_PORT, LEDS[num]);
	}
}

void led_on_off_all(uint8_t mask_leds)
{
	led_on_off_one(0, mask_leds & 0x01);
	led_on_off_one(1, mask_leds & 0x02);
	led_on_off_one(2, mask_leds & 0x04);
	led_on_off_one(3, mask_leds & 0x08);
}
