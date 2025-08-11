#ifndef TANS_EMBEDDED_H
#define TANS_EMBEDDED_H

#include <stdint.h>
#include <string.h>


#define MAX_ALPHABET_SIZE 64        // Уменьшено с 256
#define MAX_MESSAGE_SIZE 256        // Максимальный размер сообщения
#define MAX_BITSTREAM_SIZE 1024      // Максимальный размер битового потока
#define MAX_SYMBOL_POSITIONS 64     // Максимальное количество позиций символа

#define TANS_OK 0
#define TANS_ERROR_INVALID_PARAM (-1)
#define TANS_ERROR_BUFFER_OVERFLOW (-2)
#define TANS_ERROR_SYMBOL_NOT_FOUND (-3)
#define TANS_ERROR_STATE_OVERFLOW (-4)
#define TANS_ERROR_DECODE_FAILED (-5)

typedef struct {
    int n_bits_chopped;
    uint16_t min_state;
    uint32_t mask;
} normalized_entry_t;

typedef struct {
    uint16_t old_state_shifted;
    int num_bits;
    uint32_t mask;
} decoded_entry_t;

typedef struct {
    uint8_t alphabet[MAX_ALPHABET_SIZE];
    uint16_t block_size;
    uint16_t block_mask;

    uint8_t unique_symbols[MAX_ALPHABET_SIZE];
    uint16_t symbol_count[MAX_ALPHABET_SIZE];        // count_per_block
    uint16_t symbol_positions[MAX_ALPHABET_SIZE][MAX_SYMBOL_POSITIONS]; // symbol_table
    uint16_t symbol_position_count[MAX_ALPHABET_SIZE];
    uint16_t unique_symbol_count;

    normalized_entry_t normalization_table[MAX_ALPHABET_SIZE];
    uint16_t encoding_table[MAX_ALPHABET_SIZE][MAX_ALPHABET_SIZE]; // [symbol_index][old_state]
    decoded_entry_t decoding_table[MAX_ALPHABET_SIZE];

} tans_t;

typedef struct {
    uint16_t state;
    uint8_t bitstream[MAX_BITSTREAM_SIZE];
    uint16_t bitstream_size;
} tans_encode_result_t;

typedef struct {
    uint8_t message[MAX_MESSAGE_SIZE];
    uint16_t message_size;
    uint16_t final_state;
} tans_decode_result_t;

int tans_init(tans_t *tans, const uint8_t *alphabet, uint16_t alphabet_size);
int tans_encode(tans_t *tans, const uint8_t *message, uint16_t message_size,
                uint16_t initial_state, tans_encode_result_t *result);
int tans_decode(tans_t *tans, uint16_t state, const uint8_t *bitstream,
                uint16_t bitstream_size, tans_decode_result_t *result);

void tans_print_stats(tans_t *tans);
int tans_validate_tables(tans_t *tans);

#endif // TANS_EMBEDDED_H
