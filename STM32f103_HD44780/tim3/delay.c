#include "delay.h"

volatile uint8_t f_timer_2_end;
void TIM2_init(void)
{
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructInit(&TIMER_InitStructure);
	TIMER_InitStructure.TIM_Prescaler = 64 - 1;
	TIMER_InitStructure.TIM_Period = 1;
	TIM_TimeBaseInit(TIM2, &TIMER_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// считаем один раз
	TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);
}
void TIM2_IRQHandler(void)
{
  extern volatile uint8_t f_timer_2_end;
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  TIM2->SR &= ~TIM_SR_UIF;
  f_timer_2_end = 1;
  TIM_Cmd(TIM2, DISABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
}

void _delay_us(uint16_t n_usec)
{
  f_timer_2_end = 0;
  TIM2->ARR = n_usec;
  TIM_Cmd(TIM2, ENABLE);
  // для того чтобы установился PSC
  TIM2->EGR |= TIM_EGR_UG;
  TIM2->SR &= ~TIM_SR_UIF;
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);

  while(f_timer_2_end == 0);
}

void _delay_ms(uint16_t n_msec)
{
	_delay_us(n_msec * 1000);
}


void initRCC(void)
{
	RCC_DeInit();
	RCC_HSICmd(DISABLE);
	RCC_HSEConfig(RCC_HSE_ON); // разрешаем тактирование от внешнего генератора

	// ожидаем, пока внешний генератор будет готов к работе
	ErrorStatus HSEStartUpStatus = RCC_WaitForHSEStartUp();

	// если запуск внешнего генератора прошел успешно
	if (HSEStartUpStatus == SUCCESS)
	{
		/*
		Частота кварца - 8Мгц
		HSE(8 Mhz) -> PLL(x8 = 64 Mhz) -> SYSCLOCK_MUX -> SYSCLOCK(64 Mhz)
		AHB Prescaler = 1
		ABP1 Prescaler = /4
		APB2 Prescaler = /1
		
		*/
		
		//PLL
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_8);
		RCC_PLLCmd(ENABLE);
		
		//Выбираем источником PLL
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		//AHB Prescaler
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		
		//Настройка предделителей APBx
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);

		// ожидаем, пока HSE не установится в качестве источника SYSCLOCK
		while (RCC_GetSYSCLKSource() != 0x08){}
	}
	else
	{   //Если HSE не смог запуститься, тактирование настроено некорректно
		//Здесь следует поместить код обработчика этой ошибки
		while (1){} // бесконечный цикл
	}
}
