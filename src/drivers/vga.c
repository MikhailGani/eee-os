#include "vga.h"

#include <stddef.h>
#include <stdint.h>

static volatile uint16_t *const VGA = (uint16_t *)0xB8000;
static size_t vga_row = 0;
static size_t vga_col = 0;
static uint8_t vga_color = 0x0F; // white on black

static void vga_newline(void) {
    vga_col = 0;
    if (vga_row < 24) {
        vga_row++;
    }
}

void vga_clear(void) {
    for (size_t i = 0; i < 80 * 25; i++) {
        VGA[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }
    vga_row = 0;
    vga_col = 0;
}

void vga_putc(char c) {
    if (c == '\n') {
        vga_newline();
        return;
    }

    const size_t idx = vga_row * 80 + vga_col;
    VGA[idx] = (uint16_t)c | ((uint16_t)vga_color << 8);

    vga_col++;
    if (vga_col >= 80) {
        vga_newline();
    }
}

void vga_print(const char *s) {
    for (; *s; s++) {
        vga_putc(*s);
    }
}
