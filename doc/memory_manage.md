@[TOC]

# 资源
> memory_manage.md https://github.com/yangang123/ygOS.git

# 信号量接口
```
void    ygos_mem_init(void);
void*   ygos_malloc(int size);
int     ygos_free(void *ptr);
```

# 举例子
```
 for (int i = 0; i < 6; i++) {
    void *p= ygos_malloc(1<<(4+i));   
    printf("i: %d, p:%x\n",1<<(4+i), p);
    ygos_free(p);
}
```