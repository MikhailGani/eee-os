#include "console.h"

#include <stddef.h>
#include <stdint.h>

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;
static size_t vga_row = 0;
static size_t vga_col = 0;
static uint8_t vga_color = 0x0F; // white on black

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static void serial_init(void) {
    const uint16_t com1 = 0x3F8;
    outb(com1 + 1, 0x00); // Disable interrupts
    outb(com1 + 3, 0x80); // Enable DLAB
    outb(com1 + 0, 0x01); // Divisor low (115200 baud)
    outb(com1 + 1, 0x00); // Divisor high
    outb(com1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(com1 + 2, 0xC7); // Enable FIFO, clear, 14-byte threshold
    outb(com1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
}

static void serial_putc(char c) {
    const uint16_t com1 = 0x3F8;
    while ((inb(com1 + 5) & 0x20) == 0) {
        __asm__ __volatile__("pause");
    }
    outb(com1, (uint8_t)c);
}

static void vga_scroll(void) {
    if (vga_row < 25) {
        return;
    }

    for (size_t row = 1; row < 25; row++) {
        for (size_t col = 0; col < 80; col++) {
            VGA[(row - 1) * 80 + col] = VGA[row * 80 + col];
        }
    }

    for (size_t col = 0; col < 80; col++) {
        VGA[(25 - 1) * 80 + col] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }

    vga_row = 24;
}

void console_init(void) {
    serial_init();
    console_clear();
}

void console_clear(void) {
    for (size_t i = 0; i < 80 * 25; i++) {
        VGA[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }
    vga_row = 0;
    vga_col = 0;
}

void console_putc(char c) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
        vga_scroll();
        serial_putc(c);
        return;
    }

    const size_t idx = vga_row * 80 + vga_col;
    VGA[idx] = (uint16_t)c | ((uint16_t)vga_color << 8);
    serial_putc(c);

    vga_col++;
    if (vga_col >= 80) {
        vga_col = 0;
        vga_row++;
        vga_scroll();
    }
}

void console_print(const char *s) {
    for (; *s; s++) {
        console_putc(*s);
    }
}
