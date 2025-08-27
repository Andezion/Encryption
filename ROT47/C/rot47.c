#include "c_rot47.h"
#include <stdlib.h>
#include <string.h>

void rot47_init(rot47 *r, const char *str) {
    r->len = strlen(str);
    r->input = (char*)malloc(r->len + 1);
    strcpy(r->input, str);
}

char *rot47_encode(const rot47 *r) {
    char *encoded = (char*)malloc(r->len + 1);
    for (size_t i = 0; i < r->len; i++) {
        if (r->input[i] >= 33 && r->input[i] <= 126)
            encoded[i] = (char)(33 + (r->input[i] - 33 + 47) % 94);
        else
            encoded[i] = r->input[i];
    }
    encoded[r->len] = '\0';
    return encoded;
}

char *rot47_decode(const rot47 *r, const char *encoded_version) {
    char *decoded = (char*)malloc(r->len + 1);
    for (size_t i = 0; i < r->len; i++) {
        if (encoded_version[i] >= 33 && encoded_version[i] <= 126)
            decoded[i] = (char)(33 + (encoded_version[i] - 33 + 47) % 94);
        else
            decoded[i] = encoded_version[i];
    }
    decoded[r->len] = '\0';
    return decoded;
}
