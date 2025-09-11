#pragma once

#include <stddef.h>

typedef struct {
    int a;
    int b;
    char *input;
} Affine;

Affine affine_create(char *input, int a, int b);

char* affine_encode(const Affine *affine);
char* affine_decode(const Affine *affine, const char *encoded);
