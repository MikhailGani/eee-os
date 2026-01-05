#ifndef GANIOS_KERN_PANIC_H
#define GANIOS_KERN_PANIC_H

void panic(const char *msg);

#define ASSERT(x) \
    do { \
        if (!(x)) { \
            panic("ASSERT: " #x); \
        } \
    } while (0)

#endif
