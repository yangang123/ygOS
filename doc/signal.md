# 信号实现

## 应用接口
#include <signal.h>

### 注册一个信号，指定异常处理函数
```c
sighandler_t signal(int signum, sighandler_t handler)
```

### 发送信号
```c
int kill_task(int pid, int signum)
```

### 屏蔽某信号
```c
void sig_mask(int signum)
```

### 解除某信号
```c
void sig_unmask(int signum)
```
## 内核实现

```c

//信号掩码是1,则屏蔽此信号                       
rt_sigset_t     sig_mask;

//保存当前线程的栈地址                          
uint32_t        *sig_ret; 
    
//信号注册
struct list_head signal_list; 

//待执行信号队列
struct list_head signal_pending_list;
```

## 用户进程执行

cortex M一共有16个寄存器, 前8个寄存器XPS....R0是硬件触发3异常自动保存8个寄存器
后8个寄存器R11...R4是OS在任务切换的时候保存到堆栈中.

序号 | 地址 |寄存器| 保存堆栈
---|  ----  | ----  |----
1|ADD|XPSR|硬件
2|ADD-4|PC|硬件
3|ADD-8|LR|硬件
4|ADD-12|R12|硬件
5|ADD-16|R3|硬件
6|ADD-20|R2|硬件
7|ADD-24|R1|硬件
8|ADD-28|R0|硬件
9|ADD-32|R11|软件
10|ADD-36|R10|软件
11|ADD-40|R9|软件
12|ADD-44|R8|软件
13|ADD-48|R7|软件
14|ADD-52|R6|软件
15|ADD-56|R5|软件
16|ADD-60|R4|软件

### 指定信号处理函数
> 在上面分析后, 得知需要在保存上一次堆栈的位置,然后把当前堆栈的位置作为栈顶手动初始化
> 这16个寄存器

```c
//保存堆栈指针stack_ptr到sig_ret
tcb->sig_ret = tcb->stack_ptr;

//设置异常处理函数为线程新的入口函数,重新初始哈堆栈
uint32_t *stk = ygos_task_stack_init(ygos_signal_deliver_entry, 0, tcb->stack_ptr, 0);

//设置新的堆栈位置
tcb->stack_ptr = stk;
```

### 在信号处理函数重新恢复线程的PC指针,堆栈指针
```c
//恢复保存的堆栈指针
tcb->stack_ptr=tcb->sig_ret;
tcb->sig_ret=NULL;

//调度任务到新旧的线程位置
os_task_switch_to();
```
