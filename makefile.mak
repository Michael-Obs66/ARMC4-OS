# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

# Flags
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -O2 -nostdlib -ffreestanding
ASFLAGS = -mcpu=cortex-m4 -mthumb
LDFLAGS = -T boot/linker.ld -nostdlib

# Sources
SRCS = \
    boot/startup.s \
    boot/boot.c \
    kernel/kernel.c \
    kernel/scheduler.c \
    kernel/task.c \
    kernel/ipc.c \
    arch/arm/cortex_m4.c \
    arch/arm/context_switch.s \
    arch/arm/interrupt.c \
    mm/mm.c \
    mm/heap.c \
    mm/page.c \
    drivers/uart.c \
    drivers/gpio.c \
    drivers/timer.c \
    drivers/spi.c \
    lib/string.c \
    lib/printf.c \
    lib/assert.c \
    tests/test_kernel.c \
    tests/test_memory.c \
    tests/test_stress.c \
    apps/main.c

# Objects
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

# Target
TARGET = ukernelos
test: $(TARGET).bin
    echo "Flashing and testing μKernelOS..."
    make flash
    # Tambahkan script testing hardware di sini

test-sim: $(TARGET).elf
    echo "Running tests in simulator..."
    arm-none-eabi-gdb -x test_commands.gdb $(TARGET).elf
all: $(TARGET).bin

$(TARGET).elf: $(OBJS)
    $(LD) $(LDFLAGS) -o $@ $^

$(TARGET).bin: $(TARGET).elf
    $(OBJCOPY) -O binary $< $@

%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s
    $(AS) $(ASFLAGS) -o $@ $<

clean:
    rm -f $(OBJS) $(TARGET).elf $(TARGET).bin

flash: $(TARGET).bin
    openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg -c "program $(TARGET).bin verify reset exit 0x08000000"

.PHONY: all clean flash


