# 准备工作
## 本单板基于qemu的, 先安装qemu
```
sudo apt-get install qemu
```

# 调试

## 启动GDB服务端
命令
```
$ make debug 
```

qemu输出信息
```
yangang@yangang-ubuntu:~/work/github_proj/ygOS/src/board/qemu-lm3s6965evb$ make debug 
arm-none-eabi-ld -T ygos.ld -o ygos.elf vector.o main.o debug.o systick.o ../../kernel/core.o ../../kernel/idle.o ../../kernel/sleep.o ../../kernel/switch_gcc.o
arm-none-eabi-objcopy -O binary -S ygos.elf ygos.bin
arm-none-eabi-objdump -D -m arm ygos.elf > ygos.dis
qemu-system-arm -M lm3s6965evb --kernel ygos.bin -nographic -s -S

```

## 启动GDB客户端
命令
```
$ make gdb
```

qemu输出信息
````
yangang@yangang-ubuntu:~/work/github_proj/ygOS/src/board/qemu-lm3s6965evb$ arm-none-eabi-gdb ygos.elf
GNU gdb (GNU Tools for ARM Embedded Processors) 7.10.1.20160616-cvs
Copyright (C) 2015 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-linux-gnu --target=arm-none-eabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ygos.elf...done.
(gdb) target remote:1234
Remote debugging using :1234
system_vectors () at vector_gcc.S:7
7	    ldr pc, _vector_reset
(gdb) watch task1_flag 
Hardware watchpoint 1: task1_flag
(gdb) watch task2_flag 
Hardware watchpoint 2: task2_flag
(gdb) c
Continuing.
Hardware watchpoint 1: task1_flag

Old value = 0
New value = 1
task1_entry (arg=0x0) at main.c:26
26			ygos_sem_post(&sem_mavlink2);
(gdb) c
Continuing.
Hardware watchpoint 2: task2_flag

Old value = 0
New value = 1
task2_entry (arg=0x0) at main.c:37
37			ygos_sem_wait(&sem_mavlink2, 100);


````