# ================================
# Toolchain
# ================================
CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
LD      = arm-none-eabi-ld

# ================================
# MCU Specific
# ================================
CPU        = cortex-m4
FPU        = fpv4-sp-d16
FLOAT-ABI  = hard

# ================================
# Build Flags
# ================================
CFLAGS = -mcpu=$(CPU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT-ABI)
CFLAGS += -O2 -g -Wall -Wextra
CFLAGS += -Iinclude -Icmsis_core -ffreestanding -nostdlib
CFLAGS += -D__FPU_PRESENT=1U
CFLAGS += -D__NVIC_PRIO_BITS=4U

ASFLAGS = -mcpu=$(CPU) -mthumb

# ================================
# Source Files
# ================================
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

# Object files
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

# ================================
# Output
# ================================
TARGET = ukernelos
ELF    = $(TARGET).elf
BIN    = $(TARGET).bin

# ================================
# Build Rules
# ================================
all: $(BIN)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) -T boot/linker.ld -o $@ $^ -lgcc

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Compile C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly files
%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

# ================================
# Utility Targets
# ================================
clean:
	rm -f $(OBJS) $(ELF) $(BIN)

flash: $(BIN)
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $(BIN) verify reset exit"

debug:
	arm-none-eabi-gdb $(ELF)

.PHONY: all clean flash debug
