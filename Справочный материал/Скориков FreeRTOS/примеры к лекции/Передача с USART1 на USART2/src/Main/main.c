#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

#include <FreeRTOS.h>
#include <task.h>

#include "uart.h"

void task1(void* params);
void task2(void* params);

int main()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOD, &gpioInit);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    
    USART_Config();
    
    
    xTaskCreate(task1, "task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(task2, "task2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(taskUart, "taskUart", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
    
    vTaskStartScheduler();
    
    while(1)
    {
        GPIOD->ODR ^= GPIO_Pin_0;
        for(int i = 0; i < 1000000; i++);
        GPIOD->ODR ^= GPIO_Pin_0;
        GPIOD->ODR ^= GPIO_Pin_1;
        for(int i = 0; i < 1000000; i++);
    }
    
    return 0;
}


void task1(void* params)
{    
    while(1)
    {
        GPIOD->ODR ^= GPIO_Pin_0;
        vTaskDelay(250);
        //for(int i = 0; i < 1000000; i++);
    }
}

void task2(void* params)
{
    while(1)
    {
        GPIOD->ODR ^= GPIO_Pin_1;
        vTaskDelay(500);
        //for(int i = 0; i < 2000000; i++);
    }
}
