#ifndef GANIOS_ARCH_X86_IDT_H
#define GANIOS_ARCH_X86_IDT_H

#include <stdint.h>

void idt_set_gate(uint8_t vec, void (*handler)(void), uint16_t selector, uint8_t flags);
void idt_load(void);
void idt_init(void);

#endif
