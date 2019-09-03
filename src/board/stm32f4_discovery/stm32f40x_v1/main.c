#include "board_config.h"
#include "bsp_led.h"
#include "timer.h"

#include "rtos.h"

//任务1堆栈
#define TASK1_STATCK_SIZE  128 

//任务2堆栈
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
	//LED慢闪
	while(1){
	    LED1_PIN_ON();
		ygos_sleep_tick(1000);	
		LED1_PIN_OFF();	
		ygos_sleep_tick(1000);
	}
}

//任务2入口函数
void task2_entry(void *arg)
{   
	//LED快闪
	while(1){
		task2_flag++;
		LED3_PIN_ON();
		ygos_sleep_tick(200);
		LED3_PIN_OFF();
		ygos_sleep_tick(200);
	}
}

int main(void)
{  
	//定时器和GPIO初始化
	system_tim2_init();
	bsp_led_gpio_init();
	
    //操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	ygos_init();
	
    //创建用户任务
	ygos_tcb_create(0, task1_entry, (void*)0, &task1_stack[TASK1_STATCK_SIZE]);
	ygos_tcb_create(1, task2_entry, (void*)0, &task2_stack[TASK2_STATCK_SIZE]);
	
	//操作系统启动，会触发任务调度到最高优先级的任务
	ygos_start();
	
	//ygos启动失败,会执行下面
	while(1) {
		//return -1;
	}
}
