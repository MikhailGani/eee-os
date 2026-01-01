#include <stdint.h>
#include <stddef.h>

#include "serial.h"   // should exist in the PR (ticket with serial output)

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
static uint8_t vga_color = 0x0F; // white on black
static size_t vga_row = 0;
static size_t vga_col = 0;

static void vga_clear(void) {
    for (size_t i = 0; i < 80 * 25; i++) {
        VGA[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }
    vga_row = 0;
    vga_col = 0;
}

static void vga_putc(char c) {
    if (c == '\n') {
        vga_col = 0;
        if (vga_row < 24) vga_row++;
        return;
    }
    const size_t idx = vga_row * 80 + vga_col;
    VGA[idx] = (uint16_t)c | ((uint16_t)vga_color << 8);

    vga_col++;
    if (vga_col >= 80) {
        vga_col = 0;
        if (vga_row < 24) vga_row++;
    }
}

static void vga_print(const char* s) {
    for (; *s; s++) vga_putc(*s);
}

void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    (void)mb_magic;
    (void)mb_info_ptr;

    vga_clear();

    serial_init();

    vga_print("EEE-OS booted\n");
    serial_print("EEE-OS booted\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
