#ifndef PIC_H
#define PIC_H

#include <stdint.h>

void pic_init(void);
void pic_set_mask(uint8_t irq, int masked);
void pic_send_eoi(uint8_t irq);

#endif
