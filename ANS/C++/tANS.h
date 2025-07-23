#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class t_ans
{
    std::string our_labels;
    size_t block_size = 0;
    size_t block_mask = 0;

    std::unordered_map<char, std::vector<size_t>> symbol_table;
    std::unordered_map<char, size_t> count_per_block;
    std::vector<size_t> count_before_index;

    struct NormalizedEntry
    {
        int n_bits_chopped;
        size_t min_state;
        uint32_t mask;

        NormalizedEntry() : n_bits_chopped(0), min_state(0), mask(0) {}
        NormalizedEntry(const int bits, const size_t state, const uint32_t m) :
            n_bits_chopped(bits), min_state(state), mask(m) {}
    };

    struct DecodedEntry
    {
        size_t old_state_shifted;
        int num_bits;
        uint32_t mask;

        DecodedEntry() : old_state_shifted(0), num_bits(0), mask(0) {}
        DecodedEntry(const size_t old_state, const int bits, const uint32_t m) :
            old_state_shifted(old_state), num_bits(bits), mask(m) {}
    };

    std::unordered_map<char, NormalizedEntry> normalization_table;
    std::unordered_map<char, std::unordered_map<size_t, size_t>> encoding_table;
    std::vector<DecodedEntry> decoding_table;

    size_t safe_c(const size_t state, const char symbol) const
    {
        const auto it = count_per_block.find(symbol);
        if (it == count_per_block.end())
        {
            throw std::invalid_argument("Symbol not found!");
        }

        const size_t count = it->second;
        if (count == 0)
        {
            throw std::runtime_error("Symbol count is zero!");
        }

        if (state == SIZE_MAX)
        {
            throw std::overflow_error("Symbol state is too big!");
        }

        size_t full_blocks = (state + 1) / count;
        size_t symbols_left = (state + 1) % count;

        if (symbols_left == 0)
        {
            if (full_blocks == 0)
            {
                throw std::underflow_error("Symbol state is too low!");
            }

            full_blocks -= 1;
            symbols_left = count;
        }

        const auto symbol_it = symbol_table.find(symbol);
        if (symbol_it == symbol_table.end() ||
            symbols_left > symbol_it->second.size())
        {
            throw std::runtime_error("Invalid calculation in symbol_it!");
        }

        const size_t index_within_block = symbol_it->second[symbols_left - 1];
        if (full_blocks > SIZE_MAX / block_size)
        {
            throw std::overflow_error("full_blocks is too big!");
        }

        return full_blocks * block_size + index_within_block;
    }

    std::pair<char, size_t> safe_d(const size_t state) const
    {
        const size_t index_within_block = state % block_size;
        // const size_t index_within_block = state & block_mask;
        if (index_within_block >= our_labels.length())
        {
            throw std::out_of_range("index_within_block is too big!");
        }

        const char symbol = our_labels[index_within_block];
        size_t count_within_block = 0;
        for (size_t i = 0; i < index_within_block; i++)
        {
            if (our_labels[i] == symbol)
            {
                count_within_block += 1;
            }
        }

        const auto it = count_per_block.find(symbol);
        if (it == count_per_block.end())
        {
            throw std::runtime_error("Symbol not found!");
        }

        if (it->second > SIZE_MAX - count_within_block)
        {
            throw std::overflow_error("it->second is too big!");
        }

        return {symbol, it->second + count_within_block};
    }

    void table_generator()
    {
        normalization_table.clear();

        encoding_table.clear();
        decoding_table.clear();

        for (const auto &[fst, snd] : symbol_table)
        {
            const char symbol_char = fst;

            int n_bits_chopped = -1;
            size_t min_state = 0;
            int prev_n_bits = -1;

            for (size_t state = block_size; state < block_size * 2; state++)
            {
                size_t normalized = state;
                int n_bits = 0;

                while (safe_c(normalized, symbol_char) >= block_size * 2)
                {
                    if (constexpr int MAX_BITS = 64; n_bits >= MAX_BITS)
                    {
                        throw std::runtime_error("Too many bits - wrong!");
                    }
                    normalized >>= 1;
                    n_bits++;
                }

                if (n_bits != prev_n_bits)
                {
                    prev_n_bits = n_bits;
                    if (n_bits_chopped < 0)
                    {
                        n_bits_chopped = n_bits;
                    }
                    else
                    {
                        if (min_state != 0)
                        {
                            throw std::runtime_error("State is incorrect!");
                        }
                        min_state = state;
                    }
                }
            }
            if (n_bits_chopped < 0)
            {
                throw std::runtime_error("n_bits_chopped is incorrect!");
            }

            const uint32_t mask = n_bits_chopped < 32 ? ((1U << n_bits_chopped) - 1) : 0xFFFFFFFF;
            normalization_table[symbol_char] =  NormalizedEntry(n_bits_chopped, min_state, mask);
        }

        for (const auto &[fst, snd] : symbol_table)
        {
            encoding_table[fst] = std::unordered_map<size_t, size_t>();
        }

        decoding_table.resize(block_size);

        for (size_t state = block_size; state < block_size * 2; state++)
        {
            auto [symbol, old_state] = safe_d(state);
            encoding_table[symbol][old_state] = state;

            int n_bits = 0;
            size_t scaled_state = old_state;

            constexpr int MAX_SCALED_BITS = 64;
            while (scaled_state < block_size && n_bits < MAX_SCALED_BITS)
            {
                scaled_state <<= 1;
                n_bits++;
            }

            if (n_bits >= MAX_SCALED_BITS)
            {
                throw std::runtime_error("Too many bits - wrong!");
            }

            const size_t table_index = state - block_size;
            if (table_index >= decoding_table.size())
            {
                throw std::out_of_range("table_index is too big");
            }

            if (n_bits > 0 && old_state > SIZE_MAX >> n_bits)
            {
                throw std::overflow_error("Old state shift would overflow");
            }

            const uint32_t mask = (n_bits < 32) ? ((1U << n_bits) - 1) : 0xFFFFFFFF;
            decoding_table[table_index] = DecodedEntry(old_state << n_bits, n_bits, mask);
        }
    }
public:
    explicit t_ans(const std::string& labeling_str)
    {
        if (labeling_str.empty())
        {
            throw std::invalid_argument("Labeling cannot be empty");
        }

        if (labeling_str.length() > 65536)
        {
            throw std::invalid_argument("Labeling too large");
        }

        our_labels = labeling_str;
        block_size = our_labels.length();

        block_mask = block_size - 1;


        count_per_block.clear();
        count_before_index.clear();
        symbol_table.clear();

        count_before_index.reserve(our_labels.length());

        for (size_t i = 0; i < our_labels.length(); i++)
        {
            char c = our_labels[i];

            if (count_per_block.find(c) == count_per_block.end())
            {
                count_per_block[c] = 0;
                symbol_table[c] = std::vector<size_t>();
            }

            count_before_index.push_back(count_per_block[c]);
            count_per_block[c]++;
            symbol_table[c].push_back(i);
        }

        table_generator();
    }

    explicit t_ans(const std::vector<char>& labeling_vec)
        : t_ans(std::string(labeling_vec.begin(), labeling_vec.end())) {}

    struct EncodeResult
    {
        size_t state;
        std::string bitstream;  // вместо uint64_t
        EncodeResult(const size_t s, std::string b) : state(s), bitstream(std::move(b)) {}
    };

    EncodeResult encode(const std::string& message, const size_t initial_state = 0) const
    {
        if (message.empty())
        {
            throw std::invalid_argument("Message cannot be empty");
        }

        std::cout << message.size() << std::endl;

        const char last_symbol = message.back();
        const auto symbol_it = symbol_table.find(last_symbol);

        if (symbol_it == symbol_table.end())
        {
            throw std::invalid_argument("Last symbol not found in labeling");
        }

        if (initial_state >= symbol_it->second.size())
        {
            throw std::invalid_argument("Initial state out of bounds for symbol");
        }

        //uint64_t bitstream = 1;
        std::string bitstream;
        size_t state = symbol_it->second[initial_state];

        //std::cout << "Initial state: " << state << std::endl;

        if (state >= decoding_table.size())
        {
            throw std::runtime_error("Initial state index out of bounds");
        }

        if (decoding_table[state].num_bits <= 0)
        {
            throw std::runtime_error("Code is ambiguous because initial state is too large");
        }

        state += block_size;

        for (int i = static_cast<int>(message.length()) - 2; i >= 0; i--)
        {
            char symbol = message[i];

            auto renorm_it = normalization_table.find(symbol);
            if (renorm_it == normalization_table.end())
            {
                throw std::invalid_argument("Symbol not found in renormalization table");
            }

            const auto& renorm = renorm_it->second;
            int n_bits = renorm.n_bits_chopped;
            uint32_t mask = renorm.mask;

            if (renorm.min_state > 0 && state >= renorm.min_state)
            {
                n_bits++;
                mask = (mask << 1) | 1;
            }

            if (n_bits > 0)
            {
                for (int b = n_bits - 1; b >= 0; --b)
                {
                    const char bit = ((state >> b) & 1) ? '1' : '0';
                    bitstream += bit;
                }
            }

            size_t shifted_state = state >> n_bits;

            auto encoding_it = encoding_table.find(symbol);
            if (encoding_it == encoding_table.end())
            {
                throw std::runtime_error("Symbol not found in encoding table");
            }

            auto state_it = encoding_it->second.find(shifted_state);
            if (state_it == encoding_it->second.end())
            {
                throw std::runtime_error("State not found in encoding table");
            }

            state = state_it->second;
            //::cout << "New state after encoding: " << state << std::endl;
        }

        return {state, bitstream};
    }

    struct DecodeResult
    {
        std::string message;
        size_t final_state;

        DecodeResult(std::string msg, const size_t state)
            : message(std::move(msg)), final_state(state) {}
    };

    DecodeResult decode(size_t state, std::string bitstream) const
    {
        std::string message;
        message.reserve(64);

        constexpr size_t MAX_ITERATIONS = 10000;
        size_t iterations = 0;

        while (iterations < MAX_ITERATIONS)
        {
            iterations++;

            if (state < block_size)
            {
                throw std::runtime_error("State underflow during decoding");
            }

            // state -= block_size;
            //
            // if (state >= our_labels.length())
            // {
            //     throw std::out_of_range("State index out of bounds during decoding");
            // }
            //
            // message += our_labels[state];
            //
            // if (state >= decoding_table.size())
            // {
            //     throw std::out_of_range("Decoding table index out of bounds");
            // }
            //
            // const auto& decode_entry = decoding_table[state];
            size_t table_index = state - block_size;

            if (table_index >= decoding_table.size())
            {
                throw std::out_of_range("Decoding table index out of bounds");
            }

            if (table_index >= our_labels.length())
            {
                throw std::out_of_range("State index out of bounds during decoding");
            }

            //message += our_labels[table_index];

            auto [symbol, old_state] = safe_d(state);
            message += symbol;

            const auto& decode_entry = decoding_table[table_index];
            state = decode_entry.old_state_shifted;

            const int num_bits = decode_entry.num_bits;
            const uint32_t mask = decode_entry.mask;

            // if (num_bits > 0 && bitstream <= 1)
            // {
            //     break;
            // }

            if (num_bits > 0)
            {
                if (bitstream.size() < static_cast<size_t>(num_bits))
                {
                    throw std::runtime_error("Bitstream underflow during decoding");
                }

                uint32_t bits = 0;

                for (int i = 0; i < num_bits; ++i)
                {
                    char bit_char = bitstream.back();  // читаем с конца
                    bitstream.pop_back();

                    if (bit_char == '1')
                    {
                        bits |= (1U << i);  // младший бит — справа
                    }
                    else if (bit_char != '0')
                    {
                        throw std::runtime_error("Invalid character in bitstream");
                    }
                }

                state |= bits;
            }
        }

        if (iterations >= MAX_ITERATIONS)
        {
            throw std::runtime_error("Maximum iterations reached during decoding");
        }

        return {message, state};
    }

    bool validate_tables() const
    {
        try
        {
            if (decoding_table.size() != block_size) return false;
            if (encoding_table.size() != symbol_table.size()) return false;
            if (normalization_table.size() != symbol_table.size()) return false;

            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
};
