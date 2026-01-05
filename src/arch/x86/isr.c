#include "arch/x86/isr.h"

#include <stdint.h>

#include "kern/log.h"

static void print_hex32(uint32_t value) {
    static const char hex_digits[] = "0123456789ABCDEF";
    for (int shift = 28; shift >= 0; shift -= 4) {
        uint8_t nibble = (uint8_t)((value >> shift) & 0xF);
        kputc(hex_digits[nibble]);
    }
}

static void halt_forever(void) __attribute__((noreturn));

static void halt_forever(void) {
    for (;;) {
        __asm__ __volatile__("cli; hlt");
    }
}

void isr_exception_handler(struct regs *r) {
    /* Minimal handler to avoid faults during exception processing. */
    /* PR branch implementation retained during conflict resolution. */
    if (r->int_no == 8) {
        kputs("EXC 8 err=0x");
        print_hex32(r->err_code);
        kputs("\n");
        halt_forever();
    }

    kputs("EXC 0x");
    print_hex32(r->int_no);
    kputs(" err=0x");
    print_hex32(r->err_code);
    kputs("\n");
    halt_forever();
}
