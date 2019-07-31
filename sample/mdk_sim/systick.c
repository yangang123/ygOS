
#include "rtos.h"
#include "ARMCM3.h"

//系统systick中断服务程序
void SysTick_Handler(void)
{
	ygos_interrupt_enter();
	ygos_timer_sche();
	ygos_interrupt_leave();
}

//系统systick初始化
void systick_init(void)
{
	SysTick_Config(SystemCoreClock/1000);
}

