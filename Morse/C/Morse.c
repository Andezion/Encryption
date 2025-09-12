#include "Morse.h"

#include <stdlib.h>
#include <string.h>

static const char* get_morse_code(const char c)
{
    for (size_t i = 0; i < sizeof(morse_storage) / sizeof(morse_storage[0]); i++)
    {
        if (morse_storage[i].upper == c || morse_storage[i].lower == c)
        {
            return morse_storage[i].morse;
        }
    }
    return NULL;
}

Morse morse_create(const char *input)
{
    Morse m;
    m.input = input;
    return m;
}

char* morse_encode(const Morse *morse)
{
    if (!morse || !morse->input)
    {
        return NULL;
    }

    const size_t len = strlen(morse->input);
    const size_t bufSize = len * 8 + 1;
    char *result = malloc(bufSize);

    if (!result)
    {
        return NULL;
    }

    result[0] = '\0';

    for (size_t i = 0; i < len; i++)
    {
        const char *code = get_morse_code(morse->input[i]);
        if (code)
        {
            strcat(result, code);
        }
        else
        {
            strcat(result, "?");
        }
        if (i + 1 < len)
        {
            strcat(result, " ");
        }
    }

    return result;
}

char* morse_decode(const char *encoded)
{
    if (!encoded)
    {
        return NULL;
    }

    const size_t bufSize = strlen(encoded) + 1;
    char *result = malloc(bufSize);
    if (!result)
    {
        return NULL;
    }

    size_t pos = 0;
    char token[16] = {0};
    size_t tokenPos = 0;

    for (size_t i = 0; ; i++)
    {
        const char ch = encoded[i];

        if (ch != ' ' && ch != '\0')
        {
            token[tokenPos++] = ch;
            token[tokenPos] = '\0';
        }
        else
        {
            if (tokenPos > 0)
            {
                char decoded = '?';
                for (size_t j = 0; j < sizeof(morse_storage)/sizeof(morse_storage[0]); j++)
                {
                    if (strcmp(morse_storage[j].morse, token) == 0)
                    {
                        decoded = morse_storage[j].upper;
                        break;
                    }
                }
                result[pos++] = decoded;
                tokenPos = 0;
                token[0] = '\0';
            }

            if (ch == '\0')
            {
                break;
            }
        }
    }

    result[pos] = '\0';
    return result;
}
