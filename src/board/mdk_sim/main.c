
#include <rtos.h>
#include "systick.h"

#define TASK1_STATCK_SIZE  128 
#define TASK2_STATCK_SIZE  128 

//任务1和任务2的堆栈
uint32_t  task1_stack[TASK1_STATCK_SIZE];
uint32_t  task2_stack[TASK2_STATCK_SIZE];

//任务
int task1_flag = 0;
int task2_flag = 0;

//任务1入口函数
void task1_entry(void *arg)
{
	while(1){
		//task1_flag变量200个tick进行翻转
		task1_flag =1 ;
		ygos_sleep_tick(200);
		task1_flag =0 ;
		ygos_sleep_tick(200);
	}
}

//任务2入口函数
void task2_entry(void *arg)
{
	while(1){
		//task2_flag变量200个tick进行翻转
		task2_flag =1 ;
		ygos_sleep_tick(200);
		task2_flag =0 ;
		ygos_sleep_tick(200);
	}
}

int main(int argc, char **argv)
{   
	//系统时钟初始化，在系统中断服务程序中任务调度    
	systick_init();
    
	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	ygos_init();
	
    //创建用户任务
	ygos_tcb_create(0, task1_entry, (void*)0, &task1_stack[TASK1_STATCK_SIZE-1]);
	ygos_tcb_create(1, task2_entry, (void*)0, &task2_stack[TASK2_STATCK_SIZE-1]);
	
	//操作系统启动，会触发任务调度到最高优先级的任务
	ygos_start();
	
	//ygos启动失败,会执行下面
	while(1) {
		//return -1;
	}
}
