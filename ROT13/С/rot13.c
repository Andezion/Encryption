#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rot13
{
    size_t len{};
    char *input{};

    void init()
    {
        input = (char*) malloc(1000);
        if (!input)
        {
            exit(1);
        }

        fgets(input, 1000, stdin);
        input[strcspn(input, "\n")] = '\0';
        len = strlen(input);
    }

    char *encode() const
    {
        char *encoded = malloc(len + 1);

        for (size_t i = 0; i < len; i++)
        {
            if (input[i] >= 'a' && input[i] <= 'z')
            {
                encoded[i] = 'a' + (input[i] - 'a' + 13) % 26;
            }
            else if (input[i] >= 'A' && input[i] <= 'Z')
            {
                encoded[i] = 'A' + (input[i] - 'A' + 13) % 26;
            }
            else
            {
                encoded[i] = input[i];
            }
        }
        encoded[len] = '\0';
        return encoded;
    }

    char *decode(const char *encoded_version) const
    {
        char *decoded = malloc(len + 1);

        for (size_t i = 0; i < len; i++)
        {
            if (encoded_version[i] >= 'a' && encoded_version[i] <= 'z')
            {
                decoded[i] = 'a' + (encoded_version[i] - 'a' + 13) % 26;
            }
            else if (encoded_version[i] >= 'A' && encoded_version[i] <= 'Z')
            {
                decoded[i] = 'A' + (encoded_version[i] - 'A' + 13) % 26;
            }
            else
            {
                decoded[i] = encoded_version[i];
            }
        }
        decoded[len] = '\0';
        return decoded;
    }
};
