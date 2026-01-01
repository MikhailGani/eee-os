#include <stdint.h>
#include <stddef.h>

static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
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

static void serial_print(const char* s) {
    for (; *s; s++) serial_putc(*s);
}

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
    serial_init();

    vga_print("EEE-OS booted\n");
    serial_print("EEE-OS booted\n");
    vga_print("multiboot magic: ");
    vga_print_hex(mb_magic);
    vga_print("\nmultiboot info:  ");
    vga_print_hex(mb_info_ptr);
    vga_print("\n");

    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
