#include <stdint.h>

#include "console.h"
#include "idt.h"

void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    (void)mb_magic;
    (void)mb_info_ptr;

    console_init();
    idt_init();

    console_print("EEE-OS booted\n");
    console_print("Press Ctrl+Alt+Del to reboot\n");

#if KERNEL_TEST_TRAP
    __asm__ __volatile__("int3");
#endif

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
