#include "drivers/pit.h"

#include "arch/x86/io.h"

#define PIT_CHANNEL0_DATA 0x40
#define PIT_COMMAND 0x43
#define PIT_BASE_FREQUENCY 1193182u

static volatile uint64_t pit_ticks = 0;
static uint32_t pit_hz = 100;

void pit_init(uint32_t hz) {
    if (hz == 0) {
        hz = 100;
    }

    uint32_t divisor = PIT_BASE_FREQUENCY / hz;
    if (divisor == 0) {
        divisor = 1;
    }
    if (divisor > 0xFFFF) {
        divisor = 0xFFFF;
    }

    pit_hz = PIT_BASE_FREQUENCY / divisor;
    pit_ticks = 0;

    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0_DATA, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_DATA, (uint8_t)((divisor >> 8) & 0xFF));
}

void pit_handle_irq0(void) {
    pit_ticks++;
}

uint64_t pit_get_ticks(void) {
    return pit_ticks;
}

uint32_t pit_get_hz(void) {
    return pit_hz;
}

uint64_t uptime_ms(void) {
    uint64_t ticks = pit_get_ticks();
    uint32_t hz = pit_get_hz();
    if (hz == 0) {
        return 0;
    }
    return (ticks * 1000u) / hz;
}

void sleep_ms(uint32_t ms) {
    uint64_t start = pit_get_ticks();
    uint64_t target = start + ((uint64_t)ms * pit_get_hz() + 999u) / 1000u;

    while (pit_get_ticks() < target) {
        __asm__ __volatile__("pause");
    }
}
