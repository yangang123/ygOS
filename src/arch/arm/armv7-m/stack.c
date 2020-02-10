
#include <ygos/rtos.h>

//任务堆栈初始胡xPSP, PC, LR, R......
uint32_t *ygos_task_stack_init (void (*task)(void *p_arg), void *p_arg, uint32_t *ptos, int16_t opt)
{
    uint32_t *stk;
	
    (void)opt; 

	//stk指向栈顶                                  
	stk       = ptos;                           
	
	//初始化xPSP, PC, LR等寄存器
    *(stk)    = (uint32_t)0x01000000uL;            
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
