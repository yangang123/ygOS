
#include <ygos/rtos.h>

//当前执行的任务
struct tcb_s *ygos_tcb_current; 

//最高优先级的就绪任务                         
struct tcb_s *ygos_tcb_high_ready;  

//空闲的任务链表    
struct list_head ygos_tcb_free_list; 

//激活任务链表
struct list_head ygos_tcb_list;

//任务内存空间
struct tcb_s tcb_table[TASK_NUM_MAX];

//保存任务的内存地址
struct tcb_s *tcb_prio_table[TASK_NUM_TOTAL_NUM];

//就就绪表的最高优先级
uint8_t ygos_prio_hig_ready;

//当前执行的就绪任务的优先级
uint8_t ygos_prio_current; 

//中断嵌套级别
volatile int ygos_interrupt_nest;

//指示os是否启动,启动后ysos_os_runing=1
uint8_t ygos_os_runing = 0;

//所有任务就绪状态
 uint32_t task_ready_value = 0;

//系统时钟ticks
 uint32_t ygos_tick = 0;


//初始化TCB的空闲链表空间
static void ygos_tcb_list_init(void)
{   
	//初始化空闲链表头节点
	INIT_LIST_HEAD(&ygos_tcb_free_list);
	
	//把所有内存节点链表节点写入添加到空闲链表中
	for (uint8_t i =0; i < TASK_NUM_MAX; i++) {
		list_add_tail(&tcb_table[i].list, &ygos_tcb_free_list); 
	}
 
	//TCB任务指针初始化为空
	INIT_LIST_HEAD(&ygos_tcb_list);

    ygos_tcb_current = list_get_first(&ygos_tcb_free_list);

}

// //ygos获取系统tick
uint32_t ygos_get_tick(void)
{  
	int ticks = 100;

	return ticks;
}



//ygos初始化
void  ygos_init (void)
{
    if (!ygos_os_runing) {
		//ygos的TCB链表初始化
		ygos_tcb_list_init();

	}
}
