#ifndef GANIOS_ARCH_X86_IRQ_H
#define GANIOS_ARCH_X86_IRQ_H

#include "arch/x86/isr.h"

void irq0(void);
void irq_handler(struct regs *r);

#endif
