# eee-os

Minimal freestanding i686 kernel in C, booted via GRUB (Multiboot v1).
Targeting an Asus Eee PC 4G / Celeron M-class machine.

## Build prerequisites
You need tools that can build a GRUB ISO:
- `grub-mkrescue`
- `xorriso`
- `qemu-system-i386`
- a C toolchain that can build freestanding i686 code

On Linux (recommended for ISO building):
- Debian/Ubuntu: `sudo apt-get install grub-pc-bin xorriso qemu-system-x86 make`

Cross compiler (recommended):
- `i686-elf-gcc` + `i686-elf-ld` + `i686-elf-as`

## Build
- `make iso`

## Run (QEMU)
- `make run`

If you're on macOS and don't have GRUB utilities locally, you can still use the repo
and build the ISO in a Linux environment (VM, container, or Codex cloud environment),
then pull the resulting artifact.
