#ifndef ROT47_H
#define ROT47_H

#include <stddef.h>

typedef struct {
    size_t len;
    char *input;
} rot47;

void rot47_init(rot47 *r, const char *str);
char *rot47_encode(const rot47 *r);
char *rot47_decode(const rot47 *r, const char *encoded);

#endif
