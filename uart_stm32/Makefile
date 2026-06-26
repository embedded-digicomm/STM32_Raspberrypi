TARGET := uart1_polling
BUILD_DIR := build

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
SIZE := arm-none-eabi-size

CPU_FLAGS := -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
CFLAGS := $(CPU_FLAGS) -std=c11 -Wall -Wextra -Werror -Os -ffreestanding -fdata-sections -ffunction-sections
LDFLAGS := $(CPU_FLAGS) -T linker.ld -nostdlib -Wl,--gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map

SRCS := startup_stm32f427xx.s main.c usart1_driver.c
OBJS := $(addprefix $(BUILD_DIR)/,$(SRCS:=.o))

.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.c.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.s.o: %.s | $(BUILD_DIR)
	$(CC) $(CPU_FLAGS) -x assembler-with-cpp -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD_DIR)
