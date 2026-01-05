#include <stdint.h>

#include "log.h"

void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    (void)mb_magic;
    (void)mb_info_ptr;

    log_init();
    kputs("GaniOS booted\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
