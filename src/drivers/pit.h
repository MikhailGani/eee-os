#ifndef GANIOS_DRIVERS_PIT_H
#define GANIOS_DRIVERS_PIT_H

#include <stdint.h>

void pit_init(uint32_t hz);
void pit_handle_irq0(void);
uint64_t pit_get_ticks(void);
uint32_t pit_get_hz(void);
uint64_t uptime_ms(void);
void sleep_ms(uint32_t ms);

#endif
