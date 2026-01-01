#include "shell.h"

#include "calc.h"
#include "console.h"
#include "kbd.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static char shell_line[128];
static size_t shell_len = 0;

static void shell_prompt(void) {
    console_print("eee> ");
}

static void print_int(int32_t value) {
    char buf[12];
    size_t i = 0;
    if (value == 0) {
        console_putc('0');
        return;
    }
    if (value < 0) {
        console_putc('-');
        value = -value;
    }
    while (value > 0 && i < sizeof(buf)) {
        buf[i++] = (char)('0' + (value % 10));
        value /= 10;
    }
    while (i > 0) {
        console_putc(buf[--i]);
    }
}

static const char *skip_spaces(const char *s) {
    while (*s == ' ' || *s == '\t') {
        s++;
    }
    return s;
}

static bool starts_with(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s != *prefix) {
            return false;
        }
        s++;
        prefix++;
    }
    return true;
}

static bool is_end_or_space(char c) {
    return c == '\0' || c == ' ' || c == '\t';
}

static void shell_help(void) {
    console_print("commands:\n");
    console_print("  help          - show this help\n");
    console_print("  calc <expr>   - evaluate expression\n");
    console_print("  clear         - clear the screen\n");
}

static void shell_execute(const char *line) {
    const char *p = skip_spaces(line);
    if (*p == '\0') {
        return;
    }

    if (starts_with(p, "help") && is_end_or_space(p[4])) {
        shell_help();
        return;
    }

    if (starts_with(p, "clear") && is_end_or_space(p[5])) {
        console_clear();
        return;
    }

    if (starts_with(p, "calc") && is_end_or_space(p[4])) {
        p = skip_spaces(p + 4);
        if (*p == '\0') {
            console_print("error: invalid token\n");
            return;
        }
        int32_t result = 0;
        const char *error = NULL;
        if (calc_eval(p, &result, &error) != 0) {
            console_print(error ? error : "error: invalid token");
            console_putc('\n');
            return;
        }
        print_int(result);
        console_putc('\n');
        return;
    }

    console_print("error: invalid token\n");
}

void shell_init(void) {
    shell_len = 0;
    shell_prompt();
}

void shell_poll(void) {
    int ch = 0;
#ifdef KBD_POLL
    while ((ch = kbd_poll_getchar()) >= 0) {
#else
    while ((ch = kbd_getchar_nonblock()) >= 0) {
#endif
        if (ch == '\n') {
            console_putc('\n');
            shell_line[shell_len] = '\0';
            shell_execute(shell_line);
            shell_len = 0;
            shell_prompt();
            continue;
        }
        if (ch == '\b') {
            if (shell_len > 0) {
                shell_len--;
                console_putc('\b');
            }
            continue;
        }
        if (shell_len + 1 < sizeof(shell_line)) {
            shell_line[shell_len++] = (char)ch;
            console_putc((char)ch);
        }
    }
}
