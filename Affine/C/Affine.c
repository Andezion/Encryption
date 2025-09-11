#include "affine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int gcd(const int a, const int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

static int mod_inverse(int a)
{
    a = a % 26;
    for (int x = 1; x < 26; x++) {
        if (a * x % 26 == 1) {
            return x;
        }
    }
    fprintf(stderr, "No modular inverse exists for given 'a' = %d\n", a);
    exit(EXIT_FAILURE);
}

Affine affine_create(char *input, const int a, const int b)
{
    if (gcd(a, 26) != 1)
    {
        fprintf(stderr, "Parameter 'a' must be coprime with 26\n");
        exit(EXIT_FAILURE);
    }

    Affine cipher;
    cipher.a = a;
    cipher.b = b;
    cipher.input = input;

    for (char c = 'a'; c <= 'z'; c++)
    {
        printf("%c ", c);
    }
    printf("\n");

    return cipher;
}

char* affine_encode(const Affine *affine)
{
    const size_t len = strlen(affine->input);
    char *output = malloc(len + 1);
    if (!output)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < len; i++)
    {
        const char c = affine->input[i];
        if (c >= 'a' && c <= 'z')
        {
            output[i] = (char)(((c - 'a') * affine->a + affine->b) % 26 + 'a');
        }
        else if (c >= 'A' && c <= 'Z')
        {
            output[i] = (char)(((c - 'A') * affine->a + affine->b) % 26 + 'A');
        }
        else if (isspace(c) || isdigit(c) || ispunct(c))
        {
            output[i] = c;
        }
        else
        {
            fprintf(stderr, "Invalid character: %c\n", c);
            free(output);
            exit(EXIT_FAILURE);
        }
    }
    output[len] = '\0';
    return output;
}

char* affine_decode(const Affine *affine, const char *encoded)
{
    const size_t len = strlen(encoded);
    char *output = malloc(len + 1);
    
    if (!output)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    const int a_inv = mod_inverse(affine->a);

    for (size_t i = 0; i < len; i++)
    {
        const char c = encoded[i];
        if (c >= 'a' && c <= 'z')
        {
            output[i] = (char)((a_inv * (c - 'a' - affine->b + 26)) % 26 + 'a');
        }
        else if (c >= 'A' && c <= 'Z')
        {
            output[i] = (char)((a_inv * (c - 'A' - affine->b + 26)) % 26 + 'A');
        }
        else if (isspace(c) || isdigit(c) || ispunct(c))
        {
            output[i] = c;
        }
        else
        {
            fprintf(stderr, "Invalid character: %c\n", c);
            free(output);
            exit(EXIT_FAILURE);
        }
    }
    output[len] = '\0';
    return output;
}
