#  介绍
这篇文档是解析在armv7-m芯片中，ygos任务调度函数的处理过程的lr的疑问.

## 调用ygos_sem_wait触发任务切换
> 最后一次任务调度保存的PC和LR如下,可以清除看到LR保存的地址也是临时的，而不是PC执行完退出的LR，
PC执行完，是靠pop进行弹栈处理的.

PC:     ygos_sche();        
    }
ygos_interrupt_enable(level);

LR:  os_task_switch();
    }
}
ygos_interrupt_enable(level);

```c
int ygos_sem_wait( sem_t *sem, uint32_t tick)
{   
    int level;

    level = ygos_interrupt_disable();   
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
         
		 //获取当前运行链表的节点
		 struct list_head * current = &ygos_tcb_current->list;
		
         //移除就绪链表
		 list_del(current);  
         
         //将当前链表添加到信号量等待队列
         list_add_tail(current, &sem->list); 

         //任务修改成未就绪
         ygos_task_ready_delete(ygos_prio_current);

         //任务休眠后，任务调度到最高优先级的任务
         ygos_sche();        
    }
    ygos_interrupt_enable(level);
    //信号量状态
    return ygos_tcb_current->status;
}



void  ygos_sche (void)
{   
	int  level; 
	level = ygos_interrupt_disable();
	//中断服务程序中，不允许任务切换
    if (ygos_interrupt_nest == 0u) {
		//查找最高优先级的任务
		ygos_sche_new();
		//通过优先级更新TCB
		ygos_tcb_high_ready  = tcb_prio_table[ygos_prio_hig_ready];
		//最高优先级就绪任务和当前任务不一样，则更新更新最高优先级任务
		if (ygos_prio_hig_ready != ygos_prio_current) { 
			os_task_switch();
		}
	}
	ygos_interrupt_enable(level);
}
```

# 总结
利用pendsv进行处理, 避免处理关中断和开中断的问题。及时触发了pendsvc，但是中断没有打开也是不能执行到任务的

