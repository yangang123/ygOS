#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <ygos/list.h>

typedef unsigned long sigset_t;

#define SIG_NUM_MAX                     32
#define SIGNAL_TOTAL_NUM_MAX            32 
#define SIGNAL_PENDING_TOTAL_NUM_MAX    10 

typedef enum {
    SIGHUP=0,	 
    SIGINT,	 
    SIGQUIT,	 
    SIGILL,	 
    SIGTRAP,
    SIGABRT,	 
    SIGBUS,	 
    SIGFPE,	 
    SIGKILL,	
    SIGUSR1,
    SIGSEGV,
    SIGUSR2,	
    SIGPIPE,
    SIGALRM,	
    SIGTERM,
    SIGSTKFLT,	
    SIGCHLD,	
    SIGCONT,	
    SIGSTOP,	
    SIGTSTP,
    SIGTTIN,	
    SIGTTOU,	
    SIGURG,	
    SIGXCPU,	
    SIGXFSZ,
    SIGVTALRM,	
    SIGPROF,	
    SIGWINCH,	
    SIGIO,	
    SIGPWR,
    SIGSYS
}signal_t;

typedef void (*sighandler_t)(int);

//描述注册信号
struct sigactq_s
{
    struct list_head list;
    int   signo;    
    sighandler_t handler;
};

//描述信号挂起
struct sigactpending_s
{
    struct list_head list;
    int   signo;   
    sighandler_t handler; 
};

//注册一个信号，指定异常处理函数
sighandler_t    ygos_signal(int signum, sighandler_t handler);

//发送信号
int             ygos_kill_task(int pid, int signum);

//初始化TCB的空闲链表空间
void            ygos_signal_list_init(void);


#endif /* _SIGNAL_H */
