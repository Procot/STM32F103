#include "uart.h"

QueueHandle_t usartSendQueue;
uint8_t queueSize = 100;

void USART_Config()
{
    // подаем тактирование на USART1, GPIOA, GPIOD
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    if((usartSendQueue = xQueueCreate(queueSize, sizeof(uint8_t)))==NULL)
    {
        //TODO: Catch error
    }
    
    
    USART_InitTypeDef usart;
    USART_StructInit(&usart);
    usart.USART_BaudRate = 9600;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_StopBits = 1;
    usart.USART_WordLength = 8;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &usart);
    
    usart.USART_BaudRate = 115200;
    USART_Init(USART2, &usart);
    
    USART_Cmd(USART1, ENABLE); // разрешаем работу USART
    USART_Cmd(USART2, ENABLE); // разрешаем работу USART
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // разрешаем прерывание по приему
    // (входной регистр не пуст - RX not empty)
    
    // конфигурируем прерывания на контроллере
    NVIC_InitTypeDef NVIC_InitStructure;
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //конфигурируем количество групп и подгрупп прерываний
  
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //прерывание по usart1 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11; //задаем приоритет в группе
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //задаем приоритет в подгруппе
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //разрешаем прерывание
    NVIC_Init(&NVIC_InitStructure); //инициализируем    
    
    NVIC_EnableIRQ(USART2_IRQn);
        
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    
    //PA9 - TX1
    //PA10 - RX1
    //PD5 - TX2
    //PD6 - RX2
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
    
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Pin = GPIO_Pin_5;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &gpio);
    
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
    
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_6;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &gpio);
    
    // настройка ножек под светодиоды
    /*GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOD, &gpio);*/
    

}

void USART2_IRQHandler()
{
    BaseType_t pxHigherPriorityTaskWoken;
    
    //Проверяем, действительно ли прерывание вызвано приемом нового байта
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {	
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        uint8_t usartData = USART_ReceiveData(USART2);	
        
        if(xQueueSendFromISR(usartSendQueue, &(usartData), &pxHigherPriorityTaskWoken) != pdPASS)
        {
            // TODO: Catch error
        }
        
        if(pxHigherPriorityTaskWoken == pdTRUE)
        {
            portEND_SWITCHING_ISR(pxHigherPriorityTaskWoken);
        }
    }
}


void taskUart(void* params)
{    
    uint8_t usartData;
    
    while(1)
    {
        /*USART_SendData(USART2, 'A');
        vTaskDelay(1000);*/
        xQueueReceive(usartSendQueue, &usartData, portMAX_DELAY);
        
        USART_SendData(USART1, usartData);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
