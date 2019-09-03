#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f4xx.h" 


#define TIME_TICK_MS                                     1  
#define TIME_TICK_US                                     (TIME_TICK_MS * 1000)  

#define TIME_SYS_ST_DEBUG   0 

#define TIME_CMD_ENABLE(nTIM)  nTIM->CR1 |= (uint16_t)TIM_CR1_CEN;
#define TIME_CMD_DISABLE(nTIM) nTIM->CR1 &= (uint16_t)~TIM_CR1_CEN;

void system_tim2_init(void);
void time2_close_interrput(void);

#endif /*  _TIMER_H  */
