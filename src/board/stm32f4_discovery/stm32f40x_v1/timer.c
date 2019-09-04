
#include "timer.h"
#include <ygos/rtos.h>

void TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x08;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void system_tim2_init (void)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period =  TIME_TICK_US - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1 ;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 

	TIM2_NVIC_Config();  
	
	TIM_Cmd(TIM2, ENABLE);
}

void time2_close_interrput(void)
{	
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
	TIM_Cmd(TIM2, DISABLE);
}

void TIM2_IRQHandler(void)
{  
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)==SET) {   
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
		
		ygos_interrupt_enter();
		ygos_timer_sche();
		ygos_interrupt_leave();
		
	}
}


