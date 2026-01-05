# GaniOS (eee-os)

Minimal freestanding i686 kernel in C, booted via GRUB (Multiboot v1).
Targeting an Asus Eee PC 4G / Celeron M-class machine.

## Project layout
```
Makefile
linker.ld
iso/boot/grub/grub.cfg
src/
  arch/x86/
    boot.s
    io.h
  kern/
    kmain.c
    log.c log.h
    panic.c panic.h
  drivers/
    vga.c vga.h
    serial.c serial.h
```

## Build prerequisites
You need tools that can build a GRUB ISO:
- `grub-mkrescue` (Linux) or `i686-elf-grub-mkrescue` (macOS)
- `xorriso`
- `qemu-system-i386`
- a C toolchain that can build freestanding i686 code

On Linux (recommended for ISO building):
- Debian/Ubuntu (GRUB + ISO tooling): `sudo apt-get install grub-pc-bin xorriso qemu-system-x86 make`

Cross compiler (recommended):
- `i686-elf-gcc` + `i686-elf-ld` + `i686-elf-as`

If `make iso` fails with `Missing: grub-mkrescue`, install `grub-pc-bin`:
- Debian/Ubuntu: `sudo apt-get install grub-pc-bin`

On macOS, the Homebrew `i686-elf-grub` package provides `i686-elf-grub-mkrescue`.
Override detection if needed:
```
make iso GRUB_MKRESCUE=i686-elf-grub-mkrescue
```

## Build
- `make iso`

## Run (QEMU)
- `make run`
- `make run-headless` (serial console on stdio)
