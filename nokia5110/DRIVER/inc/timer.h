#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"
#include "misc.h"

extern __IO uint32_t TimingDelay;

void systick_init(__IO uint32_t systick);
extern void delay_ms(__IO uint32_t nTime);
extern void delay_us(__IO uint32_t nTime);
void TimerDelay_Decrement(void);

#endif