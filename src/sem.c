
#include "rtos.h"
#include "list.h"
//初始化信号量
int ygos_sem_init( sem_t *sem, unsigned int value)
{
    if (sem && value <= SEM_VALUE_MAX)
    {
      //信号量资源初始化
      sem->semcount         = (int16_t)value;
      INIT_LIST_HEAD(&sem->list);
    }

    return 0;
}

int ygos_sem_wait( sem_t *sem, uint32_t tick)
{   
    int level;

    ygos_interrupt_disable();   
    if (sem->semcount) {
        //资源减少1，
        sem->semcount--;
        ygos_interrupt_enable(level);
        return 0;
    } else {  
         //更新超时时间
         ygos_tcb_current->sleep_tick = tick;
         //当前任务状态为等待状态
         ygos_tcb_current->status = TASK_WAIT_SEM;

         list_add_tail(&(ygos_tcb_current->list), &sem->list); 

         //任务修改成未就绪
         ygos_task_ready_delete(ygos_prio_current);

         //任务休眠后，任务调度到最高优先级的任务
         ygos_sche();        
    }
    ygos_interrupt_enable(level);
    //信号量状态
    return ygos_tcb_current->status;
}

int ygos_sem_post( sem_t *sem)
{   
    int level;

    //如果有任务等待当前信号，那么就直接切换任务,不需要加1操作
    ygos_interrupt_disable(); 

    if (!list_empty(&sem->list)) {
        struct tcb_s    *ptcb  = list_entry(sem->list.next, struct tcb_s, list);
        if (ptcb) {
            //删除当前任务
            list_del_first(&sem->list);
            
            //任务状态变为就绪态
            ptcb->status = TASK_READY_RUN; 
            //任务等待资源
            ptcb->wait_status = WAIT_OK;
            //清除资源超时计数状态
            ptcb->sleep_tick = 0;
            //当前任务变为就绪态
            ygos_task_ready_add(ptcb->prio);
            //任务调度
            ygos_sche();

            ygos_interrupt_enable(level);
            return 0;
        }
    }

    //信号量资源加1
    ygos_interrupt_disable();   
    sem->semcount++;
    if (sem->semcount >= 65536) {
        sem->semcount = 65536;
    }
    ygos_interrupt_enable(level);

    return 0; 
}
