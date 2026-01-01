#include "panic.h"

#include "console.h"

void panic(const char *msg) {
    console_print("PANIC: ");
    console_print(msg);
    console_print("\n");

    __asm__ __volatile__("cli");
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
