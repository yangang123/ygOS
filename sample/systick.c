
#include "rtos.h"
#include "ARMCM3.h"

void SysTick_Handler(void)
{
	ygos_interrupt_enter();
	ygos_timer_sche();
	ygos_interrupt_leave();
}

void systick_init(void)
{
	SysTick_Config(SystemCoreClock/1000);
}

