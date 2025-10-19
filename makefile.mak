# ==============================================================
#   ARMC4-OS Build System for STM32 NUCLEO-F446RE
#   Updated: 2025-10-19
# ==============================================================

# --------------------------------------------------
#  Toolchain
# --------------------------------------------------
CC       = arm-none-eabi-gcc
AS       = arm-none-eabi-as
OBJCOPY  = arm-none-eabi-objcopy
OBJDUMP  = arm-none-eabi-objdump
SIZE     = arm-none-eabi-size

# --------------------------------------------------
#  Target Board
# --------------------------------------------------
BOARD     ?= nucleo-f446re
BOARD_DIR = boards/$(BOARD)

CPU       = cortex-m4
TARGET    = ARMC4-OS

# --------------------------------------------------
#  Directories
# --------------------------------------------------
SRC_DIRS  = boot kernel arch/arm mm drivers lib apps $(BOARD_DIR)
BUILD_DIR = build

# --------------------------------------------------
#  Flags
# --------------------------------------------------
CFLAGS  = -mcpu=$(CPU) -mthumb -Wall -O2 -ffreestanding -fno-common \
          -ffunction-sections -fdata-sections -MMD -MP \
          -Iinclude -I. -Ikernel -Iarch -Iarch/arm -Imm -Idrivers -Ilib -Iboards/$(BOARD)

ASFLAGS = -mcpu=$(CPU) -mthumb

LDFLAGS = -T boot/linker.ld \
          -nostartfiles -nostdlib --specs=nosys.specs \
          -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

# --------------------------------------------------
#  Source files
# --------------------------------------------------
SRCS = \
    boot/boot.c \
    boot/startup_stm32f446re.s \
    kernel/kernel.c \
    kernel/scheduler.c \
    kernel/task.c \
    kernel/ipc.c \
    arch/arm/handlers.c \
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
    drivers/i2c.c \
    drivers/oled.c \
    lib/string.c \
    lib/printf.c \
    lib/assert.c \
    apps/main.c \
    $(BOARD_DIR)/board_init.c

# --------------------------------------------------
#  Build object list
# --------------------------------------------------
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# --------------------------------------------------
#  Default rule
# --------------------------------------------------
all: prepare $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	@echo "------------------------------------------------------"
	@$(SIZE) $(BUILD_DIR)/$(TARGET).elf
	@echo "✅ Build complete for $(BOARD)!"

# --------------------------------------------------
#  Build rules
# --------------------------------------------------
prepare:
	@mkdir -p $(dir $(OBJS))
	@mkdir -p $(BUILD_DIR)

# Compile C sources
$(BUILD_DIR)/%.c.o: %.c
	@echo "🧩 CC $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Assemble ASM sources
$(BUILD_DIR)/%.s.o: %.s
	@echo "⚙️  AS $<"
	@$(AS) $(ASFLAGS) -o $@ $<

# Link final ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo "🔗 LD $@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	@$(OBJDUMP) -S $@ > $(BUILD_DIR)/$(TARGET).disasm

# Convert ELF → BIN
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	@echo "📦 OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@

# --------------------------------------------------
#  Flash to STM32 (via ST-Link)
# --------------------------------------------------
flash: all
	@echo "🚀 Flashing $(TARGET).bin to board..."
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000

# --------------------------------------------------
#  Clean
# --------------------------------------------------
clean:
	@echo "🧹 Cleaning build directory..."
	rm -rf $(BUILD_DIR)

# --------------------------------------------------
#  Include dependencies
# --------------------------------------------------
-include $(DEPS)

.PHONY: all clean flash prepare
