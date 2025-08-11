#include "tans_embedded.h"
#include <stdio.h>

static int find_symbol_index(tans_t *tans, uint8_t symbol) {
    for (int i = 0; i < tans->unique_symbol_count; i++) {
        if (tans->unique_symbols[i] == symbol) {
            return i;
        }
    }
    return -1;
}

static int safe_c(tans_t *tans, uint16_t state, uint8_t symbol, uint16_t *result) {
    int symbol_idx = find_symbol_index(tans, symbol);
    if (symbol_idx < 0) return TANS_ERROR_SYMBOL_NOT_FOUND;
    
    uint16_t count = tans->symbol_count[symbol_idx];
    if (count == 0) return TANS_ERROR_SYMBOL_NOT_FOUND;
    
    if (state == UINT16_MAX) return TANS_ERROR_STATE_OVERFLOW;
    
    uint16_t full_blocks = (state + 1) / count;
    uint16_t symbols_left = (state + 1) % count;
    
    if (symbols_left == 0) {
        if (full_blocks == 0) return TANS_ERROR_STATE_OVERFLOW;
        full_blocks -= 1;
        symbols_left = count;
    }
    
    if (symbols_left > tans->symbol_position_count[symbol_idx]) {
        return TANS_ERROR_DECODE_FAILED;
    }
    
    uint16_t index_within_block = tans->symbol_positions[symbol_idx][symbols_left - 1];
    *result = full_blocks * tans->block_size + index_within_block;
    
    return TANS_OK;
}

static int safe_d(tans_t *tans, uint16_t state, uint8_t *symbol, uint16_t *old_state) {
    uint16_t index_within_block = state % tans->block_size;
    if (index_within_block >= tans->block_size) {
        return TANS_ERROR_DECODE_FAILED;
    }
    
    *symbol = tans->alphabet[index_within_block];
    
    uint16_t count_within_block = 0;
    for (uint16_t i = 0; i < index_within_block; i++) {
        if (tans->alphabet[i] == *symbol) {
            count_within_block++;
        }
    }
    
    int symbol_idx = find_symbol_index(tans, *symbol);
    if (symbol_idx < 0) return TANS_ERROR_SYMBOL_NOT_FOUND;
    
    *old_state = tans->symbol_count[symbol_idx] + count_within_block;
    return TANS_OK;
}

static int table_generator(tans_t *tans) {
    memset(tans->normalization_table, 0, sizeof(tans->normalization_table));
    memset(tans->encoding_table, 0, sizeof(tans->encoding_table));
    memset(tans->decoding_table, 0, sizeof(tans->decoding_table));

    if (tans->unique_symbol_count == 1) {
        int symbol_idx = 0;
        tans->normalization_table[symbol_idx].n_bits_chopped = 0;
        tans->normalization_table[symbol_idx].min_state = 0;
        tans->normalization_table[symbol_idx].mask = 0;
        
        for (uint16_t i = 0; i < tans->block_size; i++) {
            tans->encoding_table[symbol_idx][i + 1] = tans->block_size + i;
            tans->decoding_table[i].old_state_shifted = i + 1;
            tans->decoding_table[i].num_bits = 0;
            tans->decoding_table[i].mask = 0;
        }
        return TANS_OK;
    }

    for (int sym_idx = 0; sym_idx < tans->unique_symbol_count; sym_idx++) {
        uint8_t symbol = tans->unique_symbols[sym_idx];
        
        int n_bits_chopped = -1;
        uint16_t min_state = 0;
        int prev_n_bits = -1;
        
        for (uint16_t state = tans->block_size; state < tans->block_size * 2; state++) {
            uint16_t normalized = state;
            int n_bits = 0;
            
            uint16_t c_result;
            while (safe_c(tans, normalized, symbol, &c_result) == TANS_OK && 
                   c_result >= tans->block_size * 2) {
                if (n_bits >= 16) return TANS_ERROR_STATE_OVERFLOW;
                normalized >>= 1;
                n_bits++;
            }
            
            if (n_bits != prev_n_bits) {
                prev_n_bits = n_bits;
                if (n_bits_chopped < 0) {
                    n_bits_chopped = n_bits;
                } else {
                    if (min_state == 0) {
                        min_state = state;
                    }
                }
            }
        }
        
        if (n_bits_chopped < 0) n_bits_chopped = 0;
        
        uint32_t mask = n_bits_chopped < 32 ? ((1U << n_bits_chopped) - 1) : 0xFFFFFFFF;
        tans->normalization_table[sym_idx].n_bits_chopped = n_bits_chopped;
        tans->normalization_table[sym_idx].min_state = min_state;
        tans->normalization_table[sym_idx].mask = mask;
    }

    for (uint16_t state = tans->block_size; state < tans->block_size * 2; state++) {
        uint8_t symbol;
        uint16_t old_state;
        
        if (safe_d(tans, state, &symbol, &old_state) != TANS_OK) {
            return TANS_ERROR_DECODE_FAILED;
        }
        
        int symbol_idx = find_symbol_index(tans, symbol);
        if (symbol_idx < 0) return TANS_ERROR_SYMBOL_NOT_FOUND;
        
        tans->encoding_table[symbol_idx][old_state] = state;
        
        int n_bits = 0;
        uint16_t scaled_state = old_state;
        
        while (scaled_state < tans->block_size && n_bits < 16) {
            scaled_state <<= 1;
            n_bits++;
        }
        
        if (n_bits >= 16) return TANS_ERROR_STATE_OVERFLOW;
        
        uint16_t table_index = state - tans->block_size;
        uint32_t mask = (n_bits < 32) ? ((1U << n_bits) - 1) : 0xFFFFFFFF;
        
        tans->decoding_table[table_index].old_state_shifted = old_state << n_bits;
        tans->decoding_table[table_index].num_bits = n_bits;
        tans->decoding_table[table_index].mask = mask;
    }
    
    return TANS_OK;
}

