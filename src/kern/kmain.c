#include <stdint.h>

#include "arch/x86/idt.h"
#include "drivers/pic.h"
#include "drivers/pit.h"
#include "kern/log.h"
#include "kern/shell.h"

void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    (void)mb_magic;
    (void)mb_info_ptr;

    log_init();
    idt_init();
    pic_init();
    pit_init(100);

    kputs("GaniOS booted\n");

    __asm__ __volatile__("sti");

#ifdef KERNEL_TEST_TRAP
    __asm__ __volatile__("int3");
#endif

    shell_run();
}
