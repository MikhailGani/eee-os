#ifndef CALC_H
#define CALC_H

#include <stdint.h>

int calc_eval(const char *expr, int32_t *out, const char **error);

#endif
