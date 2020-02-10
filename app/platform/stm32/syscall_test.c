
#include <rtos.h>
#include "systick.h"
#include "ramlog.h"
#include <ygos/libc/stdio.h>
#include <ygos/fs/fs.h>

#define TASK1_STATCK_SIZE  128 
#define TASK2_STATCK_SIZE  128 
#define TASK3_STATCK_SIZE  128 

//任务1和任务2的堆栈
uint32_t  task1_stack[TASK1_STATCK_SIZE];
uint32_t  task2_stack[TASK2_STATCK_SIZE];
uint32_t  task3_stack[TASK3_STATCK_SIZE];

//任务
int task1_flag = 0;
int task2_flag = 0;

//信号量测试
sem_t sem_mavlink;
char buf1[24] = {0x00};
char buf1_len = 0x00;

//任务1入口函数
void task1_entry(void *arg)
{   
//	int fd = open("/dev/ramlog", 0);
//    if (fd >= 0) {
//        char *p = "abc";
//        write(fd, p, strlen(p));
//        read(fd, buf, 24);
//    }
	
	if (!mount(NULL, "/proc", "procfs", 0, NULL)) {
        int fd = open("/proc/uptime", 0x01);
		if (fd < 0) {
			//printf("open error\n");
			//return 0;
		} else {
			buf1_len = read(fd, buf1, 24);
			//printf("buf:%s\n", buf);
		}
    }
	
	ygos_sem_init(&sem_mavlink, 0);
	while(1){
		task1_flag =1 ;
		//等待时间是1000ms
		ygos_sem_wait(&sem_mavlink, 1000);
		task1_flag =0 ;
	}
}

//任务2入口函数
void task2_entry(void *arg)
{
	while(1){
		task2_flag =1 ;
		ygos_sem_wait(&sem_mavlink, 1000);
	    task2_flag =0 ;
	}
}

//任务2入口函数
void task3_entry(void *arg)
{
	while(1){
		task2_flag =1 ;
		//延时200ms
		ygos_sleep_tick(20);
		//post信号量
		ygos_sem_post(&sem_mavlink);
	    task2_flag =0 ;
	}
}

int main(int argc, char **argv)
{   
	//系统时钟初始化，在系统中断服务程序中任务调度    
	systick_init();
   
    //注册驱动	
	ygos_inode_list_init();
	ramlog_register(); 
	
	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	ygos_init();

	
    //创建用户任务
	ygos_tcb_create(0, task1_entry, (void*)0, &task1_stack[TASK1_STATCK_SIZE-1]);
	ygos_tcb_create(1, task2_entry, (void*)0, &task2_stack[TASK2_STATCK_SIZE-1]);
	ygos_tcb_create(2, task3_entry, (void*)0, &task3_stack[TASK3_STATCK_SIZE-1]);
	
	//操作系统启动，会触发任务调度到最高优先级的任务
	ygos_start();
	
	//ygos启动失败,会执行下面
	while(1) {
		//return -1;
	}
}

