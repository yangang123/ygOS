
#include "rtos.h"

//当前执行的任务
struct tcb_s *ygos_tcb_current; 

//最高优先级的就绪任务                         
struct tcb_s *ygos_tcb_high_ready;  

//空闲的任务链表    
struct tcb_s *ygos_tcb_free_list; 

//激活任务链表
struct tcb_s *ygos_tcb_list;       

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
static uint32_t task_ready_value = 0;

//系统时钟ticks
static uint32_t ygos_tick = 0;


//触发任务切换
extern __asm void os_task_switch(void);

//第一次触发任务启动，在系统启动的时候调用
extern __asm void ygos_start_high_ready(void);


//任务堆栈初始胡xPSP, PC, LR, R......
static uint32_t *ygos_task_stack_init (void (*task)(void *p_arg), void *p_arg, uint32_t *ptos, int16_t opt)
{
    uint32_t *stk;
	
    (void)opt; 

	//stk指向栈顶                                  
    stk       = ptos;                           
	
	//初始化xPSP, PC, LR等寄存器
    *(--stk)    = (uint32_t)0x01000000uL;            
    *(--stk)  = (uint32_t)task;                    
    *(--stk)  = (uint32_t)0;           /* R14 (LR)                                           */
    *(--stk)  = (uint32_t)0x12121212uL;            
    *(--stk)  = (uint32_t)0x03030303uL;           
    *(--stk)  = (uint32_t)0x02020202uL;           
    *(--stk)  = (uint32_t)0x01010101uL;           
    *(--stk)  = (uint32_t)p_arg;                  
   
    //初始化R11-R4                                             
    *(--stk)  = (uint32_t)0x11111111uL;            
    *(--stk)  = (uint32_t)0x10101010uL;            
    *(--stk)  = (uint32_t)0x09090909uL;            
    *(--stk)  = (uint32_t)0x08080808uL;           
    *(--stk)  = (uint32_t)0x07070707uL;            
    *(--stk)  = (uint32_t)0x06060606uL;           
    *(--stk)  = (uint32_t)0x05050505uL;           
    *(--stk)  = (uint32_t)0x04040404uL;           

    return (stk);
}

// 创建任务到任务链表中
void  ygos_tcb_create (int prio, void (*task)(void *p_arg), void *p_arg, uint32_t  *ptos)                 
{  
    struct tcb_s    *ptcb;
	uint32_t *stk;
    int level;
	
	ygos_interrupt_disable();
    ptcb = ygos_tcb_free_list;
	if (ptcb != (struct tcb_s *)0) {
		//空闲任务指针指向链表的下一个节点
        ygos_tcb_free_list            = ptcb->next; 
		//当前任务优先级  
		ptcb->prio = prio;		
        //初始化当前任务的堆栈
		stk = ygos_task_stack_init(task, 0, ptos, 0);
		//更新当前任务堆栈指针到当TCb中
		ptcb->stack_ptr = stk;	
        //把当前任务的指针放到TCB的顺序表中
		tcb_prio_table[prio] = ptcb;
		//当前tcb插入tcb任务链表的头部
		ptcb->next = ygos_tcb_list;
		ptcb->prev = (struct tcb_s *)0;
		if(ygos_tcb_list != (struct tcb_s *)0) {
			ygos_tcb_list->prev  = ptcb;
		}
		ygos_tcb_list = ptcb;
        //当前任务初始化为就绪状态
		ygos_task_ready_add(prio);
	}
	ygos_interrupt_enable(level);

}

//初始化TCB的空闲链表空间
static void ygos_tcb_list_init(void)
{
	for (uint8_t i =0; i < TASK_NUM_MAX -1; i++) {
		tcb_table[i].next = &tcb_table[i+1];
	}
    //TCB空闲链表指针指向第一块TCB
	ygos_tcb_free_list = &tcb_table[0];
    //TCB任务指针初始化为空
	ygos_tcb_list      = (struct tcb_s *)0;
}

//当前任务从未就绪状态变为就绪状态，prio表示任务的优先级
void ygos_task_ready_add(int prio)
{
	task_ready_value |= 1 << prio;
}

//查找任务就绪列表中最高优先级的任务
static int ygos_task_high_ready_get(void)
{
	for (uint8_t i =0; i < 32; i++){
		if (task_ready_value & (1<<i)) {
			return i;	
		}	
	}
	
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
    int level;
    
	//ygos支持中断嵌套,
    ygos_interrupt_disable();
    ygos_interrupt_nest++;
    ygos_interrupt_enable(level);
}

//中断退出，任务切换一次，和ygos_interrupt_enter进行配对使用
void ygos_interrupt_leave(void)
{
    int  level;

	ygos_interrupt_disable();
	ygos_interrupt_nest --;
	if (ygos_interrupt_nest == 0u) {
		//任务切换
		ygos_sche();
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
	ygos_interrupt_disable();
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
	ygos_interrupt_disable();
	ygos_tick++;
	ygos_interrupt_enable(level);
    
	//从tcb list的指针ygos_tcb_list的头部进行遍历所有的激活的任务
	ptcb = ygos_tcb_list;                                  
	while (ptcb->prio != IDLE_TASK_PRIO) {     
		ygos_interrupt_disable();
		if (ptcb->sleep_tick != 0u) {                    
			ptcb->sleep_tick--;                         
			if (ptcb->sleep_tick == 0u) {
				//ptcb指向的任务的状态从未就绪修改成就绪
				ygos_task_ready_add(ptcb->prio);                
				//在就绪列表中获取最高优先级任务
                ygos_sche_new();
			}
		}
		//指向下一个任务
		ptcb = ptcb->next;                        
		ygos_interrupt_enable(level);
	}
}

//ygos获取系统tick
uint32_t ygos_get_tick(void)
{  
	int ticks;
	int level;
    
	//临界区保护,防止线程获取tick的时候，被中断打断
	ygos_interrupt_disable();
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
	}
}
