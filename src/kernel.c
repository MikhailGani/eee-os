#include <stdint.h>
#include <stddef.h>

#include "console.h"
#include "idt.h"

// Multiboot passes eax=magic, ebx=mb_info_ptr. We don't use them yet.
void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    (void)mb_magic;
    (void)mb_info_ptr;

    console_init();
    console_clear();

    // Install IDT and exception handlers (0..31)
    idt_init();

    console_print("EEE-OS booted\n");

#ifdef KERNEL_TEST_TRAP
    // A simple test: trigger an exception to verify panic output.
    // int3 -> breakpoint exception (vector 3)
    __asm__ __volatile__("int3");
#endif

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
