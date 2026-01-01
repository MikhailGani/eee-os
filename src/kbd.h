#ifndef KBD_H
#define KBD_H

int kbd_getchar_nonblock(void);
void kbd_handle_irq(void);
void kbd_init(void);

#endif
