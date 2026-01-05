#ifndef GANIOS_KERN_LOG_H
#define GANIOS_KERN_LOG_H

void log_init(void);
void kputc(char c);
void kputs(const char *s);

#endif
