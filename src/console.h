#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdbool.h>

void console_init(void);
void console_clear(void);
void console_putc(char c);
void console_print(const char *s);
void console_set_serial_enabled(bool enabled);

#endif
