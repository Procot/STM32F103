#ifndef __timerDelayInit__
#define __timerDelayInit__
#include <stm32f10x_tim.h>
#include <misc.h>
#include <FreeRTOS.h>
#include <task.h>

void _delay_us(uint16_t n_usec);
void _delay_ms(uint16_t n_msec);
void initRCC(void);
void TIM2_init(void);

#endif
