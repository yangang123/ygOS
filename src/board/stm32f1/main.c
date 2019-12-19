/**
  ******************************************************************************
  * @file    SysTick/TimeBase/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************

  */ 

#include <ygos/rtos.h>


#include "stm32f10x.h"
#include "led.h"

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
    gpio_toggle( GPIOE, GPIO_Pin_5);
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
    gpio_toggle( GPIOE, GPIO_Pin_6);
	}
}

int main(int argc, char **argv)
{   
	//系统时钟初始化，在系统中断服务程序中任务调度    
 if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
    init_led();
	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	ygos_init();
	
    //创建用户任务
	ygos_tcb_create(0, task1_entry, (void*)0, &task1_stack[TASK1_STATCK_SIZE]);
	ygos_tcb_create(1, task2_entry, (void*)0, &task2_stack[TASK2_STATCK_SIZE]);
	
	//操作系统启动，会触发任务调度到最高优先级的任务
	ygos_start();
  
	
	// //ygos启动失败,会执行下面
	while(1) {
		//return -1;
	}
}