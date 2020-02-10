
#include <rtos.h>
#include "systick.h"

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

//任务1入口函数
void task1_entry(void *arg)
{   
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
//#include <ygos/list.h>
//#include <stdio.h>

////定义1个数据类型
//typedef struct node {
//    int value;
//    struct list_head list;
//}node_t;



////静态定义1个头节点
//static LIST_HEAD(head);  //已经有内存空间,空指针

////定义3个全局变量
//node_t node[3] = {
//    {1, {NULL, NULL} },
//    {2, {NULL, NULL} },
//    {3, {NULL, NULL} }
//};

//void print_all_node(void)
//{
//     //判断节点是否是空
//    if (!list_empty(&head)) {
//        //遍历所有节点
//        node_t *sp;
//        list_for_each_entry(sp, &head, node_t, list){
//            //DEBUG("sp:%x, value:%d\n", sp, sp->value);
//        }
//    } 
//}
//int main(int argc, char **argv)
//{   
//    //添加3个节点
//    list_add_tail(&node[0].list, &head);
//    list_add_tail(&node[1].list, &head);
//    list_add_tail(&node[2].list, &head);

//    print_all_node();

//    //删除尾节点
//    //DEBUG("delete tail(node[2])\n");
//    list_del(head.prev);
//    print_all_node();

//    //DEBUG("delete tail(node[1])\n");
//    list_del(head.prev);
//    print_all_node();

//    //DEBUG("delete tail(node[0])\n");
//    list_del(head.prev);
//    print_all_node();


//    return 0;
//}   
