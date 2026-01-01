#include "calc.h"

#include <stddef.h>
#include <stdint.h>

static void skip_ws(const char **p) {
    while (**p == ' ' || **p == '\t') {
        (*p)++;
    }
}

static int parse_expr(const char **p, int32_t *out, const char **error);

static int parse_number(const char **p, int32_t *out, const char **error) {
    skip_ws(p);

    int sign = 1;
    if (**p == '+' || **p == '-') {
        if (**p == '-') {
            sign = -1;
        }
        (*p)++;
    }

    if (**p < '0' || **p > '9') {
        *error = "error: invalid token";
        return -1;
    }

    int32_t value = 0;
    while (**p >= '0' && **p <= '9') {
        value = (int32_t)(value * 10 + (**p - '0'));
        (*p)++;
    }

    *out = (int32_t)(value * sign);
    return 0;
}

static int parse_factor(const char **p, int32_t *out, const char **error) {
    skip_ws(p);

    if (**p == '(') {
        (*p)++;
        if (parse_expr(p, out, error) != 0) {
            return -1;
        }
        skip_ws(p);
        if (**p != ')') {
            *error = "error: mismatched parentheses";
            return -1;
        }
        (*p)++;
        return 0;
    }

    return parse_number(p, out, error);
}

static int parse_term(const char **p, int32_t *out, const char **error) {
    if (parse_factor(p, out, error) != 0) {
        return -1;
    }

    for (;;) {
        skip_ws(p);
        char op = **p;
        if (op != '*' && op != '/') {
            return 0;
        }
        (*p)++;

        int32_t rhs = 0;
        if (parse_factor(p, &rhs, error) != 0) {
            return -1;
        }

        if (op == '*') {
            *out = (int32_t)(*out * rhs);
        } else {
            if (rhs == 0) {
                *error = "error: division by zero";
                return -1;
            }
            *out = (int32_t)(*out / rhs);
        }
    }
}

static int parse_expr(const char **p, int32_t *out, const char **error) {
    if (parse_term(p, out, error) != 0) {
        return -1;
    }

    for (;;) {
        skip_ws(p);
        char op = **p;
        if (op != '+' && op != '-') {
            return 0;
        }
        (*p)++;

        int32_t rhs = 0;
        if (parse_term(p, &rhs, error) != 0) {
            return -1;
        }

        if (op == '+') {
            *out = (int32_t)(*out + rhs);
        } else {
            *out = (int32_t)(*out - rhs);
        }
    }
}

int calc_eval(const char *expr, int32_t *out, const char **error) {
    if (!expr) {
        *error = "error: invalid token";
        return -1;
    }

    const char *p = expr;
    if (parse_expr(&p, out, error) != 0) {
        return -1;
    }

    skip_ws(&p);
    if (*p != '\0') {
        *error = "error: invalid token";
        return -1;
    }

    return 0;
}
