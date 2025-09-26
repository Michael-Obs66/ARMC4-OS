# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

# MCU specific
CPU = cortex-m4
FPU = fpv4-sp-d16
FLOAT-ABI = hard

# Build flags
CFLAGS = -mcpu=$(CPU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)
CFLAGS += -O2 -g -Wall -Wextra
CFLAGS += -Iinclude -ffreestanding -nostdlib
CFLAGS += -DSystemCoreClock=16000000UL
CFLAGS += -Icmsis_core

ASFLAGS = -mcpu=$(CPU) -mthumb

# Source files
SRCS = boot/startup.s \
       boot/boot.c \
       kernel/kernel.c \
       kernel/scheduler.c \
       kernel/task.c \
       kernel/ipc.c \
       arch/arm/cortex_m4.c \
       arch/arm/interrupt.c \
       mm/mm.c \
       mm/heap.c \
       mm/mpu.c \
       drivers/uart.c \
       drivers/gpio.c \
       drivers/timer.c \
       apps/blinky.c \
       apps/shell.c

OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

# Output
TARGET = ukernelos
ELF = $(TARGET).elf
BIN = $(TARGET).bin

# Rules
all: $(BIN)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) -T boot/linker.ld -o $@ $^ -lgcc

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(ELF) $(BIN)

flash: $(BIN)
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BIN) verify reset exit"

debug:
	arm-none-eabi-gdb $(ELF)

renode: $(ELF)
	renode -e "mach create; machine LoadPlatformDescription @platforms/boards/stm32f4_discovery.repl; sysbus LoadELF @$(ELF); start"

.PHONY: all clean flash debug


