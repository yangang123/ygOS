
#include "rtos.h"

//ygos_
void  ygos_sleep_tick (int32_t ticks)
{ 
	int level;
    if (ygos_interrupt_nest > 0u) {                  
        return;
    }

    if (ticks > 0u) {                          
        ygos_interrupt_disable();
        ygos_tcb_current->sleep_tick = ticks;  
	    //把当前任务从就绪列表删除
		ygos_task_ready_delete(ygos_prio_current);
        ygos_interrupt_enable(level);
        ygos_sche();                              
    }
}