#include "arch/x86/irq.h"

#include <stdint.h>

#include "drivers/kbd.h"
#include "drivers/pic.h"
#include "drivers/pit.h"

void irq_handler(struct regs *r) {
    uint8_t irq = (uint8_t)(r->int_no - 0x20);

    if (r->int_no == 0x20) {
        pit_handle_irq0();
    } else if (r->int_no == 0x21) {
        kbd_handle_irq();
    }

    pic_send_eoi(irq);
}
