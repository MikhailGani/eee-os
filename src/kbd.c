#include "kbd.h"

#include "io.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

static char kbd_buffer[128];
static volatile size_t kbd_head = 0;
static volatile size_t kbd_tail = 0;
static bool kbd_shift = false;

static const char kbd_map[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '8',
    [0x0A] = '9', [0x0B] = '0', [0x0C] = '-', [0x0D] = '+',
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r',
    [0x14] = 't', [0x15] = 'y', [0x16] = 'u', [0x17] = 'i',
    [0x18] = 'o', [0x19] = 'p', [0x1E] = 'a', [0x1F] = 's',
    [0x20] = 'd', [0x21] = 'f', [0x22] = 'g', [0x23] = 'h',
    [0x24] = 'j', [0x25] = 'k', [0x26] = 'l', [0x2C] = 'z',
    [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v', [0x30] = 'b',
    [0x31] = 'n', [0x32] = 'm', [0x33] = ',', [0x34] = '.',
    [0x35] = '/', [0x37] = '*', [0x39] = ' ',
    [0x1C] = '\n', [0x0E] = '\b'
};

static const char kbd_shift_map[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4',
    [0x06] = '5', [0x07] = '6', [0x08] = '7', [0x09] = '*',
    [0x0A] = '(', [0x0B] = ')', [0x0C] = '-', [0x0D] = '+',
    [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R',
    [0x14] = 'T', [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I',
    [0x18] = 'O', [0x19] = 'P', [0x1E] = 'A', [0x1F] = 'S',
    [0x20] = 'D', [0x21] = 'F', [0x22] = 'G', [0x23] = 'H',
    [0x24] = 'J', [0x25] = 'K', [0x26] = 'L', [0x2C] = 'Z',
    [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B',
    [0x31] = 'N', [0x32] = 'M', [0x33] = ',', [0x34] = '.',
    [0x35] = '/', [0x37] = '*', [0x39] = ' ',
    [0x1C] = '\n', [0x0E] = '\b'
};

void kbd_init(void) {
    kbd_head = 0;
    kbd_tail = 0;
    kbd_shift = false;
}

static void kbd_buffer_push(char c) {
    size_t next = (kbd_head + 1) % sizeof(kbd_buffer);
    if (next == kbd_tail) {
        return;
    }
    kbd_buffer[kbd_head] = c;
    kbd_head = next;
}

static char kbd_translate(uint8_t scancode) {
    if (scancode == 0x2A || scancode == 0x36) {
        kbd_shift = true;
        return 0;
    }
    if (scancode == 0xAA || scancode == 0xB6) {
        kbd_shift = false;
        return 0;
    }
    if (scancode & 0x80) {
        return 0;
    }
    if (kbd_shift) {
        return kbd_shift_map[scancode];
    }
    return kbd_map[scancode];
}

void kbd_handle_irq(void) {
    uint8_t status = inb(KBD_STATUS_PORT);
    if ((status & 0x01) == 0) {
        return;
    }
    uint8_t scancode = inb(KBD_DATA_PORT);
    char ch = kbd_translate(scancode);
    if (ch) {
        kbd_buffer_push(ch);
    }
}

int kbd_poll_getchar(void) {
    uint8_t status = inb(KBD_STATUS_PORT);
    if ((status & 0x01) == 0) {
        return -1;
    }
    uint8_t scancode = inb(KBD_DATA_PORT);
    char ch = kbd_translate(scancode);
    if (!ch) {
        return -1;
    }
    return (int)(unsigned char)ch;
}

int kbd_getchar_nonblock(void) {
    if (kbd_head == kbd_tail) {
        return -1;
    }
    char ch = kbd_buffer[kbd_tail];
    kbd_tail = (kbd_tail + 1) % sizeof(kbd_buffer);
    return (int)(unsigned char)ch;
}
