

.extern ygos_prio_current
.extern ygos_prio_hig_ready
.extern ygos_tcb_current
.extern ygos_tcb_high_ready
.extern ygos_os_runing

.section .text, "ax"

@   
@   int ygos_interrupt_disable()
@    
@   return: r0 作为返回值

.globl ygos_interrupt_disable
ygos_interrupt_disable:
    mrs r0, cpsr
    cpsid i
    bx  lr

@   
@   ygos_interrupt_enable(int level)
@    
@   r0 == level: level传递参数到r0, 为了防止关闭中断嵌套,保证最后
@   打开中断的时候, 进行打开中断, 所以在第一次关闭中断前的CPSR寄存器
@   是打开中断的值, 比如打开中断是1, 关闭中断0, 最后一次把1恢复到CPSR
@   才是正确的

.globl ygos_interrupt_enable
ygos_interrupt_enable:
    msr cpsr, r0
    bx  lr

@   
@   void ygos_start_high_ready(void)
@    
.globl ygos_start_high_ready
ygos_start_high_ready:

    @ ygos_os_runing = 1 
    ldr     R0, =ygos_os_runing                                     
    movs    R1, #1
    strb    R1, [R0]
    bl context_switch

@   
@   void os_task_switch(void)
@   注意 : 1) 加载ygos_tcb_high_ready的值,相当于加载了1个地址 &ygos_tcb_high_ready->sp
@   "   ldr     R4, =ygos_tcb_high_ready                                     
@       ldr r1, [r4] "
@
@          2) 再加载一次,相当于结构指针的成员的值 ygos_tcb_high_ready->sp
@    "   ldr sp, [r1] "       
@  
.section .text.isr, "ax"
.globl os_task_switch
os_task_switch:

    @ 压栈lr代替压入pc指针
    stmfd   sp!, {lr}      
    stmfd   sp!, {r0-r12, lr}  

    @ CPSR的T=0：arm，T=1：thumb
    mrs r4, cpsr
    tst lr, #0x01
    orrne r4, r4, #0x20     
    stmfd sp!, {r4}         

    ldr     R3, =ygos_tcb_current                                     
    ldr r0, [r3]            
    str sp, [r0]           
    
    @ 更新ygos_prio_current = ygos_prio_hig_ready
    ldr     R0, =ygos_prio_current                                     
    ldr     R1, =ygos_prio_hig_ready
    ldrb    R2, [R1]
    strb    R2, [R0]

    @ 更新ygos_tcb_current = ygos_tcb_high_ready
    ldr     R0, =ygos_tcb_current                                       
    ldr     R1, =ygos_tcb_high_ready
    ldr     R2, [R1]
    STR     R2, [R0]
 
 context_switch:
    ldr     R4, =ygos_tcb_high_ready                                     
    ldr r1, [r4]
    ldr sp, [r1]           
    ldmfd sp!, {r4}         
    msr spsr_cxsf, r4
    ldmfd sp!, {r0-r12, lr, pc}^ 
