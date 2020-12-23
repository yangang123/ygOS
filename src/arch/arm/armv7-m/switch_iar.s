;PendSV中断控制器地址
NVIC_INT_CTRL       EQU   	0xE000Ed04 

;触发PendSV
NVIC_PENDSV_SET     EQU  	0x10000000 

;PendSV优先级控制地址
NVIC_SYSPRI2        EQU   	0xE000Ed22 

NVIC_PENDSV_PRI  	EQU  	0x000000ff 


	RSEG    CODE:CODE(2)
	thumb

	IMPORT ygos_tcb_current
	IMPORT ygos_prio_current
	IMPORT ygos_prio_hig_ready
	IMPORT ygos_tcb_high_ready
	IMPORT ygos_os_runing
	IMPORT ygos_os_swtich_flag

	EXPORT ygos_interrupt_disable
ygos_interrupt_disable:
	MRS     R0, PRIMASK
	CPSID   I
	BX      LR

	EXPORT ygos_interrupt_enable
ygos_interrupt_enable:
	MSR     PRIMASK, R0
	BX      LR


	EXPORT PendSV_Handler
PendSV_Handler:
	;关闭中断
	CPSID   I

	LDR     R0, = ygos_os_swtich_flag
	LDR     R1, [R0] 
	CMP     R1, #1	
	BEQ     OS_CPU_PendSVHandler_nosave       

	;PSP => RO	
	MRS     R0, PSP

	;通过寄存器R4-R11，一共是8个字的寄存器，占用空间是4*8=32byte
	SUBS    R0, R0, #0x20   
	;保存寄存器R4-R11到PSP的地址	
	STM     R0, {R4-R11}

	;加载ygos_tcb_current指针到R1
	LDR     R1, =ygos_tcb_current 
	
	LDR     R1, [R1]
        
	STR     R0, [R1]                                          

OS_CPU_PendSVHandler_nosave:
    LDR     R0, =ygos_os_swtich_flag                                     
    MOVS    R1, #0
    STR    R1, [R0]

	;ygos_prio_current = ygos_prio_hig_ready
    LDR     R0, =ygos_prio_current                                     
    LDR     R1, =ygos_prio_hig_ready
    LDRB    R2, [R1]
    STRB    R2, [R0]

	;ygos_tcb_current = ygos_tcb_high_ready
    LDR     R0, =ygos_tcb_current                                       
    LDR     R1, =ygos_tcb_high_ready
    LDR     R2, [R1]
    STR     R2, [R0]
    
    ;R0加载
    LDR     R0, [R2]

    LDM     R0, {R4-R11} 
    ;更新栈地址的指针到R0	
    ADDS    R0, R0, #0x20

    ;更新R0到PSP
    MSR     PSP, R0 
    ;栈地址为用户栈指针PSP
    ORR     LR, LR, #0x04    

    ;打开中断	
    CPSIE   I
    BX      LR  
	
	;防止编译出现警告信息
    NOP	 

	EXPORT ygos_start_high_ready
ygos_start_high_ready:

    LDR     R0, =NVIC_SYSPRI2                                 
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]
    
	;PSP = 0
    MOVS    R0, #0                                             
    MSR     PSP, R0
	
	;ygos_os_runing =1
	LDR     R0, =ygos_os_runing                                     
    MOVS    R1, #1
    STRB    R1, [R0]
    
    LDR     R0, =ygos_os_swtich_flag                                     
    MOVS    R1, #1
    STR     R1, [R0]
    
	
	;触发pendsv异常
	LDR     R0, =NVIC_INT_CTRL                                  
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0]
	   
	BX LR
	
	;防止编译出现警告信息
	NOP

	EXPORT os_task_switch
os_task_switch:
	;触发pensv异常
    LDR     R0, =NVIC_INT_CTRL                              
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0] 
    BX      LR
	
	;防止编译出现警告信息
	NOP

	EXPORT os_task_switch_to
os_task_switch_to:

    LDR     R0, =ygos_os_swtich_flag                                     
    MOVS    R1, #1
    STR     R1, [R0]

	;触发pensv异常
    LDR     R0, =NVIC_INT_CTRL                              
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0] 
    BX      LR
        END
