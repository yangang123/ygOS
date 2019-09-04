@[TOC]

# 资源
>  device_manage.md
>  https://github.com/yangang123/ygOS
>  闫刚 ygOS设备管理

# 设备注册方法
```
int register_driver(const char*path, const struct file_operations *fops, void *priv)          
```

# 举例子
```
//注册驱动节点
register_driver("/dev/ramlog", &g_ramlogops, (void*)0);

//通过VFS读取文件
echo("log", "/dev/ramlog");
cat("/dev/ramlog");
```
