#include "rtos.h"

//#include "stm32f4xx.h"

//PendSV中断控制器地址
#define  NVIC_INT_CTRL      0xE000Ed04 
//触发PendSV
#define  NVIC_PENDSV_SET    0x10000000 
//PendSV优先级控制地址
#define  NVIC_SYSPRI2       0xE000Ed22 
//PendSV设置为最低优先值255
#define  NVIC_PENDSV_PRI    0x000000ff 


//关闭全局中断
uint32_t  ygos_interrupt_disable(void)
{
//   uint32_t pri_mask = __get_PRIMASK();
//   __disable_irq();
//	
//	return pri_mask;
	return 0;
}

//使能全局中断
void  ygos_interrupt_enable(int level)
{
//    __set_PRIMASK(level);
} 

//PendSV_Handler异常处理
__asm void PendSV_Handler(void)
{
	IMPORT ygos_tcb_current
	IMPORT ygos_prio_current
	IMPORT ygos_prio_hig_ready
	IMPORT ygos_tcb_current
	IMPORT ygos_tcb_high_ready
	
	//关闭中断
	CPSID   I
	
    //PSP => RO	
    MRS     R0, PSP 
    //初始化ygos的时候会初始化PSP为0	
    CBZ     R0, OS_CPU_PendSVHandler_nosave                    
    
	//通过寄存器R4-R11，一共是8个字的寄存器，占用空间是4*8=32byte
    SUBS    R0, R0, #0x20   
    //保存寄存器R4-R11到PSP的地址	
    STM     R0, {R4-R11}
    
	//加载ygos_tcb_current指针到R1
    LDR     R1, =ygos_tcb_current 
     //加载R1的堆栈指针	
    LDR     R1, [R1]
	//存储R0寄存器的值,到堆栈指针的位置
    STR     R0, [R1]                                         

//任务切换
OS_CPU_PendSVHandler_nosave
	//ygos_prio_current = ygos_prio_hig_ready
    LDR     R0, =ygos_prio_current                                     
    LDR     R1, =ygos_prio_hig_ready
    LDRB    R2, [R1]
    STRB    R2, [R0]

	//ygos_tcb_current = ygos_tcb_high_ready
    LDR     R0, =ygos_tcb_current                                       
    LDR     R1, =ygos_tcb_high_ready
    LDR     R2, [R1]
    STR     R2, [R0]
    
	//R0加载
    LDR     R0, [R2]
     //R4-R11的8个寄存器恢复到ygos_tcb_current的堆栈地址	
    LDM     R0, {R4-R11} 
    //更新栈地址的指针到R0	
    ADDS    R0, R0, #0x20
    
	//更新R0到PSP
    MSR     PSP, R0 
     //栈地址为用户栈指针PSP
    ORR     LR, LR, #0x04    
    
    //打开中断	
    CPSIE   I
    BX      LR  
	
	//防止编译出现警告信息
    NOP	
}

//os初始化第一次任务调度
__asm void ygos_start_high_ready(void)
{
	IMPORT ygos_os_runing
	
	//设置中断优先级为最低
	LDR     R0, =NVIC_SYSPRI2                                 
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]
    
	//PSP = 0
    MOVS    R0, #0                                             
    MSR     PSP, R0
	
	//ygos_os_runing =1
	LDR     R0, =ygos_os_runing                                     
    MOVS    R1, #1
    STRB    R1, [R0]
	
	//触发pendsv异常
	LDR     R0, =NVIC_INT_CTRL                                  
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0]
	   
	BX LR
	
	//防止编译出现警告信息
	NOP
}

//触发pendsv完成任务切换
__asm void os_task_switch(void)
{   
	//触发pensv异常
    LDR     R0, =NVIC_INT_CTRL                              
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0] 
    BX      LR
	
	//防止编译出现警告信息
	NOP
}


