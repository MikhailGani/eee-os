#ifndef GANIOS_DRIVERS_KBD_H
#define GANIOS_DRIVERS_KBD_H

#include <stdbool.h>
#include <stddef.h>

void kbd_init(void);
bool kbd_try_read(char *out_char);
void kbd_read_line(char *buf, size_t buf_len);

#endif
