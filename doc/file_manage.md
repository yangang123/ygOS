@[TOC]

# 资源
> file_manage.md https://github.com/yangang123/ygOS.git

# 信号量接口
```
int ygopen( const char * path, int oflags, ...);
int ygread(int fd,void * buf ,int count);
int ygwrite(int fd,void * buf ,int count);
int ygclose(int fd);
```

# 举例子
```
 if (!mount(NULL, "/fat", "fatfs", 0, NULL)) {
    DEBUG_LR("fatfs mount ok!");
 }

cat("/fat/test1.c");
```