#ifndef RTOS_H
#define RTOS_H

#include <stdint.h>

#define TASK_NUM_MAX                      5
#define IDLE_TASK_PRIO                    31
#define TASK_NUM_TOTAL_NUM                32

struct tcb_s
{
	uint32_t 		*stack_ptr;
	struct tcb_s    *next;
	struct tcb_s    *prev;                
	int8_t 			prio;
	uint32_t        sleep_tick;            
	int8_t          stat;        
};

extern struct tcb_s *ygos_tcb_current;                         
extern struct tcb_s *ygos_tcb_high_ready;       
extern struct tcb_s *ygos_tcb_free_list; 

extern uint8_t ygos_prio_hig_ready;
extern uint8_t ygos_prio_current; 
extern volatile int ygos_interrupt_nest;
extern uint8_t ygos_os_runing ;

void ygos_start (void);
void ygos_init(void);
void ygos_tcb_create (int prio, void (*task)(void *p_arg), void *p_arg, uint32_t  *ptos);
void ygos_interrupt_enter(void);
void ygos_interrupt_leave(void);
void ygos_interrupt_disable(void);
void ygos_interrupt_enable(int level);
void ygos_sche (void);
void ygos_sche_new (void);
void ygos_timer_sche(void);
void ygos_sleep_tick (int32_t ticks);
void ygos_task_ready_add(int prio);
void ygos_task_ready_delete(int prio);

#endif 
