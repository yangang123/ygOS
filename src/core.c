
#include "rtos.h"

struct tcb_s *ygos_tcb_current;                          
struct tcb_s *ygos_tcb_high_ready;      
struct tcb_s *ygos_tcb_free_list; 
struct tcb_s *ygos_tcb_list;                    
struct tcb_s tcb_table[TASK_NUM_MAX];
struct tcb_s *tcb_prio_table[TASK_NUM_TOTAL_NUM];
static uint32_t task_ready_value = 0;
static uint32_t ygos_tick = 0;

uint8_t ygos_prio_hig_ready;
uint8_t ygos_prio_current; 
volatile int ygos_interrupt_nest;
uint8_t ygos_os_runing = 0;

//任务堆栈初始胡xPSP, PC, LR, R......
static uint32_t *ygos_task_stack_init (void (*task)(void *p_arg), void *p_arg, uint32_t *ptos, int16_t opt)
{
    uint32_t *stk;
	
    (void)opt;                                   
    stk       = ptos;                           
	
    *(--stk)    = (uint32_t)0x01000000uL;            
    *(--stk)  = (uint32_t)task;                    
    *(--stk)  = (uint32_t)0;           /* R14 (LR)                                           */
    *(--stk)  = (uint32_t)0x12121212uL;            
    *(--stk)  = (uint32_t)0x03030303uL;           
    *(--stk)  = (uint32_t)0x02020202uL;           
    *(--stk)  = (uint32_t)0x01010101uL;           
    *(--stk)  = (uint32_t)p_arg;                  

                                                
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

// 1. 创建任务到任务链表中
void  ygos_tcb_create (int prio, void (*task)(void *p_arg), void *p_arg, uint32_t  *ptos)                 
{  
    struct tcb_s    *ptcb;
	uint32_t *stk;
    int level;
	
	ygos_interrupt_disable();
    ptcb = ygos_tcb_free_list;
	if (ptcb != (struct tcb_s *)0) {
        ygos_tcb_free_list            = ptcb->next;   
		ptcb->prio = prio;		
		stk = ygos_task_stack_init(task, 0, ptos, 0);
		ptcb->stack_ptr = stk;	

		tcb_prio_table[prio] = ptcb;
		//当前tcb插入tcb任务链表的头部
		ptcb->next = ygos_tcb_list;
		ptcb->prev = (struct tcb_s *)0;
		if(ygos_tcb_list != (struct tcb_s *)0) {
			ygos_tcb_list->prev  = ptcb;
		}
		ygos_tcb_list = ptcb;

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
	ygos_tcb_free_list = &tcb_table[0];
	ygos_tcb_list      = (struct tcb_s *)0;
}


void ygos_task_ready_add(int prio)
{
	task_ready_value |= 1 << prio;
}

int ygos_task_high_ready_get(void)
{
	for (uint8_t i =0; i < 32; i++){
		if (task_ready_value & (1<<i)) {
			return i;	
		}	
	}
	
	return -1;
}

void ygos_task_ready_delete(int prio)
{
	task_ready_value &= ~(1 << prio);
}
	
void ygos_interrupt_enter(void)
{
    int level;

    ygos_interrupt_disable();
    ygos_interrupt_nest ++;
    ygos_interrupt_enable(level);
}

//中断退出，任务切换一次
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

void  ygos_sche_new (void)
{   
	//更新任务优先级
	int prio = 0;
	prio = ygos_task_high_ready_get();
	if (prio >= 0) {
		ygos_prio_hig_ready = prio;
	}	
}

void  ygos_sche (void)
{   
	int  level; 
	ygos_interrupt_disable();
    if (ygos_interrupt_nest == 0u) {
		ygos_sche_new();
		//更新tcb
		ygos_tcb_high_ready  = tcb_prio_table[ygos_prio_hig_ready];
 		if (ygos_prio_hig_ready != ygos_prio_current) { 
			 os_task_switch();
		 }
	}
	ygos_interrupt_enable(level);
}

void ygos_timer_sche(void)
{
    struct tcb_s    *ptcb;
    int level;
	
	ygos_tick++;
	
	ptcb = ygos_tcb_list;                                  
	while (ptcb->prio != IDLE_TASK_PRIO) {     
		ygos_interrupt_disable();
		if (ptcb->sleep_tick != 0u) {                    
			ptcb->sleep_tick--;                         
			if (ptcb->sleep_tick == 0u) {
				ygos_task_ready_add(ptcb->prio);                
				//获取最高优先级任务
                ygos_sche_new();
			}
		}
		ptcb = ptcb->next;                        
		ygos_interrupt_enable(level);
	}
}



//ygos启动
void  ygos_start (void)
{
    if (!ygos_os_runing) {
		//任务未初始化,不需要临界保护
	    ygos_sche_new();
        ygos_prio_current     = ygos_prio_hig_ready;
        ygos_tcb_high_ready  = tcb_prio_table[ygos_prio_hig_ready];
		ygos_tcb_current     = ygos_tcb_high_ready;
		ygos_start_high_ready();
	}
}

//ygos初始化
void  ygos_init (void)
{
    if (!ygos_os_runing) {
		ygos_tcb_list_init();
		idle_task_init();
	}
}
