#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char* preprocess_text(const char* text) {
    size_t len = strlen(text);
    char* cleaned = (char*)malloc(len + 1);
    if (!cleaned) return NULL;

    size_t idx = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)text[i];
        if (isalpha(c)) {
            cleaned[idx++] = (char)tolower(c);
        }
    }
    cleaned[idx] = '\0';
    return cleaned;
}

char* scytale_encode(const char* input, size_t cols, int reverse) {
    size_t len = strlen(input);
    size_t rows = (len + cols - 1) / cols;
    char* output = (char*)malloc(rows * cols + 1);
    if (!output) return NULL;

    char** matrix = (char**)malloc(rows * sizeof(char*));
    for (size_t i = 0; i < rows; i++) {
        matrix[i] = (char*)malloc(cols);
        for (size_t j = 0; j < cols; j++) {
            size_t index = i * cols + j;
            if (index < len) {
                matrix[i][j] = input[index];
            } else {
                matrix[i][j] = 'x'; 
            }
        }
    }

    size_t out_idx = 0;
    if (reverse) {
        for (int j = (int)cols - 1; j >= 0; j--) {
            for (size_t i = 0; i < rows; i++) {
                output[out_idx++] = (char)toupper(matrix[i][j]);
            }
        }
    } else {
        for (size_t j = 0; j < cols; j++) {
            for (size_t i = 0; i < rows; i++) {
                output[out_idx++] = (char)toupper(matrix[i][j]);
            }
        }
    }
    output[out_idx] = '\0';

    for (size_t i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);

    return output;
}

char* scytale_decode(const char* encoded, size_t cols, int reverse) {
    size_t len = strlen(encoded);
    size_t rows = len / cols;
    char** matrix = (char**)malloc(rows * sizeof(char*));
    for (size_t i = 0; i < rows; i++) {
        matrix[i] = (char*)malloc(cols);
    }

    size_t idx = 0;
    if (reverse) {
        for (int col = (int)cols - 1; col >= 0; col--) {
            for (size_t row = 0; row < rows; row++) {
                matrix[row][col] = (char)tolower((unsigned char)encoded[idx++]);
            }
        }
    } else {
        for (size_t col = 0; col < cols; col++) {
            for (size_t row = 0; row < rows; row++) {
                matrix[row][col] = (char)tolower((unsigned char)encoded[idx++]);
            }
        }
    }

    char* output = (char*)malloc(len + 1);
    idx = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            output[idx++] = matrix[i][j];
        }
    }
    output[idx] = '\0';

    while (idx > 0 && output[idx - 1] == 'x') {
        output[--idx] = '\0';
    }

    for (size_t i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);

    return output;
}
