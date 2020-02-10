# 准备工作
## 本单板基于qemu的, 先安装qemu
```
qemu版本:  4.2.0
```

# 调试

## 启动GDB服务端
命令
```
$ make debug 
```

qemu输出信息
```
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o main.o main.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/vector_gcc.o ../../arch/arm/armv7-a/vector_gcc.S
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/start_gcc.o ../../arch/arm/armv7-a/start_gcc.S
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/switch_gcc.o ../../arch/arm/armv7-a/switch_gcc.S
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/stack.o ../../arch/arm/armv7-a/stack.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../kernel/core.o ../../kernel/core.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../kernel/idle.o ../../kernel/idle.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../kernel/sleep.o ../../kernel/sleep.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../ipc/sem.o ../../ipc/sem.c
arm-none-eabi-ld -g -Map ygos.map -T ygos.ld -o ygos.elf build/vector_gcc.o  build/start_gcc.o  build/switch_gcc.o  build/stack.o  build/core.o  build/idle.o  build/sleep.o  build/sem.o build/main.o
arm-none-eabi-objcopy -O binary -S ygos.elf ygos.bin
arm-none-eabi-objdump -D -m arm ygos.elf > ygos.dis
/home/yangang/work/qemu-4.2.0/yangang_arm/bin/qemu-system-arm -M mcimx6ul-evk -kernel ygos.elf -s -S

```

## 启动GDB客户端
命令
```
$ make gdb
```

qemu输出信息
````
yangang@ubuntu:~/work/ygOS/src/board/qemu-mcimx6ul-evk$ make gdb
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o main.o main.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/vector_gcc.o ../../arch/arm/armv7-a/vector_gcc.S
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/start_gcc.o ../../arch/arm/armv7-a/start_gcc.S
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/switch_gcc.o ../../arch/arm/armv7-a/switch_gcc.S
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../arch/arm/armv7-a/stack.o ../../arch/arm/armv7-a/stack.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../kernel/core.o ../../kernel/core.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../kernel/idle.o ../../kernel/idle.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../kernel/sleep.o ../../kernel/sleep.c
arm-none-eabi-gcc -Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB -I./ -D GCC_BUILD -I../../include -c -o ../../ipc/sem.o ../../ipc/sem.c
arm-none-eabi-ld -g -Map ygos.map -T ygos.ld -o ygos.elf build/vector_gcc.o  build/start_gcc.o  build/switch_gcc.o  build/stack.o  build/core.o  build/idle.o  build/sleep.o  build/sem.o build/main.o
arm-none-eabi-objcopy -O binary -S ygos.elf ygos.bin
arm-none-eabi-objdump -D -m arm ygos.elf > ygos.dis
arm-none-eabi-gdb *.elf
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
(gdb) target remote :1234
Remote debugging using :1234
system_vectors () at ../../arch/arm/armv7-a/vector_gcc.S:7
7	    ldr pc, _vector_reset
(gdb) b main
Breakpoint 1 at 0x87801220: file main.c, line 43.
(gdb) c
Continuing.


````