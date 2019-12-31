# ygOS说明

ygOS是一款宏内核设计的简约的操作系统,包含任务管理, 任务通信, 设备管理,文件管理，内存管理五大模块，整个代码低于2000行

# 硬件平台

## 所有支持硬件平台
```
linux simulator  
mdk simulator     
stm32f4discovery 
qemu-lm3s6965evb  
stm32f1     
```

## 推荐qemu-lm3s6965evb平台测试
```
cd src/board/qemu-lm3s6965evb
make run
```
      
# 功能说明
- 线程管理最多支持32个线程
- 线程调度是完全基于线程优先级的抢占式调度
- 支持线程主动休眠
- 支持线程同步通信对象：信号量
- 支持基于内存池的内存管理malloc和free
- 支持FAT32文件系统
- 整个内核代码不到2000行
- 支持VFS文件系统
- 支持proc虚拟文件系统
- 支持基于二叉树的驱动设备管理

# 构建

## linux平台
```sh
# git clone https://github.com/yangang123/ygOS.git
# cd ygOS && make build && cd build
# cmake ../
# make 
```


# 目录树
```
yangang@ubuntu:~/work/ygOS$
.
├── app
│   ├── platform
│   └── ygsh
├── board
│   ├── mdk_sim
│   └── stm32f4_discovery
├── CMakeLists.txt
├── doc
│   └── kernel_list_graph.png
├── project
│   ├── project-mdk-sim
│   └── project-stm32f4discovery
├── README.md
├── ReleaseNote.md
└── src
    ├── arch
    ├── driver
    ├── fs
    ├── include
    ├── ipc
    ├── kernel
    └── mm

18 directories, 4 files
```



