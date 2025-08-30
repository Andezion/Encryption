#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct atbash
{
    size_t len{};
    char *input{};

    void init()
    {
        input = (char *) malloc(1000);
        if (!input)
        {
            perror("malloc");
            exit(1);
        }

        fgets(input, 1000, stdin);
        input[strcspn(input, "\n")] = '\0';
        len = strlen(input);
    }

    char *encoded() const
    {
        char *encoded = (char *) malloc(len + 1);
        if (!encoded) return nullptr;

        for (size_t i = 0; i < len; i++)
        {
            if (input[i] >= 'a' && input[i] <= 'z')
            {
                encoded[i] = 'z' - (input[i] - 'a');
            }
            else if (input[i] >= 'A' && input[i] <= 'Z')
            {
                encoded[i] = 'Z' - (input[i] - 'A');
            }
            else if (isspace(input[i]) || isdigit(input[i]) || ispunct(input[i]))
            {
                encoded[i] = input[i];
            }
            else
            {
                free(encoded);
                return nullptr;
            }
        }
        encoded[len] = '\0';
        return encoded;
    }

    char *decode(const char *encoded_version) const
    {
        char *decoded = (char *) malloc(len + 1);
        if (!decoded) return nullptr;

        for (size_t i = 0; i < len; i++)
        {
            if (encoded_version[i] >= 'a' && encoded_version[i] <= 'z')
            {
                decoded[i] = 'z' - (encoded_version[i] - 'a');
            }
            else if (encoded_version[i] >= 'A' && encoded_version[i] <= 'Z')
            {
                decoded[i] = 'Z' - (encoded_version[i] - 'A');
            }
            else if (isspace(encoded_version[i]) || isdigit(encoded_version[i]) || ispunct(encoded_version[i]))
            {
                decoded[i] = encoded_version[i];
            }
            else
            {
                free(decoded);
                return nullptr;
            }
        }

        decoded[len] = '\0';
        return decoded;
    }

    void destroy()
    {
        free(input);
        input = nullptr;
        len = 0;
    }
};
