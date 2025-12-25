# Project: eee-os (Asus Eee PC 4G / Intel Celeron M)

Target hardware
- CPU: Intel Celeron M (32-bit i686), single-core
- Boot: Legacy BIOS + GRUB (Multiboot v1)
- RAM: 1024 MiB installed (DO NOT hardcode; later read Multiboot memory map)
- Display: VGA text mode for early boot

Tooling (typical)
- Cross-compiler: i686-elf-gcc (freestanding)
- Emulator: QEMU i386 (for dev)

Commands
- make iso      -> builds bootable ISO at build/eee-os.iso
- make run      -> runs ISO in QEMU
- make clean

Definition of Done (v0)
- `make run` boots and prints "EEE-OS booted" in VGA text mode, then halts.

Rules for agents
- Keep changes small and explain diffs.
- Always show commands run + output.
- Never introduce host-only headers; use freestanding C only.