int tans_init(tans_t *tans, const uint8_t *alphabet, uint16_t alphabet_size) {
    if (!tans || !alphabet || alphabet_size == 0 || alphabet_size > MAX_ALPHABET_SIZE) {
        return TANS_ERROR_INVALID_PARAM;
    }
    
    memset(tans, 0, sizeof(tans_t));

    memcpy(tans->alphabet, alphabet, alphabet_size);
    tans->block_size = alphabet_size;
    tans->block_mask = alphabet_size - 1;

    for (uint16_t i = 0; i < alphabet_size; i++) {
        uint8_t c = alphabet[i];

        int symbol_idx = find_symbol_index(tans, c);
        if (symbol_idx < 0) {

            if (tans->unique_symbol_count >= MAX_ALPHABET_SIZE) {
                return TANS_ERROR_BUFFER_OVERFLOW;
            }
            symbol_idx = tans->unique_symbol_count;
            tans->unique_symbols[symbol_idx] = c;
            tans->unique_symbol_count++;
        }

        if (tans->symbol_position_count[symbol_idx] >= MAX_SYMBOL_POSITIONS) {
            return TANS_ERROR_BUFFER_OVERFLOW;
        }
        
        tans->symbol_positions[symbol_idx][tans->symbol_position_count[symbol_idx]] = i;
        tans->symbol_position_count[symbol_idx]++;
        tans->symbol_count[symbol_idx]++;
    }

    return table_generator(tans);
}

int tans_encode(tans_t *tans, const uint8_t *message, uint16_t message_size, 
                uint16_t initial_state, tans_encode_result_t *result) {
    if (!tans || !message || message_size == 0 || !result) {
        return TANS_ERROR_INVALID_PARAM;
    }
    
    if (message_size > MAX_MESSAGE_SIZE) {
        return TANS_ERROR_BUFFER_OVERFLOW;
    }

    uint8_t last_symbol = message[message_size - 1];
    int symbol_idx = find_symbol_index(tans, last_symbol);
    if (symbol_idx < 0) return TANS_ERROR_SYMBOL_NOT_FOUND;
    
    if (initial_state >= tans->symbol_position_count[symbol_idx]) {
        return TANS_ERROR_INVALID_PARAM;
    }
    
    uint16_t state = tans->symbol_positions[symbol_idx][initial_state] + tans->block_size;
    
    result->bitstream_size = 0;

    for (int i = message_size - 2; i >= 0; i--) {
        uint8_t symbol = message[i];
        int sym_idx = find_symbol_index(tans, symbol);
        if (sym_idx < 0) return TANS_ERROR_SYMBOL_NOT_FOUND;
        
        normalized_entry_t *norm = &tans->normalization_table[sym_idx];
        int n_bits = norm->n_bits_chopped;
        uint32_t mask = norm->mask;
        
        if (norm->min_state > 0 && state >= norm->min_state) {
            n_bits++;
            mask = (mask << 1) | 1;
        }

        if (n_bits > 0) {
            if (result->bitstream_size + n_bits > MAX_BITSTREAM_SIZE * 8) {
                return TANS_ERROR_BUFFER_OVERFLOW;
            }
            
            for (int b = n_bits - 1; b >= 0; --b) {
                uint8_t bit = (state >> b) & 1;
                uint16_t byte_idx = result->bitstream_size / 8;
                uint8_t bit_idx = result->bitstream_size % 8;
                
                if (bit) {
                    result->bitstream[byte_idx] |= (1 << (7 - bit_idx));
                }
                result->bitstream_size++;
            }
        }
        
        uint16_t shifted_state = state >> n_bits;
        state = tans->encoding_table[sym_idx][shifted_state];
    }
    
    result->state = state;
    return TANS_OK;
}

int tans_decode(tans_t *tans, uint16_t state, const uint8_t *bitstream, 
                uint16_t bitstream_size, tans_decode_result_t *result) {
    if (!tans || !bitstream || !result) {
        return TANS_ERROR_INVALID_PARAM;
    }
    
    result->message_size = 0;
    result->final_state = state;
    
    uint16_t bitstream_pos = bitstream_size;
    uint16_t iterations = 0;
    const uint16_t MAX_ITERATIONS = 1000;
    
    while (iterations < MAX_ITERATIONS) {
        iterations++;
        
        if (state < tans->block_size) break;
        
        uint16_t table_index = state - tans->block_size;
        if (table_index >= tans->block_size) {
            return TANS_ERROR_DECODE_FAILED;
        }
        
        uint8_t symbol;
        uint16_t old_state;
        if (safe_d(tans, state, &symbol, &old_state) != TANS_OK) {
            return TANS_ERROR_DECODE_FAILED;
        }
        
        if (result->message_size >= MAX_MESSAGE_SIZE) {
            return TANS_ERROR_BUFFER_OVERFLOW;
        }
        
        result->message[result->message_size++] = symbol;
        
        decoded_entry_t *decode_entry = &tans->decoding_table[table_index];
        state = decode_entry->old_state_shifted;
        
        int num_bits = decode_entry->num_bits;
        if (num_bits > 0) {
            if (bitstream_pos < num_bits) break;
            
            uint32_t bits = 0;
            for (int i = 0; i < num_bits; i++) {
                bitstream_pos--;
                uint16_t byte_idx = bitstream_pos / 8;
                uint8_t bit_idx = bitstream_pos % 8;
                
                if (bitstream[byte_idx] & (1 << (7 - bit_idx))) {
                    bits |= (1U << i);
                }
            }
            
            state |= bits;
        } else {
            if (bitstream_pos == 0 && tans->unique_symbol_count > 1) {
                break;
            }
        }
    }
    
    if (iterations >= MAX_ITERATIONS) {
        return TANS_ERROR_DECODE_FAILED;
    }
    
    result->final_state = state;
    return TANS_OK;
}

void tans_print_stats(tans_t *tans) {
    if (!tans) return;
    
    printf("Block size: %u\n", tans->block_size);
    printf("Unique symbols: %u\n", tans->unique_symbol_count);
    
    printf("Symbol frequencies: ");
    for (int i = 0; i < tans->unique_symbol_count; i++) {
        uint8_t symbol = tans->unique_symbols[i];
        if (symbol >= 32 && symbol <= 126) {
            printf("'%c':%u ", symbol, tans->symbol_count[i]);
        } else {
            printf("(0x%02x):%u ", symbol, tans->symbol_count[i]);
        }
    }
    printf("\n");
}

int tans_validate_tables(tans_t *tans) {
    if (!tans) return 0;

    if (tans->unique_symbol_count == 0 || tans->block_size == 0) {
        return 0;
    }
    
    return 1;
}
