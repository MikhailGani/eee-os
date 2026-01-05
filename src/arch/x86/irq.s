/* IRQ stubs */

.extern irq_handler

.section .text

.macro IRQ num vec
.global irq\num
.type irq\num, @function
irq\num:
    pushl $0
    pushl $\vec
    jmp irq_common
.endm

irq_common:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp
    call irq_handler
    add $4, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

IRQ 0, 32
