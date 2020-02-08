#ifndef _SIGNAL_H
#define _SIGNAL_H

typedef unsigned long sigset_t;

#define SIG_NUM_MAX 32

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
sighandler_t signal(int signum, sighandler_t handler);
int kill_task(int pid, int signum);
#endif /* _SIGNAL_H */