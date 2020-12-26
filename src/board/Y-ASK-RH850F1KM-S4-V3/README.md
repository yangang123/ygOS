# 环境
- CS+ for cc
- Y-ASK-RH850F1KM-S4-V3
- R7F7106493

# cs+配置

##  c语言环境
设置成c99

## E2仿真器
设置成16Mhz时钟，CPU时钟是120Mhz

## 编译宏
__CSP__
GCC_BUILD
RH850_CHIP
LINUX

## 头文件路径
..\..\source\yangang456-ygOS-develop\ygOS\src\include

## main.c
删除main.c

## boot.asm文件修改

### 1. 
.extern __lowinit
++++++++
.extern _ei_interrupt_common
.extern _os_task_switch_low
++++++++

### 2. 
.align	16
jr32	_Dummy ; FETRAP
++++++++
.align	16
jr32	_os_task_switch_low ; TRAP0
++++++++
.align	16
jr32	_Dummy_EI ; TRAP1

### 3. 
.offset (0x51*4)
.dw	#_INTTAUJ0I1
++++++++
.offset (0x54*4)
.dw	#_ei_interrupt_common
++++++++
.offset (0x013B*4)
.dw	#_INTETNB0DATA

### 4. 直接中断优先级地址
```
.align	16
	syncp
	jr32	_ei_interrupt_common ; INTn(priority0)

	.align	16
	syncp
	jr32	_ei_interrupt_common ; INTn(priority1)
    

	.align	16
	syncp
	jr32	_ei_interrupt_common ; INTn(priority15)
```

# 编译信息
```
>DefaultBuild\F1KM-S4_StarterKit_Project.abs DefaultBuild\F1KM-S4_StarterKit_Project.mot
W0561016:The evaluation version is valid for the remaining 10 days
W0561016:The evaluation version is valid for the remaining 10 days
Renesas Optimizing Linker Completed
------ Build ended(Error:0, Warning:22)(F1KM-S4_StarterKit_Project, DefaultBuild) ------
========== Ended(Success:1 Projects, Failed:0 Projects)(2020年12月25日 10:52:58) ==========
```

