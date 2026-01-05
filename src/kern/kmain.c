#include <stdint.h>

#include "arch/x86/idt.h"
#include "log.h"

void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    (void)mb_magic;
    (void)mb_info_ptr;

    log_init();
    idt_init();

    kputs("GaniOS booted\n");

#ifdef KERNEL_TEST_TRAP
    __asm__ __volatile__("int3");
#endif

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
