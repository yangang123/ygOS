#include <ygos/rtos.h>

#define sig_mask(sig_no)            (1u << sig_no)
#define sig_pending_set(sig_no)     (1u << sig_no)

//空闲的信号链表    
struct list_head ygos_signal_free_list; 

//激活信号链表
struct list_head ygos_signal_list;

//信号内存空间
struct sigactq_s signal_table[SIGNAL_TOTAL_NUM_MAX];

//初始化TCB的空闲链表空间
void ygos_signal_list_init(void)
{   
    //初始化空闲链表头节点
    INIT_LIST_HEAD(&ygos_signal_free_list);

    //把所有内存节点链表节点写入添加到空闲链表中
    for (uint8_t i =0; i < SIGNAL_TOTAL_NUM_MAX; i++) {
        list_add_tail(&signal_table[i].list, &ygos_signal_free_list); 
    }
}

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

    struct list_head *first = list_get_first(&ygos_signal_free_list);
    struct sigactq_s* sigact = list_entry(first, struct sigactq_s, list);
     
    sigact->signo    = signum;
    sigact->handler  = handler;

    //删除ygos_tcb_free_list的第一个节点的链表
    list_del_first(&ygos_signal_free_list);

    //把当前节点添加到激活链表中
    struct tcb_s * tcb; 
    tcb = ygos_tcb_self();
    list_add_tail(first, &tcb->signal_list);
    
    return NULL;
}

void ygos_signal_deliver(struct tcb_s * tcb)
{
    task1_signal_handler(2);
    // tcb->stack_ptr = tcb->sig_ret；
    // // tcb->sig_ret   = NULL;
    // // tcb

    tcb->stack_ptr=tcb->sig_ret;
    tcb->sig_ret=NULL;
    ygos_start_high_ready();

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
        // 当前线程和运行线程相等
        if  (pid == ygos_prio_current) {

            // 未屏蔽此信号,则立即执行此信号
            tcb->sig_pending |= sig_pending_set(signum);
            
            if(!list_empty(&tcb->signal_list)) {
                struct sigactq_s* sigact;
                list_for_each_entry(sigact, &tcb->signal_list, struct sigactq_s, list) {
                    if (sigact->signo == signum) {
                        if (sigact->handler) {
                            sigact->handler(signum);
                        }
                        break;
                    }
                }
            }
        } else {
            //等待
            // if (tcb->sig_handler_table[signum]) {
            //    tcb->sig_handler_table[signum](signum);
            // }

        //设置信号处理函数为入口函数
        int level;

	    level = ygos_interrupt_disable();

        //保存当前堆栈的位置
        tcb->sig_ret = tcb->stack_ptr;
        uint32_t *stk = ygos_task_stack_init(ygos_signal_deliver, 0, tcb->stack_ptr, 0);
		tcb->stack_ptr = stk;
        ygos_interrupt_enable(level);
        //重新调度一下.
        }
    }
}



//屏蔽某信号
void ygos_sig_mask(int signum)
{   
    struct tcb_s * tcb; 
    tcb = ygos_tcb_self();
    if (tcb == NULL) {
        return;
    }

    tcb->sig_mask |= 1 < signum;
}

//解除某信号
void ygos_sig_unmask(int signum)
{   
    struct tcb_s * tcb; 
    tcb = ygos_tcb_self();
    if (tcb == NULL) {
        return;
    }

    tcb->sig_mask &= ~(1 < signum);
}