#include "console.h"

#include "serial.h"

#include <stddef.h>
#include <stdint.h>

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;
static size_t vga_row = 0;
static size_t vga_col = 0;
static uint8_t vga_color = 0x0F; // white on black
static bool serial_enabled = false;

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
    console_set_serial_enabled(true);
    console_clear();
}

void console_clear(void) {
    for (size_t i = 0; i < 80 * 25; i++) {
        VGA[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }
    vga_row = 0;
    vga_col = 0;
}

void console_set_serial_enabled(bool enabled) {
    serial_enabled = enabled;
}

void console_putc(char c) {
    if (c == '\b') {
        if (vga_col == 0 && vga_row == 0) {
            return;
        }
        if (vga_col == 0) {
            vga_row--;
            vga_col = 79;
        } else {
            vga_col--;
        }
        const size_t idx = vga_row * 80 + vga_col;
        VGA[idx] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
        if (serial_enabled) {
            serial_putc('\b');
            serial_putc(' ');
            serial_putc('\b');
        }
        return;
    }

    if (c == '\n') {
        vga_col = 0;
        vga_row++;
        vga_scroll();
        if (serial_enabled) {
            serial_putc(c);
        }
        return;
    }

    const size_t idx = vga_row * 80 + vga_col;
    VGA[idx] = (uint16_t)c | ((uint16_t)vga_color << 8);
    if (serial_enabled) {
        serial_putc(c);
    }

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
