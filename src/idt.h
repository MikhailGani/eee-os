#ifndef IDT_H
#define IDT_H

#include <stdint.h>

void idt_init(void);
void isr_handler(uint32_t vector, uint32_t error_code);

#endif
