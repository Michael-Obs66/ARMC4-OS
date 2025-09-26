# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

# Flags untuk Cortex-M4
CPU = cortex-m4
CFLAGS = -mcpu=$(CPU) -mthumb -Wall -O2 -nostdlib -ffreestanding \
         -fno-common -ffunction-sections -fdata-sections \
         -Iinclude -I. -Ikernel -Iarch/arm -Imm -Idrivers -Ilib
ASFLAGS = -mcpu=$(CPU) -mthumb
LDFLAGS = -T boot/linker.ld -nostdlib -nostartfiles \
          --specs=nosys.specs -Wl,--gc-sections

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

OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)

TARGET = ukernelos

all: $(TARGET).bin $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(OBJDUMP) -S $@ > $@.disasm

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).elf.disasm

.PHONY: all clean
