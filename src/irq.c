#include <stdint.h>

#include "kbd.h"
#include "pic.h"

void irq_handler(uint32_t vector) {
    if (vector < 0x20 || vector > 0x2F) {
        return;
    }

    uint8_t irq = (uint8_t)(vector - 0x20);
    if (irq == 1) {
        kbd_handle_irq();
    }
    pic_send_eoi(irq);
}
