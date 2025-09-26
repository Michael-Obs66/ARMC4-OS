# Toolchain
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# Flags untuk Cortex-M4
CPU = cortex-m4
CFLAGS = -mcpu=$(CPU) -mthumb -Wall -O2 -nostdlib -ffreestanding \
         -fno-common -ffunction-sections -fdata-sections
ASFLAGS = -mcpu=$(CPU) -mthumb
LDFLAGS = -T boot/linker.ld -nostdlib -nostartfiles \
          --specs=nosys.specs -Wl,--gc-sections

# QEMU configuration
QEMU = qemu-system-arm
QEMU_MACHINE = netduinoplus2
QEMU_CPU = cortex-m4
QEMU_MEM = 128M

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

# Targets
TARGET = ukernelos

all: $(TARGET).bin $(TARGET).elf
	$(SIZE) $(TARGET).elf

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(OBJDUMP) -S $@ > $@.disasm

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<

# QEMU Targets
qemu: $(TARGET).elf
	$(QEMU) -machine $(QEMU_MACHINE) -cpu $(QEMU_CPU) \
	        -kernel $(TARGET).elf -nographic -serial mon:stdio \
	        -m $(QEMU_MEM)

qemu-debug: $(TARGET).elf
	$(QEMU) -machine $(QEMU_MACHINE) -cpu $(QEMU_CPU) \
	        -kernel $(TARGET).elf -nographic -serial mon:stdio \
	        -m $(QEMU_MEM) -s -S

qemu-gdb: $(TARGET).elf
	$(QEMU) -machine $(QEMU_MACHINE) -cpu $(QEMU_CPU) \
	        -kernel $(TARGET).elf -nographic -serial mon:stdio \
	        -m $(QEMU_MEM) -s -S &

qemu-test: $(TARGET).elf
	echo "Starting automated tests in QEMU..."
	$(QEMU) -machine $(QEMU_MACHINE) -cpu $(QEMU_CPU) \
	        -kernel $(TARGET).elf -nographic -serial stdio \
	        -m $(QEMU_MEM) | tee test_output.log

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).bin $(TARGET).elf.disasm

.PHONY: all clean qemu qemu-debug qemu-gdb qemu-test
