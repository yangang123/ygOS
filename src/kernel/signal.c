#include <ygos/rtos.h>

#define sig_mask(sig_no)            (1u << sig_no)
#define sig_pending_set(sig_no)     (1u << sig_no)

//空闲的信号链表    
struct list_head ygos_signal_free_list; 
//信号内存空间
struct sigactq_s signal_table[SIGNAL_TOTAL_NUM_MAX];

//空闲的信号链表    
struct list_head ygos_signal_pending_free_list; 
//信号内存空间
struct sigactpending_s signal_pending_table[SIGNAL_PENDING_TOTAL_NUM_MAX];

//初始化TCB的空闲链表空间
void ygos_signal_list_init(void)
{   
    //初始化空闲链表头节点
    INIT_LIST_HEAD(&ygos_signal_free_list);

    //把所有内存节点链表节点写入添加到空闲链表中
    for (uint8_t i =0; i < SIGNAL_TOTAL_NUM_MAX; i++) {
        list_add_tail(&signal_table[i].list, &ygos_signal_free_list); 
    }

     //初始化空闲链表头节点
    INIT_LIST_HEAD(&ygos_signal_pending_free_list);

    //把所有内存节点链表节点写入添加到空闲链表中
    for (uint8_t i =0; i < SIGNAL_PENDING_TOTAL_NUM_MAX; i++) {
        list_add_tail(&signal_pending_table[i].list, &ygos_signal_pending_free_list); 
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
    
    //这里仅仅是多线程打断, 可以选择互斥锁或者阻塞调度方式进行临界保护,提高系统并行能力
    int level = ygos_interrupt_disable();

    //申请信号内存对象,在free_list中移除,同时把内存对象放到tcb队列中
    struct list_head *first = list_get_first(&ygos_signal_free_list);
    struct sigactq_s* sigact = list_entry(first, struct sigactq_s, list);
    sigact->signo    = signum;
    sigact->handler  = handler;
  
    //把当前节点添加到激活链表中
    struct tcb_s * tcb; 
    tcb = ygos_tcb_self();
    list_add_tail(first, &tcb->signal_list);
    
    ygos_interrupt_enable(level);

    return NULL;
}

//分配内存
struct sigactpending_s*  ygos_signal_action_pending_alloc(void)
{   
    int level = ygos_interrupt_disable();

    struct list_head *first = list_get_first(&ygos_signal_pending_free_list);
    struct sigactpending_s* sigactpending = list_entry(first, struct sigactpending_s, list);

    //ygos_signal_pending_free_list
    list_del_first(&ygos_signal_pending_free_list);
    
    ygos_interrupt_enable(level);

    return sigactpending;
}

//释放内存
void ygos_signal_action_pending_free( struct sigactpending_s* pending)
{   
    int level = ygos_interrupt_disable();

    //从链表中删除
    list_del(&pending->list);

    //添加到空闲链表中
    list_add_tail(&pending->list, &ygos_signal_pending_free_list);
    
    ygos_interrupt_enable(level);
}

//信号执行
void ygos_signal_handler(void)
{  
    struct tcb_s * tcb; 
    tcb = ygos_tcb_self();
    if (tcb == NULL) {
        return;
    }
    
    //遍历执行队列中所有节点
    int level = ygos_interrupt_disable();

    if(!list_empty(&tcb->signal_pending_list)) {
        struct sigactpending_s* sigact_pending;
        list_for_each_entry(sigact_pending, &tcb->signal_pending_list, struct sigactpending_s, list) {
            if (sigact_pending->handler) {
                sigact_pending->handler(sigact_pending->signo);
            }
            ygos_signal_action_pending_free(sigact_pending);
            if(list_empty(&tcb->signal_pending_list)) {
                break;
            }
			
        }
    }
    ygos_interrupt_enable(level);
}

//从队列中查找匹配信号值的信号对象
static struct sigactq_s* ygos_signal_find_action(struct tcb_s * tcb, int signum)
{   
	struct sigactq_s* sigact_p = NULL;
	
    int level = ygos_interrupt_disable();

    if(!list_empty(&tcb->signal_list)) {
        struct sigactq_s* sigact;
        list_for_each_entry(sigact, &tcb->signal_list, struct sigactq_s, list) {
            if (sigact->signo == signum) {
				sigact_p = sigact;
				break;
            }
        }
    }
    ygos_interrupt_enable(level);

    return sigact_p;
}

//新的异常处理入口
static void ygos_signal_deliver_entry(void *p_arg)
{  
    ygos_signal_handler();

    struct tcb_s * tcb; 
    tcb = ygos_tcb_self();
    if (tcb == NULL) {
        return;
    }

    tcb->stack_ptr=tcb->sig_ret;
    tcb->sig_ret=NULL;
    os_task_switch_to();
}

//信号到达处理函数
static void ygos_signal_deliver(int pid)
{ 
    struct tcb_s * tcb; 

    //获取TCB
    tcb = ygos_tcb_get(pid);
    if (tcb == NULL) {
        return;
    }
	
    // 当前线程和运行线程相等
    if  (pid == ygos_prio_current) {
        // 未屏蔽此信号,则立即执行此信号
        ygos_signal_handler();
    } else {
        int level;
        level = ygos_interrupt_disable();
        //保存当前堆栈的位置, 设置信号处理函数为入口函数
        tcb->sig_ret = tcb->stack_ptr;
        uint32_t *stk = ygos_task_stack_init(ygos_signal_deliver_entry, 0, tcb->stack_ptr, 0);
        tcb->stack_ptr = stk;
        ygos_interrupt_enable(level);
    }
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
    
    //信号添加到执行队列中
    if (tcb->sig_mask & sig_mask(signum))  {
        //屏蔽信号, 先不实现
        return -1;
    } else {
        
        struct sigactq_s* sigact;
        sigact = ygos_signal_find_action(tcb, signum);
        if (sigact == NULL) {
            return -1;
        }
        
        int level;
        level = ygos_interrupt_disable();

        //申请信号内存对象,在free_list中移除,同时把内存对象放到tcb队列中
        struct sigactpending_s* sigactpending =  ygos_signal_action_pending_alloc();
        sigactpending->signo    =   signum;
        sigactpending->handler  =   sigact->handler;
    
        //把当前节点添加到激活链表中
        list_add_tail(&sigactpending->list, &tcb->signal_pending_list);

        ygos_interrupt_enable(level);
    }
	
    //处理信号等待执行队列
    ygos_signal_deliver(pid);

    return 0;
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

