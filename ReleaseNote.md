# ygOS说明


# v1.0.0_beta1
- 支持任务管理
- 支持信号量
- 支持休眠

# v1.0.1_beta1
- 支持系统调用open, read, write, close 
- 支持虚拟文件系统设备树node

```
用户看到目录
/dev 
    console
    null 
    ramlog 
```

```
内核中的二叉树
    dev
    /
console
    \
      null
        \
        ramlog
```

# v1.0.2_beta1
-  支持通过mount挂载proc虚拟文件系统
-  支持通过proc文件系统读取/proc/uptime获取系统tick


# v1.0.3_beta1
-  支持通过mount挂载FAT32文件系统
-  支持通过ygOS中的FAT32文件系统，直接访问linux的块设备
```
mount(NULL, "/fat", "fatfs", 0, NULL)
cat("/fat/test1.c");
````