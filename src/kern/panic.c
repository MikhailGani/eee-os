#include "panic.h"

#include "log.h"

void panic(const char *msg) {
    kputs("PANIC: ");
    kputs(msg);
    kputs("\n");

    __asm__ __volatile__("cli");
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
