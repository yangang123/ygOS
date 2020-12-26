#include <ygos/rtos.h>

#define TASK1_STATCK_SIZE  128 
#define TASK2_STATCK_SIZE  128 

//任务1和任务2的堆栈
uint32_t  task1_stack[TASK1_STATCK_SIZE];
uint32_t  task2_stack[TASK2_STATCK_SIZE];

//任务
int task1_flag = 0;
int task2_flag = 0;

//信号量变量
sem_t sem_mavlink1;
sem_t sem_mavlink2;

//任务1入口函数
void task1_entry(void *arg)
{     
	while(1){
		ygos_sleep_tick(5);
		ygos_sem_post(&sem_mavlink2);
		task1_flag++;
	}
}

//任务2入口函数
void task2_entry(void *arg)
{
	while(1){
		ygos_sem_wait(&sem_mavlink2, 100);
		task2_flag++;
	}
}

void main(void)
{   
	bsp_init();
	
    ygos_sem_init(&sem_mavlink1, 0);
	ygos_sem_init(&sem_mavlink2, 0);

	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	ygos_init();
	
    //创建用户任务
	ygos_tcb_create(0, task1_entry, (void*)0, &task1_stack[TASK1_STATCK_SIZE-1]);
	ygos_tcb_create(1, task2_entry, (void*)0, &task2_stack[TASK2_STATCK_SIZE-1]);
	
	//操作系统启动，会触发任务调度到最高优先级的任务
	ygos_start();
  
	// //ygos启动失败,会执行下面
	while(1) {
		//return -1;
	}
}



