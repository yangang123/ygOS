#include <ygos/rtos.h>


void ygos_task_exit(void)
{
  
}

//任务堆栈初始胡xPSP, PC, LR, R......
uint32_t *ygos_task_stack_init(void (*task)(void *p_arg), void *p_arg, uint32_t *ptos, int16_t opt)
{
	uint32_t *stk;
    
	//stk指向栈顶
	stk 	 = (uint32_t*)ptos;
   
    *( stk ) = ( uint32_t ) ygos_task_exit;   	/* R31 (LP) */
    *( --stk ) = ( uint32_t ) p_arg;            /* R6       */
    *( --stk ) = ( uint32_t ) 0x07070707;       /* R7       */
    *( --stk ) = ( uint32_t ) 0x08080808;       /* R8       */
    *( --stk ) = ( uint32_t ) 0x09090909;       /* R9       */
    *( --stk ) = ( uint32_t ) 0x10101010;       /* R10      */
    *( --stk ) = ( uint32_t ) 0x11111111;       /* R11      */
    *( --stk ) = ( uint32_t ) 0x12121212;       /* R12      */
    *( --stk ) = ( uint32_t ) 0x13131313;       /* R13      */
    *( --stk ) = ( uint32_t ) 0x14141414;       /* R14      */
    *( --stk ) = ( uint32_t ) 0x15151515;       /* R15      */
    *( --stk ) = ( uint32_t ) 0x16161616;       /* R16      */
    *( --stk ) = ( uint32_t ) 0x17171717;       /* R17      */
    *( --stk ) = ( uint32_t ) 0x18181818;       /* R18      */
    *( --stk ) = ( uint32_t ) 0x19191919;       /* R19      */
    *( --stk ) = ( uint32_t ) 0x20202020;       /* R20      */
    *( --stk ) = ( uint32_t ) 0x21212121;       /* R21      */
    *( --stk ) = ( uint32_t ) 0x22222222;       /* R22      */
    *( --stk ) = ( uint32_t ) 0x23232323;       /* R23      */
    *( --stk ) = ( uint32_t ) 0x24242424;       /* R24      */
    *( --stk ) = ( uint32_t ) 0x25252525;       /* R25      */
    *( --stk ) = ( uint32_t ) 0x26262626;       /* R26      */
    *( --stk ) = ( uint32_t ) 0x27272727;       /* R27      */
    *( --stk ) = ( uint32_t ) 0x28282828;       /* R28      */
    *( --stk ) = ( uint32_t ) 0x29292929;       /* R29      */
    *( --stk ) = ( uint32_t ) 0x30303030;       /* R30 (EP) */
    *( --stk ) = ( uint32_t ) 0x01010101;       /* R1       */
    *( --stk ) = ( uint32_t ) 0x02020202;       /* R2       */
    *( --stk ) = ( uint32_t ) 0x00010000;  	/* EIPSW    */
    *( --stk ) = ( uint32_t ) task;           	/* EIPC     */

	return (uint32_t *)stk;
}

void ygos_runing_start(void)
{
	ygos_os_runing  = 1;
}

void ygos_task_context_switch(void)
{
	ygos_prio_current = ygos_prio_hig_ready;
	ygos_tcb_current  = ygos_tcb_high_ready;
}

int ygos_interrupt_disable(void)
{       
	int value = 0;
		
	value = __stsr(5);
	__DI();
	
	return value;
}

void ygos_interrupt_enable(int value)
{
	__ldsr(5, value);
}