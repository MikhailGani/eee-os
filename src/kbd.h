#ifndef KBD_H
#define KBD_H

int kbd_getchar_nonblock(void);
int kbd_poll_getchar(void);
void kbd_handle_irq(void);
void kbd_init(void);

#endif
