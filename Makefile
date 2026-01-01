# eee-os Makefile (i686 + GRUB Multiboot v1)

PREFIX ?=
CC      := $(PREFIX)i686-elf-gcc
AS      := $(PREFIX)i686-elf-as
LD      := $(PREFIX)i686-elf-ld

# On Linux this is usually `grub-mkrescue`
# On macOS (Homebrew i686-elf-grub) it is often `i686-elf-grub-mkrescue`
GRUB_MKRESCUE ?= $(shell command -v grub-mkrescue 2>/dev/null || command -v i686-elf-grub-mkrescue 2>/dev/null)

CFLAGS  := -std=c11 -ffreestanding -O2 -Wall -Wextra -Werror \
           -fno-stack-protector -fno-pic -fno-pie -m32
LDFLAGS := -T linker.ld -nostdlib

ifdef TEST_TRAP
CFLAGS += -DKERNEL_TEST_TRAP
endif

BUILD   := build
ISO_DIR := iso
KERNEL  := $(BUILD)/kernel.elf
ISO     := $(BUILD)/eee-os.iso

# Build everything in src/ (prevents future merge pain)
SRC_C   := $(wildcard src/*.c)
SRC_S   := $(wildcard src/*.s)
OBJ_C   := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC_C))
OBJ_S   := $(patsubst src/%.s,$(BUILD)/%.o,$(SRC_S))

.PHONY: all iso run run-headless clean check-tools

all: iso

check-tools:
	@command -v "$(GRUB_MKRESCUE)" >/dev/null || (echo "Missing: grub-mkrescue (Linux) or i686-elf-grub-mkrescue (macOS Homebrew i686-elf-grub). Install grub-pc-bin + xorriso (Ubuntu/Debian) or i686-elf-grub (Homebrew)." && exit 1)
	@command -v xorriso >/dev/null || (echo "Missing: xorriso (Ubuntu/Debian: apt-get install xorriso)" && exit 1)
	@command -v qemu-system-i386 >/dev/null || (echo "Missing: qemu-system-i386 (Ubuntu/Debian: apt-get install qemu-system-x86)" && exit 1)

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
	"$(GRUB_MKRESCUE)" -o $(ISO) $(ISO_DIR) >/dev/null
	@echo "Built ISO: $(ISO)"

run: iso
	qemu-system-i386 -cdrom $(ISO) -m 256M -no-reboot -no-shutdown

run-headless: iso
	qemu-system-i386 -cdrom $(ISO) -m 256M -nographic -serial mon:stdio -no-reboot -no-shutdown

clean:
	rm -rf $(BUILD) $(ISO_DIR)/boot/kernel.elf
