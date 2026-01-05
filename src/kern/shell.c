#include "kern/shell.h"

#include <stddef.h>
#include <stdint.h>

#include "drivers/kbd.h"
#include "drivers/pit.h"
#include "drivers/vga.h"
#include "kern/log.h"
#include "kern/panic.h"

static void kput_u64(uint64_t value) {
    char buf[21];
    size_t pos = 0;

    if (value == 0) {
        kputc('0');
        return;
    }

    while (value > 0 && pos < sizeof(buf)) {
        buf[pos++] = (char)('0' + (value % 10));
        value /= 10;
    }

    while (pos-- > 0) {
        kputc(buf[pos]);
    }
}

static int str_eq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

static void shell_help(void) {
    kputs("Commands:\n");
    kputs("  help   - show this list\n");
    kputs("  uptime - show milliseconds since boot\n");
    kputs("  panic <msg> - trigger kernel panic\n");
    kputs("  clear  - clear the screen\n");
}

static void shell_uptime(void) {
    uint64_t ms = uptime_ms();
    kputs("uptime ");
    kput_u64(ms);
    kputs(" ms\n");
}

static void shell_clear(void) {
    vga_clear();
}

static void shell_panic(const char *msg) {
    if (msg == NULL || *msg == '\0') {
        panic("panic");
    }
    panic(msg);
}

void shell_run(void) {
    char line[128];

    kbd_init();

    for (;;) {
        kputs("ganios> ");
        kbd_read_line(line, sizeof(line));

        if (str_eq(line, "help")) {
            shell_help();
            continue;
        }
        if (str_eq(line, "uptime")) {
            shell_uptime();
            continue;
        }
        if (str_eq(line, "clear")) {
            shell_clear();
            continue;
        }

        const char *prefix = "panic ";
        size_t i = 0;
        while (prefix[i] && line[i] == prefix[i]) {
            i++;
        }
        if (prefix[i] == '\0') {
            shell_panic(line + i);
            continue;
        }

        if (line[0] != '\0') {
            kputs("Unknown command: ");
            kputs(line);
            kputs("\n");
        }
    }
}
