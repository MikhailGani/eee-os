#ifndef SERIAL_H
#define SERIAL_H

#include <stdbool.h>

void serial_init(void);
bool serial_is_transmit_empty(void);
void serial_putc(char c);
void serial_print(const char *s);

#endif
