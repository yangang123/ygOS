
#include "rtos.h"

//当前任务休眠ticks个系统滴答
void  ygos_sleep_tick (int32_t ticks)
{ 
	int level;
    //在中断服务程序中不允许调用休眠函数
    if (ygos_interrupt_nest > 0u) {                  
        return;
    }

    if (ticks > 0u) {                          
        level = ygos_interrupt_disable();
        //把ticks的值赋值给我TCB的sleep_tick
        ygos_tcb_current->sleep_tick = ticks;  
	    //把当前任务从就绪列表删除
		ygos_task_ready_delete(ygos_prio_current);
        ygos_interrupt_enable(level);

        //任务休眠后，任务调度到最高优先级的任务
        ygos_sche();                              
    }
}
