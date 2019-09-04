@[TOC]

# 资源
> task_ipc.md https://github.com/yangang123/ygOS.git

# 信号量接口
```
 //初始化信号量
int ygos_sem_init( sem_t *sem, unsigned int value);

//等待信号量
int ygos_sem_wait( sem_t *sem, uint32_t tick);

//发送信号量
int ygos_sem_post( sem_t *sem);
  
```

# 举例子
```
//信号量测试
sem_t sem_mavlink;

//任务1入口函数
void task1_entry(void *arg)
{   
	ygos_sem_init(&sem_mavlink, 0);
	while(1){
		task1_flag =1 ;
		//等待时间是1000ms
		ygos_sem_wait(&sem_mavlink, 1000);
		task1_flag =0 ;
	}
}

//任务2入口函数
void task2_entry(void *arg)
{
	while(1){
		task2_flag =1 ;
		ygos_sem_wait(&sem_mavlink, 1000);
	    task2_flag =0 ;
	}
}
```