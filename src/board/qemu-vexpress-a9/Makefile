TOOL_CHAIN = arm-none-eabi-
CC 		= ${TOOL_CHAIN}gcc
AS 		= ${TOOL_CHAIN}as
LD 		= ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS	 =	-Wall -g -march=armv7-a -marm -msoft-float -Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB
CFLAGS	+=	-I./
LDFLAGS	:= 	-g -Map ygos.map
objs 	:=  main.o  

-march=armv7-a -marm -msoft-float -nostartfiles -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,system_vectors

# os 部分
CFLAGS		+=-D GCC_BUILD
CFLAGS		+=-I../../include
KERNEL_DIR	=../../kernel
IPC_DIR		=../../ipc

ARCH_DIR	=../../arch/arm/armv7-a

objs		+=$(ARCH_DIR)/vector_gcc.o \
	$(ARCH_DIR)/start_gcc.o \
	$(ARCH_DIR)/switch_gcc.o \
	$(ARCH_DIR)/stack.o 

objs		+=$(KERNEL_DIR)/core.o \
	$(KERNEL_DIR)/idle.o \
	$(KERNEL_DIR)/sleep.o\
	$(IPC_DIR)/sem.o

BUILD_DIR:=build

object 	:=	$(BUILD_DIR)/vector_gcc.o 
object 	+=	$(BUILD_DIR)/start_gcc.o 
object 	+=	$(BUILD_DIR)/switch_gcc.o 
object 	+=	$(BUILD_DIR)/stack.o 

object 	+=	$(BUILD_DIR)/core.o 
object 	+=	$(BUILD_DIR)/idle.o 
object 	+=	$(BUILD_DIR)/sleep.o 
object 	+=	$(BUILD_DIR)/sem.o

object 	+=	$(BUILD_DIR)/main.o


ygos.bin: $(objs) $(SRC) move
	${LD} $(LDFLAGS) -T ygos.ld -o ygos.elf $(object)
	${OBJCOPY} -O binary -S ygos.elf $@
	${OBJDUMP} -D -m arm ygos.elf > ygos.dis

run: $(objs) ygos.bin 
	qemu-system-arm -M vexpress-a9 -kernel ygos.elf -serial stdio   

debug: $(objs) ygos.bin
	qemu-system-arm -M vexpress-a9 -kernel ygos.elf -s -S

gdb: $(objs) ygos.bin
	arm-none-eabi-gdb *.elf

%.o:%.c 
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.S
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis *.map
	
move: 
	@rm build -rf 
	@mkdir build
	@mv $(objs)  build