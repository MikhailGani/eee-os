#include "serial.h"

#include <stdint.h>

#include "arch/x86/io.h"

static const uint16_t COM1 = 0x3F8;

void serial_init(void) {
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x01); // Divisor low (115200 baud)
    outb(COM1 + 1, 0x00); // Divisor high
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // Enable FIFO, clear, 14-byte threshold
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

bool serial_is_transmit_empty(void) {
    return (inb(COM1 + 5) & 0x20) != 0;
}

void serial_putc(char c) {
    if (c == '\n') {
        serial_putc('\r');
    }

    while (!serial_is_transmit_empty()) {
        __asm__ __volatile__("pause");
    }
    outb(COM1, (uint8_t)c);
}

void serial_print(const char *s) {
    for (; *s; s++) {
        serial_putc(*s);
    }
}
