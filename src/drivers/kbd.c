#include "drivers/kbd.h"

#include <stdint.h>

#include "arch/x86/io.h"
#include "kern/log.h"

#define KBD_STATUS_PORT 0x64
#define KBD_DATA_PORT 0x60

#define KBD_STATUS_OBF 0x01

static const char scancode_table[128] = {
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    [0x39] = ' ',
};

void kbd_init(void) {
}

bool kbd_try_read(char *out_char) {
    uint8_t status = inb(KBD_STATUS_PORT);
    if ((status & KBD_STATUS_OBF) == 0) {
        return false;
    }

    uint8_t scancode = inb(KBD_DATA_PORT);
    if (scancode & 0x80) {
        return false;
    }

    if (scancode == 0x0E) {
        *out_char = '\b';
        return true;
    }
    if (scancode == 0x1C) {
        *out_char = '\n';
        return true;
    }

    if (scancode < 128) {
        char mapped = scancode_table[scancode];
        if (mapped != 0) {
            *out_char = mapped;
            return true;
        }
    }

    return false;
}

void kbd_read_line(char *buf, size_t buf_len) {
    if (buf_len == 0) {
        return;
    }

    size_t idx = 0;
    for (;;) {
        char c = 0;
        if (!kbd_try_read(&c)) {
            __asm__ __volatile__("pause");
            continue;
        }

        if (c == '\n') {
            kputc('\n');
            break;
        }

        if (c == '\b') {
            if (idx > 0) {
                idx--;
                kputc('\b');
            }
            continue;
        }

        if (idx + 1 < buf_len) {
            buf[idx++] = c;
            kputc(c);
        }
    }

    buf[idx] = '\0';
}
