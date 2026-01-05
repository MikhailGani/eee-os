#include "log.h"

#include "drivers/serial.h"
#include "drivers/vga.h"

void log_init(void) {
    vga_clear();
    serial_init();
}

void kputc(char c) {
    vga_putc(c);
    serial_putc(c);
}

void kputs(const char *s) {
    for (; *s; s++) {
        kputc(*s);
    }
}
