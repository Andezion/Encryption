#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Transposition
{
    size_t len;
    char *input;
};

void init(struct Transposition *t)
{
    t->input = malloc(1000);
    if (!t->input)
    {
        perror("malloc");
        exit(1);
    }

    if (!fgets(t->input, 1000, stdin))
    {
        perror("fgets");
        exit(1);
    }

    t->input[strcspn(t->input, "\n")] = 0;
    t->len = strlen(t->input);
}

char *encode(const struct Transposition *t)
{
    char *encoded = malloc(t->len + 1);
    if (!encoded)
    {
        perror("malloc");
        exit(1);
    }

    const size_t capacity = t->len / 5;
    if (capacity == 0)
    {
        encoded[0] = '\0';
        return encoded;
    }

    const size_t rows = capacity;
    char **matrix = malloc(rows * sizeof(char *));
    for (size_t i = 0; i < rows; i++)
    {
        matrix[i] = malloc(5);
    }

    size_t col = 0, row = 0;
    for (size_t i = 0; i < t->len; i++)
    {
        if (isalpha(t->input[i]))
        {
            matrix[row][col++] = tolower(t->input[i]);
            if (col == 5)
            {
                col = 0;
                row++;
                if (row == rows) break;
            }
        }
    }

    size_t index = 0;
    for (int j = 4; j >= 0; j--)
    {
        for (size_t i = 0; i < rows; i++)
        {
            encoded[index++] = toupper(matrix[i][j]);
        }
    }

    encoded[index] = '\0';

    for (size_t i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return encoded;
}

char *decode(const char *encoded)
{
    const size_t len = strlen(encoded);
    if (len == 0)
    {
        char *out = malloc(1);
        out[0] = '\0';
        return out;
    }

    const size_t rows = len / 5;
    char **matrix = malloc(rows * sizeof(char *));
    for (size_t i = 0; i < rows; i++)
    {
        matrix[i] = malloc(5);
    }

    size_t index = 0;
    for (int col = 4; col >= 0; col--)
    {
        for (size_t row = 0; row < rows; row++)
        {
            matrix[row][col] = encoded[index++];
        }
    }

    char *decoded = malloc(len + 1);
    index = 0;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            decoded[index++] = matrix[i][j];
        }
    }
    decoded[index] = '\0';

    for (size_t i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return decoded;
}
