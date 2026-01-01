# eee-os Makefile (i686 + GRUB Multiboot v1)

PREFIX ?=
CC      := $(PREFIX)i686-elf-gcc
AS      := $(PREFIX)i686-elf-as
LD      := $(PREFIX)i686-elf-ld

TEST_TRAP ?= 0

CFLAGS  := -std=c11 -ffreestanding -O2 -Wall -Wextra -Werror \
           -fno-stack-protector -fno-pic -fno-pie -m32 \
           -DKERNEL_TEST_TRAP=$(TEST_TRAP)
LDFLAGS := -T linker.ld -nostdlib

BUILD   := build
ISO_DIR := iso
KERNEL  := $(BUILD)/kernel.elf
ISO     := $(BUILD)/eee-os.iso

SRC_C   := src/kernel.c \
           src/console.c \
           src/panic.c \
           src/idt.c
SRC_S   := src/boot.s \
           src/isr.s
OBJ_C   := $(SRC_C:src/%.c=$(BUILD)/%.o)
OBJ_S   := $(SRC_S:src/%.s=$(BUILD)/%.o)

.PHONY: all iso run run-headless clean check-tools

all: iso

check-tools:
	@command -v grub-mkrescue >/dev/null || (echo "Missing: grub-mkrescue" && exit 1)
	@command -v xorriso >/dev/null || (echo "Missing: xorriso" && exit 1)
	@command -v qemu-system-i386 >/dev/null || (echo "Missing: qemu-system-i386" && exit 1)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.s | $(BUILD)
	$(AS) --32 $< -o $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJ_S) $(OBJ_C)
	$(LD) $(LDFLAGS) -o $@ $^

iso: check-tools $(KERNEL)
	# Place kernel where grub.cfg expects it
	mkdir -p $(ISO_DIR)/boot
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.elf
	mkdir -p $(BUILD)
	grub-mkrescue -o $(ISO) $(ISO_DIR) >/dev/null
	@echo "Built ISO: $(ISO)"

run: iso
	qemu-system-i386 -cdrom $(ISO) -m 256M -no-reboot -no-shutdown

run-headless: iso
	qemu-system-i386 -cdrom $(ISO) -m 256M -nographic -serial mon:stdio -no-reboot -no-shutdown

clean:
	rm -rf $(BUILD) $(ISO_DIR)/boot/kernel.elf
