# 常用命令

target remote: 1234
> 连接远程

b 
查看当前断点
```
(gdb) b
Note: breakpoint 4 also set at pc 0x60001388.
Breakpoint 5 at 0x60001388: file ../../arch/arm/armv7-a/switch_gcc.S, line 61.
```

bt
查看栈回溯
```
#0  os_task_switch () at ../../arch/arm/armv7-a/switch_gcc.S:61
#1  0x60000a0c in ygos_sche () at ../../kernel/core.c:233
#2  0x60001168 in ygos_sem_post (sem=0x60003134 <sem_mavlink1>) at ../../ipc/sem.c:83
#3  0x60001244 in task2_entry (arg=0x0) at main.c:35
#4  0x00000000 in ?? ()
```

info reg
查看当前寄存器
```
(gdb) info reg
r0             0x0	0
r1             0x600024c8	1610622152
r2             0x0	0
r3             0x1	1
r4             0x0	0
r5             0x0	0
r6             0x0	0
r7             0x0	0
r8             0x0	0
r9             0x0	0
r10            0x0	0
r11            0x60002f00	1610624768
r12            0x0	0
sp             0x60002ef4	0x60002ef4 <task2_stack+448>
lr             0x60000a0c	1610615308
pc             0x60001388	0x60001388 <os_task_switch>
cpsr           0x80000093	-2147483501
```

(gdb) b ygos_sche
建立断点的方法
Breakpoint 7 at 0x6000099c: file ../../kernel/core.c, line 224.

disassemble
查看当前的汇编
```
Dump of assembler code for function ygos_sche:
   0x60000990 <+0>:	push	{r11, lr}
   0x60000994 <+4>:	add	r11, sp, #4
   0x60000998 <+8>:	sub	sp, sp, #8
=> 0x6000099c <+12>:	bl	0x600000f0 <ygos_interrupt_disable>
   0x600009a0 <+16>:	mov	r3, r0
   0x600009a4 <+20>:	str	r3, [r11, #-8]
```






