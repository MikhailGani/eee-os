#ifndef PANIC_H
#define PANIC_H

void panic(const char *msg);

#define ASSERT(cond, msg)            \
    do {                             \
        if (!(cond)) {               \
            panic(msg);              \
        }                            \
    } while (0)

#endif
