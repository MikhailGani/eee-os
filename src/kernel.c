#include <stdint.h>
#include <stddef.h>

#include "console.h"
#include "idt.h"
#include "kbd.h"
#include "pic.h"
#include "shell.h"

// Multiboot passes eax=magic, ebx=mb_info_ptr. We don't use them yet.
void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    (void)mb_magic;
    (void)mb_info_ptr;

    console_init();
    console_clear();

    // Install IDT and exception handlers (0..31)
    idt_init();
    pic_init();
    kbd_init();

    console_print("EEE-OS booted\n");

    __asm__ __volatile__("sti");
    shell_init();

#ifdef KERNEL_TEST_TRAP
    // A simple test: trigger an exception to verify panic output.
    // int3 -> breakpoint exception (vector 3)
    __asm__ __volatile__("int3");
#endif

    for (;;) {
        shell_poll();
        __asm__ __volatile__("hlt");
    }
}
