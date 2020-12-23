
#include <ygos/rtos.h>

#define PRIO_VALID(prio) (prio < TASK_NUM_TOTAL_NUM)

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

 uint32_t ygos_os_swtich_flag = 0;


//hash表记录优先级是0~255，第一个1的位置，也就是优先级位置
const uint8_t bitmap[] =
{
    /* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    /* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

// 创建任务到任务链表中
void  ygos_tcb_create (int prio, void (*task)(void *p_arg), void *p_arg, uint32_t  *ptos)                 
{  
	struct tcb_s    *ptcb;
	uint32_t *stk;
	int level;

	level = ygos_interrupt_disable();

	struct list_head *first = list_get_first(&ygos_tcb_free_list);
	ptcb  = list_entry(first, struct tcb_s, list);

	if (ptcb != (struct tcb_s *)0) {
		//空闲任务指针指向链表的下一个节点

		//当前任务优先级  
		ptcb->prio = prio;		
		//初始化当前任务的堆栈
		stk = ygos_task_stack_init(task, 0, ptos, 0);
		//更新当前任务堆栈指针到当TCb中
		ptcb->stack_ptr = stk;	
		//把当前任务的指针放到TCB的顺序表中
		tcb_prio_table[prio] = ptcb;

		//删除ygos_tcb_free_list的第一个节点的链表
		list_del_first(&ygos_tcb_free_list);

		//把当前节点添加到激活链表中
		list_add_tail(first, &ygos_tcb_list); 
#ifdef YGOS_SIGNAL_ENABLE
		//初始化信号
		INIT_LIST_HEAD(&ptcb->signal_list);
		INIT_LIST_HEAD(&ptcb->signal_pending_list);
#endif 
		//当前任务初始化为就绪状态
		ygos_task_ready_add(prio);
	}
	ygos_interrupt_enable(level);

}

//获取任务TCB
struct tcb_s *  ygos_tcb_get (int prio)                 
{    
	if (PRIO_VALID(prio)) {
			return tcb_prio_table[prio]; 
	}
	
	return NULL;
}

//获取当前任务TCB
struct tcb_s *  ygos_tcb_self (void)                 
{    
	return ygos_tcb_current;
}


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
}

//当前任务从未就绪状态变为就绪状态，prio表示任务的优先级
void ygos_task_ready_add(int prio)
{
	task_ready_value |= 1 << prio;
}

//查找任务就绪列表中最高优先级的任务，时间复杂度是1
static int ygos_task_high_ready_get(void)
{   
	//优先级值的数值越低，任务优先级越高
	if (task_ready_value  & 0xff) {
		return bitmap[task_ready_value  & 0x00ff ];
   	}
     
	if (task_ready_value & 0xff00 ) {
		return bitmap[ (task_ready_value & 0xff00) >> 8 ] + 8 ;
	}

	if ((task_ready_value & 0xff0000) ) {
		return bitmap[(task_ready_value  & 0xff0000) >> 16 ] + 16;
	}

	if ((task_ready_value & 0xff000000) ) {
		return bitmap[(task_ready_value  & 0xff000000) >> 24 ] + 24 ;
	}
    
	//应该不会跑到这里，空闲任务一直处于就绪状态
	return -1;
}

//当前任务从就绪状态变为未就绪状态，prio表示任务的优先级
void ygos_task_ready_delete(int prio)
{
	task_ready_value &= ~(1 << prio);
}

//进入中断服务程序，此函数被调用，更新中断嵌套的级别
void ygos_interrupt_enter(void)
{   
	if (!ygos_os_runing){
		return;
	}
	int level;
    
	//ygos支持中断嵌套,
    level = ygos_interrupt_disable();
    ygos_interrupt_nest++;
    ygos_interrupt_enable(level);
}

//中断退出，任务切换一次，和ygos_interrupt_enter进行配对使用
void ygos_interrupt_leave(void)
{   
	if (!ygos_os_runing){
		return;
	}

	int  level;

	level = ygos_interrupt_disable();
	ygos_interrupt_nest --;
	if (ygos_interrupt_nest == 0u) {
		//任务切换
#ifndef  RH850_CHIP
		ygos_sche();
#else 
		ygos_sche_new();		
		ygos_tcb_high_ready  = tcb_prio_table[ygos_prio_hig_ready];
#endif 
	}
	ygos_interrupt_enable(level);
}

//刷新就绪列表中最高优先级的任务
void  ygos_sche_new (void)
{   
	//更新任务优先级
	int prio = 0;
	//通过优先级算法，获取到最高优先级的任务
	prio = ygos_task_high_ready_get();
	if (prio >= 0) {
		//更新最高优先级的任务
		ygos_prio_hig_ready = prio;
	}	
}

//从就绪列表中找到最高优先级的就绪任务,在退出所有嵌套中断后就行任务切换
void  ygos_sche (void)
{   
	int  level; 
	level = ygos_interrupt_disable();
	//中断服务程序中，不允许任务切换
    if (ygos_interrupt_nest == 0u) {
		//查找最高优先级的任务
		ygos_sche_new();
		//通过优先级更新TCB
		ygos_tcb_high_ready  = tcb_prio_table[ygos_prio_hig_ready];
		//最高优先级就绪任务和当前任务不一样，则更新更新最高优先级任务
		if (ygos_prio_hig_ready != ygos_prio_current) { 
			os_task_switch();
		}
	}
	ygos_interrupt_enable(level);
}

//定时器周期查询就绪任务队列中的最高优先级是否发生变化
void ygos_timer_sche(void)
{   
    struct tcb_s    *ptcb;
    int level;
	
	//更新系统tick
	level = ygos_interrupt_disable();
	ygos_tick++;
	ygos_interrupt_enable(level);
    
	if (!ygos_os_runing){
		return;
	}

	//从tcb list的指针ygos_tcb_list的头部进行遍历所有的激活的任务
	list_for_each_entry(ptcb, &ygos_tcb_list, struct tcb_s, list) {
		if (ptcb->prio != IDLE_TASK_PRIO) {
			level = ygos_interrupt_disable();
			if (ptcb->sleep_tick != 0u) {                    
				ptcb->sleep_tick--;                         
				if (ptcb->sleep_tick == 0u) {
					
					//任务处于等待资源状态
					if (ptcb->status != TASK_READY_RUN) {

						//任务状态修改成运行态
						ptcb->status = TASK_READY_RUN;
						
						ptcb->wait_status = WAIT_TIMEOUT;
					} else {
						ptcb->wait_status = WAIT_OK;
					}
					
					//ptcb指向的任务的状态从未就绪修改成就绪
					ygos_task_ready_add(ptcb->prio);                
					//在就绪列表中获取最高优先级任务
					ygos_sche_new();
				}
			}                  
			ygos_interrupt_enable(level);
		}
	}                                 
}

//ygos获取系统tick
uint32_t ygos_get_tick(void)
{  
	int ticks;
	int level;
    
	//临界区保护,防止线程获取tick的时候，被中断打断
	level = ygos_interrupt_disable();
	ticks = ygos_tick;
	ygos_interrupt_enable(level);
	
	return ticks;
}

//ygos启动
void  ygos_start (void)
{
    if (!ygos_os_runing) {
		//从就绪列表中找到最高优先级的任务
		ygos_sche_new();
		//ygos_prio_hig_ready最高优先级赋值给ygos_prio_current
		ygos_prio_current     = ygos_prio_hig_ready;
		//通过ygos_prio_hig_ready获取顺序表中TCB指针
		ygos_tcb_high_ready  = tcb_prio_table[ygos_prio_hig_ready];
		//最高优先级任务的TCB赋值给当前运行任务的TCB指针
		ygos_tcb_current     = ygos_tcb_high_ready;

		//第一次，触发pensv异常，os从这里就跑起来了。
		ygos_start_high_ready();
	}
}

//ygos初始化
void  ygos_init (void)
{
    if (!ygos_os_runing) {
		//ygos的TCB链表初始化
		ygos_tcb_list_init();

		//空闲任务初始化,空闲任务一直处于就绪状态，优先级最低
		ygos_idle_task_init();

#ifdef YGOS_SIGNAL_ENABLE       
		//初始化信号的内存空间
		ygos_signal_list_init();
#endif
	}
}
