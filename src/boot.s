/* Multiboot v1 header + entry point for i686 */

.section .multiboot
.align 4
    /* Multiboot v1 header */
    .set MB_MAGIC,  0x1BADB002
    .set MB_FLAGS,  0x00000003        /* align modules + memory info */
    .set MB_CHECK,  -(MB_MAGIC + MB_FLAGS)

    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECK

.section .text
.global _start
.type _start, @function
_start:
    cli
    /* Set up a simple stack */
    mov $stack_top, %esp

    /* Multiboot passes:
       eax = magic (0x2BADB002)
       ebx = pointer to multiboot info structure
    */
    push %ebx
    push %eax
    call kmain

.hang:
    hlt
    jmp .hang

.section .bss
.align 16
stack_bottom:
    .skip 16384
stack_top:
