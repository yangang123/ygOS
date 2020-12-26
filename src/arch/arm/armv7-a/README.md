# cortex-a7 为什么调度的时候部进行使用swi
>在cortex-a7中为了获得操作寄存器的权限,任务执行过程中一直处于svc模式下，而不是user模式下. 所以可以直接调度os_switch_contex进行切换，
>如果task在user模式下运行，那么需要使用swi进行触发异常，获得cpu权限，进行操作寄存器. 所以ygos在任务调度部分,使用的直接调度的方法
```
uint32_t *ygos_task_stack_init(void (*task)(void *p_arg), void *p_arg, uint32_t *ptos, int16_t opt)
		*(--stk) = SVCMODE;					/* arm mode   */
```

# 启动过程

## 1. 在reset入口
先切换svc模式，关闭IRQ和FIQ中断， 设置各个模式下栈地址，为调度c语言函数进行做栈准备，同时清除bss段，清空为0,
最后跳转到main入口函数
```
.globl _reset
_reset:
    /* set the cpu to SVC32 mode and disable interrupt */
    mrs     r0, cpsr
    bic     r0, r0, #0x1f
    orr     r0, r0, #0x13
    msr     cpsr_c, r0
    
    /*初*/
    bl      stack_setup
   
    ldr     pc, _startup

 _startup:
    .word main
```

## 2. 中断向量表设置、
```
.section .vectors, "ax"
.code 32

.globl system_vectors
system_vectors:
    ldr pc, _vector_reset
    ldr pc, _vector_undef
    ldr pc, _vector_swi
    ldr pc, _vector_pabt
    ldr pc, _vector_dabt
    ldr pc, _vector_resv
    ldr pc, _vector_irq
    ldr pc, _vector_fiq
```

#  介绍
这篇文档是解析在armv7-a芯片中，ygos任务调度函数的处理过程的lr的疑问.

Dump of assembler code for function ygos_sche:
## 1. 跳转到ygos_sche中,lr已经压栈
0x60000990 <+0>:	push	{r11, lr}
0x60000994 <+4>:	add	r11, sp, #4
0x60000998 <+8>:	sub	sp, sp, #8

## 2. 跳转到os_task_switch,lr = 0x60000a0c
0x60000a08 <+120>:	bl	0x60001388 <os_task_switch>
0x60000a0c <+124>:	ldr	r0, [r11, #-8]

## 3.在0x60000a0c的位置进行打断点
>这里我们看出lr,pc地址都是0x60000a0c，是错误的，但是下面会执行弹栈，请看第4条
End of assembler dump.
(gdb) info reg
r0             0x1	1
r1             0x60003138	1610625336
r2             0x1	1
r3             0x0	0
r4             0x0	0
r5             0x0	0
r6             0x0	0
r7             0x0	0
r8             0x0	0
r9             0x0	0
r10            0x0	0
r11            0x60003108	1610625288
r12            0x0	0
sp             0x600030fc	0x600030fc <task1_stack+456>
lr             0x60000a0c	1610615308
pc             0x60000a0c	0x60000a0c <ygos_sche+124>
cpsr           0x20000093	536871059

## 4. ygos_sche退出的时候进行弹栈操作
0x60000a1c <+140>:	pop	{r11, pc}

# 建议

1. 任务跳转函数，最好使用汇编函数，c语言跳转到汇编后，整个压栈过程开发可以控制的,如果
   c语言嵌套汇编，可能会将其他寄存器压栈到sp中，影响保存现场

2. 在svi权限下直接任务切换，可能需要判断当前任务是否关闭中断，中断要成对出现的问题