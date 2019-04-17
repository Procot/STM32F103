#include "stm32f10x.h"                  // Device header
#include "misc.h"                       // Keil::Device:StdPeriph Drivers:Framework

#include <FreeRTOS.h>
#include "queue.h"
#include <task.h>

#include "lcd/hd44780.h"
#include "keyboard/keyboard.h"
#include "buzer/buzer.h"
#include "led/led.h"

QueueHandle_t keyboardQueue, lcdQueue, buzzerQueue;
uint8_t queueSize = 10;

void taskLCD(void *params);
void taskKeyBoard(void *params);
void taskLEDS(void *params);
void taskBuzzer(void *params);
void taskSwitch(void *params);

int main(void)
{
	initRCC();
	lcd_init();
	keyboard_init();
	led_init();
	
	keyboardQueue = xQueueCreate(queueSize, sizeof(unsigned char));
	lcdQueue = xQueueCreate(queueSize, sizeof(unsigned char));
	buzzerQueue = xQueueCreate(queueSize, sizeof(unsigned char));
	
	xTaskCreate(taskLCD, "taskLCD", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(taskKeyBoard, "taskKeyBoard", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(taskLEDS, "taskLEDS", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(taskSwitch, "taskSwitch", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(taskBuzzer, "taskBuzzer", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	vTaskStartScheduler();
	
	while(1);
}

void taskLCD(void *params)
{
	uint16_t SYMBOL_COUNTER = 0;
	unsigned char c;

	while(1)
	{
		xQueueReceive(lcdQueue, &c, portMAX_DELAY);
		if (SYMBOL_COUNTER == 32)
		{
			lcd_clear();
			lcd_cursor_on_begin();
			SYMBOL_COUNTER = 0;
		}
		
		lcd_send_char(c);
		if(++SYMBOL_COUNTER == 16) {
			lcd_send_enter();
		}
	}
}

void taskKeyBoard(void *params)
{
	unsigned char c;

	while(1)
	{
		c = keyboard_key_press();
		xQueueSend(keyboardQueue, &c, portMAX_DELAY);
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void taskLEDS(void *params)
{
	uint8_t number = 0;
	int dir = 1;
	while(1)
	{
		led_on_off_all(1 << number);
		
		if (number == 3)
		{
			dir = -1;
		}
		if (number == 0)
		{
			dir = 1;
		}
		
		number += dir;
		
		vTaskDelay(100 / portTICK_PERIOD_MS);
	}
}

void taskBuzzer(void *params)
{
	unsigned char c;
	u8 state = 0;
	
	while(1)
	{
		xQueueReceive(buzzerQueue, &c, portMAX_DELAY);
		if (c == 'B')
		{
			if (state)
			{
				disable_PWM();
			}
			else
			{
				init_PWM();
			}
			
			state = !state;
		}
	}
}

void taskSwitch(void *params)
{
	unsigned char c;
	
	while(1)
	{
		xQueueReceive(keyboardQueue, &c, portMAX_DELAY);
		xQueueSend(lcdQueue, &c, portMAX_DELAY);
		if (c == 'B')
		{
			xQueueSend(buzzerQueue, &c, portMAX_DELAY);
		}
	}
}