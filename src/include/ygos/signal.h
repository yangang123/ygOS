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

struct sigactq_s
{
    struct list_head list;
    int   signo;    
    sighandler_t handler;
};

struct sigactpending_s
{
    struct list_head list;
    int   signo;   
    sighandler_t handler; 
};

sighandler_t    signal(int signum, sighandler_t handler);
int             kill_task(int pid, int signum);
void            ygos_signal_list_init(void);

#endif /* _SIGNAL_H */
