# 信号实现


## 应用接口
#include <signal.h>

void handle(int signum)
{
	printf("我收到了SIGUSR1\n");
	exit(0);
}
int main()
{
	signal(SIGUSR1,handle);
	for(;;);
}

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

//触发信号，则位置1,如果对应的sig_mask
//对应位是0，则执行信号绑定的处理函数
rt_sigset_t     sig_pending;

//信号掩码是1,则屏蔽此信号                       
rt_sigset_t     sig_mask;

//保存当前线程的栈地址                          
uint32_t        *sig_ret; 
    
//信号处理函数的标
rt_sighandler_t *sig_handler_table[SIG_NUM_MAX];  


> 如果发出的当前信号，已经被挂载到了队列中，要更新信号的内容