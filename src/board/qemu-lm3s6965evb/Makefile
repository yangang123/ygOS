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
objs		+=$(KERNEL_DIR)/core.o \
	$(KERNEL_DIR)/idle.o \
	$(KERNEL_DIR)/sleep.o\
	$(KERNEL_DIR)/switch_gcc.o
	
ygos.bin: $(objs) $(SRC)
	${LD} -T ygos.ld -o ygos.elf $^
	${OBJCOPY} -O binary -S ygos.elf $@
	${OBJDUMP} -D -m arm ygos.elf > ygos.dis

run: $(objs)
	${LD} -T ygos.ld -o ygos.elf $^
	${OBJCOPY} -O binary -S ygos.elf ygos.bin
	${OBJDUMP} -D -m arm ygos.elf > ygos.dis
	qemu-system-arm -M lm3s6965evb --kernel ygos.bin -nographic

debug: $(objs)
	${LD} -T ygos.ld -o ygos.elf $^
	${OBJCOPY} -O binary -S ygos.elf ygos.bin
	${OBJDUMP} -D -m arm ygos.elf > ygos.dis
	qemu-system-arm -M lm3s6965evb --kernel ygos.bin -nographic -s -S

%.o:%.c 
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis
