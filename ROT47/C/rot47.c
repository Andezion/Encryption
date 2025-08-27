#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rot47
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
            if (input[i] >= 33 && input[i] <= 126)
            {
                encoded[i] = (char) (33 + (input[i] - 33 + 47) % 94);
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
            if (input[i] >= 33 && input[i] <= 126)
            {
                decoded[i] = (char) (33 + (input[i] - 33 + 47) % 94);
            }
        }
        decoded[len] = '\0';
        return decoded;
    }
};
