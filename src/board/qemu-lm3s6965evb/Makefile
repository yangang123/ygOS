TOOL_CHAIN = arm-none-eabi-
CC 		= ${TOOL_CHAIN}gcc
AS 		= ${TOOL_CHAIN}as
LD 		= ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS	 =	-Wall -g -mlittle-endian -mthumb -nostdlib -ffreestanding
CFLAGS	+=	-mcpu=cortex-m3
CFLAGS	+=	-I./
LDFLAGS	:= 	-g
objs 	:= 	vector.o main.o debug.o systick.o

# os 部分
CFLAGS		+=-D GCC_BUILD
CFLAGS		+=-I../../include
KERNEL_DIR	=../../kernel
IPC_DIR	=../../ipc

ARCH_DIR	=../../arch/arm/armv7-m
objs		+=$(ARCH_DIR)/switch_gcc.o \
	$(ARCH_DIR)/stack.o 

objs		+=$(KERNEL_DIR)/core.o \
	$(KERNEL_DIR)/idle.o \
	$(KERNEL_DIR)/sleep.o\
	$(KERNEL_DIR)/signal.o\
	$(IPC_DIR)/sem.o


BUILD_DIR:=build

object 	:=	$(BUILD_DIR)/vector.o 
object 	+=	$(BUILD_DIR)/main.o 
object 	+=	$(BUILD_DIR)/debug.o 
object 	+=	$(BUILD_DIR)/systick.o 

object 	+=	$(BUILD_DIR)/core.o 
object 	+=	$(BUILD_DIR)/signal.o
object 	+=	$(BUILD_DIR)/idle.o 
object 	+=	$(BUILD_DIR)/sleep.o 
object 	+=	$(BUILD_DIR)/sem.o
object 	+=	$(BUILD_DIR)/switch_gcc.o
object 	+=	$(BUILD_DIR)/stack.o

ygos.bin: $(objs) $(SRC) move 
	${LD} -T ygos.ld -o ygos.elf $(object)
	${OBJCOPY} -O binary -S ygos.elf $@
	${OBJDUMP} -D -m arm ygos.elf > ygos.dis

run: $(objs) ygos.bin 
	qemu-system-arm -M lm3s6965evb --kernel ygos.bin -serial stdio 

debug: $(objs) ygos.bin
	qemu-system-arm -M lm3s6965evb --kernel ygos.bin -serial stdio -s -S

move: 
	@rm build -rf 
	@mkdir build
	@mv $(objs)  build

%.o:%.c 
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis build
