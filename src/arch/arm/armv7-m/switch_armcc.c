#include <ygos/rtos.h>

//PendSV中断控制器地址
#define  NVIC_INT_CTRL      0xE000Ed04 
//触发PendSV
#define  NVIC_PENDSV_SET    0x10000000 
//PendSV优先级控制地址
#define  NVIC_SYSPRI2       0xE000Ed22 
//PendSV设置为最低优先值255
#define  NVIC_PENDSV_PRI    0x000000ff 

//关闭全局中断
__asm uint32_t  ygos_interrupt_disable(void)
 {
    MRS     R0, PRIMASK
    CPSID   I
    BX      LR
    NOP
 }

 //使能全局中断
__asm void  ygos_interrupt_enable(int level)
 {
    MSR     PRIMASK, R0
    BX      LR
    NOP
 }

//PendSV_Handler异常处理
__asm void PendSV_Handler(void)
{
	IMPORT ygos_tcb_current
	IMPORT ygos_prio_current
	IMPORT ygos_prio_hig_ready
	IMPORT ygos_tcb_current
	IMPORT ygos_tcb_high_ready
	IMPORT ygos_os_swtich_flag
	
	//关闭中断
	CPSID   I
	
    //PSP => RO	
    //MRS     R0, PSP 
    //初始化ygos的时候会初始化PSP为0	
    LDR     R0, = ygos_os_swtich_flag
    LDR     R1, [R0] 
    CMP     R1, #1	
    BEQ     OS_CPU_PendSVHandler_nosave 

    MRS     R0, PSP
    
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
    LDR     R0, =ygos_os_swtich_flag                                     
    MOVS    R1, #0
    STR     R1, [R0]
		
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
	IMPORT ygos_os_swtich_flag
	
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

    LDR     R0, =ygos_os_swtich_flag                                     
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

__asm void os_task_switch_to(void)
{
    IMPORT ygos_os_swtich_flag

    //ygos_os_swtich_flag=1
    LDR     R0, =ygos_os_swtich_flag                                     
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


