
#include <ygos/rtos.h>

//空闲任务
static uint32_t  idle_task_stack[128];
static uint32_t idle_task_count = 0;

//空闲任务
static void  idle_task_entry(void *arg)
{
	while(1){
		//空闲任务变量加1
		idle_task_count++;
	}
}

//空闲任务创建
void ygos_idle_task_init(void)
{   
	//创建空闲任务tcb
	ygos_tcb_create(IDLE_TASK_PRIO, idle_task_entry, (void*)0, &idle_task_stack[127]);
	ygos_task_ready_add(IDLE_TASK_PRIO);
}
	

