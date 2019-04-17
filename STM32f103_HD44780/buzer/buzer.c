#include "buzer.h"

void init_PWM(void)
{
	GPIO_InitTypeDef GPIO_Options;
	TIM_TimeBaseInitTypeDef TIM_BaseOptions;
	TIM_OCInitTypeDef TIM_PWM_Options;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//PB.0 TIM3_CH3, BUZZER
	GPIO_Options.GPIO_Pin = GPIO_Pin_0;
	GPIO_Options.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Options.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_Options);

	TIM_BaseOptions.TIM_Period = 1;
	TIM_BaseOptions.TIM_Prescaler = 1;
	TIM_BaseOptions.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseOptions.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(TIM3, &TIM_BaseOptions);

	TIM_PWM_Options.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_PWM_Options.TIM_OutputState = TIM_OutputState_Enable;
	TIM_PWM_Options.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_PWM_Options.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_PWM_Options.TIM_Pulse = 1;
	TIM_OC3Init(TIM3, &TIM_PWM_Options);

	TIM_Cmd(TIM3, ENABLE);
	return;
}

void disable_PWM(void)
{			
	TIM_OCInitTypeDef PWM_InitStructure;
	TIM_OCStructInit(&PWM_InitStructure);
	PWM_InitStructure.TIM_OutputState = TIM_OutputState_Disable;
	TIM_OC3Init(TIM3, &PWM_InitStructure);
	TIM_Cmd(TIM3, DISABLE);
}
