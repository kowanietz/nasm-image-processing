ASM = nasm
ASMFLAGS = -f elf64

CC = gcc
CFLAGS = -Wall -Iinclude

SRC_DIR = src
ASM_DIR = asm
BUILD_DIR = build

C_SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/bmp.c
ASM_SRCS = $(ASM_DIR)/filters/invert.asm

C_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRCS))
ASM_OBJS = $(patsubst $(ASM_DIR)/filters/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))

TARGET = $(BUILD_DIR)/main




all: $(TARGET)

$(TARGET): $(C_OBJS) $(ASM_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(ASM_DIR)/filters/%.asm | $(BUILD_DIR)
	$(ASM) $(ASMFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*
