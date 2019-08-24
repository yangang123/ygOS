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
