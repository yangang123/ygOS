
#include <rtos.h>
#include "systick.h"

uint32_t  task1_stack[128];
uint32_t  task2_stack[128];
int task1_flag = 0;
int task2_flag = 0;

void task1_entry(void *arg)
{
	while(1){
		task1_flag =1 ;
		ygos_sleep_tick(200);
		task1_flag =0 ;
		ygos_sleep_tick(200);
	}
}

void task2_entry(void *arg)
{
	while(1){
		task2_flag =1 ;
		ygos_sleep_tick(200);
		task2_flag =0 ;
		ygos_sleep_tick(200);
	}
}

int main()
{    
	systick_init();

	ygos_init();
	
    //创建用户任务
	ygos_tcb_create(0, task1_entry, (void*)0, &task1_stack[128]);
	ygos_tcb_create(1, task2_entry, (void*)0, &task2_stack[128]);
	
	ygos_start();
	
	//ygos启动失败,会执行下面
	while(1) {
		//return -1;
	}
}
