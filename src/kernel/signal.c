#include <ygos/rtos.h>

#define sig_mask(sig_no)            (1u << sig_no)
#define sig_pending_set(sig_no)     (1u << sig_no)

//注册一个信号，指定异常处理函数
sighandler_t signal(int signum, sighandler_t handler)
{
    //检查信号是否在范围内
    if ( !(signum < SIG_NUM_MAX) ) {
        return NULL;
    }

    if (handler ==  NULL) {
        return NULL;
    }
    
    //获取当前线程TCB
    struct tcb_s * tcb; 

    tcb = ygos_tcb_self();
    tcb->sig_handler_table[signum] =  handler;
     
    return NULL;
}

//发送信号
int kill_task(int pid, int signum)
{   
    
    struct tcb_s * tcb; 
    
    //获取TCB
    tcb = ygos_tcb_get(pid);
    if (tcb == NULL) {
        return -1;
    }
  
    if (tcb->sig_mask & sig_mask(signum))  {
    //屏蔽此信号, 挂在到

    } else {
        // 未屏蔽此信号,则立即执行此信号
        tcb->sig_pending |= sig_pending_set(signum);

        // 当前线程和运行线程相等
        if  (pid == ygos_prio_current) {

        //直接运行
            if (tcb->sig_handler_table[signum]) {
               tcb->sig_handler_table[signum](signum);
            }
        } else {
        //等待
            if (tcb->sig_handler_table[signum]) {
               tcb->sig_handler_table[signum](signum);
            }
        }
    }
}

// //屏蔽某信号
// void sig_mask(int signum)
// {
//     tcb->sig_mask |= 1 < signum;
// }

// //解除某信号
// void sig_unmask(int signum)
// {
//     tcb->sig_mask &= ~(1 < signum);
// }