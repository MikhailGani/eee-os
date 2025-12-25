#include <stdint.h>
#include <stddef.h>

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
static size_t vga_row = 0;
static size_t vga_col = 0;
static uint8_t vga_color = 0x0F; // white on black

static void vga_putc(char c) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
        return;
    }
    const size_t idx = vga_row * 80 + vga_col;
    VGA[idx] = (uint16_t)c | ((uint16_t)vga_color << 8);
    vga_col++;
    if (vga_col >= 80) {
        vga_col = 0;
        vga_row++;
    }
}

static void vga_print(const char* s) {
    for (; *s; s++) vga_putc(*s);
}

static void vga_print_hex(uint32_t x) {
    const char* hex = "0123456789ABCDEF";
    vga_print("0x");
    for (int i = 7; i >= 0; --i) {
        uint8_t nib = (x >> (i * 4)) & 0xF;
        vga_putc(hex[nib]);
    }
}

void kmain(uint32_t mb_magic, uint32_t mb_info_ptr) {
    // Clear screen
    for (size_t i = 0; i < 80 * 25; i++) {
        VGA[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
    }
    vga_row = 0;
    vga_col = 0;

    vga_print("EEE-OS booted\n");
    vga_print("multiboot magic: ");
    vga_print_hex(mb_magic);
    vga_print("\nmultiboot info:  ");
    vga_print_hex(mb_info_ptr);
    vga_print("\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
