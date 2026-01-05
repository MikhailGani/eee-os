#include "arch/x86/isr.h"

#include <stdint.h>

#include "kern/log.h"

static const char *const exc_names[32] = {
    "Divide Error",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    "Reserved"
};

static void print_hex32(uint32_t value) {
    static const char hex_digits[] = "0123456789ABCDEF";
    for (int shift = 28; shift >= 0; shift -= 4) {
        uint8_t nibble = (uint8_t)((value >> shift) & 0xF);
        kputc(hex_digits[nibble]);
    }
}

static void print_dec(uint32_t value) {
    char buf[11];
    int pos = 0;

    if (value == 0) {
        kputc('0');
        return;
    }

    while (value > 0 && pos < (int)sizeof(buf)) {
        buf[pos++] = (char)('0' + (value % 10));
        value /= 10;
    }

    while (pos-- > 0) {
        kputc(buf[pos]);
    }
}

static void halt_forever(void) __attribute__((noreturn));

static void halt_forever(void) {
    for (;;) {
        __asm__ __volatile__("cli; hlt");
    }
}

void isr_exception_handler(struct regs *r) {
    if (r->int_no == 8) {
        kputs("EXC 8 Double Fault\n");
        halt_forever();
    }

    kputs("EXC ");
    print_dec(r->int_no);
    kputs(" err=0x");
    print_hex32(r->err_code);

    if (r->int_no < 32) {
        kputs(" ");
        kputs(exc_names[r->int_no]);
    }

    kputs("\n");
    halt_forever();
}
