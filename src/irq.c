#include <stdint.h>

#include "kbd.h"
#include "pic.h"

void irq_handler(uint32_t vector) {
    if (vector == 0x21) {
        kbd_handle_irq();
    }
    pic_send_eoi(vector);
}
