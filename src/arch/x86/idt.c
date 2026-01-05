#include "arch/x86/idt.h"

#include <stdint.h>

#include "arch/x86/isr.h"
#include "arch/x86/irq.h"

#define IDT_ENTRIES 256
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_INT_GATE 0x0E

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr idt_desc;

void idt_set_gate(uint8_t vec, void (*handler)(void), uint16_t selector, uint8_t flags) {
    uint32_t base = (uint32_t)handler;

    idt[vec].base_low = (uint16_t)(base & 0xFFFF);
    idt[vec].selector = selector;
    idt[vec].zero = 0;
    idt[vec].flags = flags;
    idt[vec].base_high = (uint16_t)((base >> 16) & 0xFFFF);
}

void idt_load(void) {
    __asm__ __volatile__("lidt (%0)" : : "r"(&idt_desc));
}

static void idt_clear(void) {
    for (uint16_t i = 0; i < IDT_ENTRIES; i++) {
        idt[i].base_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].flags = 0;
        idt[i].base_high = 0;
    }
}

void idt_init(void) {
    idt_clear();

    idt_desc.limit = (uint16_t)(sizeof(idt) - 1);
    idt_desc.base = (uint32_t)&idt[0];

    uint8_t flags = IDT_FLAG_PRESENT | IDT_FLAG_INT_GATE;
    uint16_t selector = 0x08;

    idt_set_gate(0, isr0, selector, flags);
    idt_set_gate(1, isr1, selector, flags);
    idt_set_gate(2, isr2, selector, flags);
    idt_set_gate(3, isr3, selector, flags);
    idt_set_gate(4, isr4, selector, flags);
    idt_set_gate(5, isr5, selector, flags);
    idt_set_gate(6, isr6, selector, flags);
    idt_set_gate(7, isr7, selector, flags);
    idt_set_gate(8, isr8, selector, flags);
    idt_set_gate(9, isr9, selector, flags);
    idt_set_gate(10, isr10, selector, flags);
    idt_set_gate(11, isr11, selector, flags);
    idt_set_gate(12, isr12, selector, flags);
    idt_set_gate(13, isr13, selector, flags);
    idt_set_gate(14, isr14, selector, flags);
    idt_set_gate(15, isr15, selector, flags);
    idt_set_gate(16, isr16, selector, flags);
    idt_set_gate(17, isr17, selector, flags);
    idt_set_gate(18, isr18, selector, flags);
    idt_set_gate(19, isr19, selector, flags);
    idt_set_gate(20, isr20, selector, flags);
    idt_set_gate(21, isr21, selector, flags);
    idt_set_gate(22, isr22, selector, flags);
    idt_set_gate(23, isr23, selector, flags);
    idt_set_gate(24, isr24, selector, flags);
    idt_set_gate(25, isr25, selector, flags);
    idt_set_gate(26, isr26, selector, flags);
    idt_set_gate(27, isr27, selector, flags);
    idt_set_gate(28, isr28, selector, flags);
    idt_set_gate(29, isr29, selector, flags);
    idt_set_gate(30, isr30, selector, flags);
    idt_set_gate(31, isr31, selector, flags);
    idt_set_gate(32, irq0, selector, flags);

    idt_load();
}
