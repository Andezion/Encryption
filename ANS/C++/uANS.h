#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <cassert>
#include <cmath>
#include <cstdint>

class uANS
{
    static constexpr size_t TABLE_SIZE = 1024;
    static constexpr size_t STATE_BITS = 16;
    static constexpr size_t MIN_STATE = 1u << STATE_BITS;
    static constexpr size_t MAX_STATE = (1u << (STATE_BITS + 1)) - 1;

    std::unordered_map<char, size_t> original_freq;
    std::unordered_map<char, size_t> normalized_freq;
    std::vector<char> symbols;
    std::unordered_map<char, size_t> cumulative_freq;
    std::vector<char> decode_table;
    std::vector<size_t> next_state;

    struct EncodedData
    {
        std::vector<uint8_t> stream;
        size_t final_state;
    };

    static bool safe_multiply(const size_t a, const size_t b, size_t& result)
    {
        if (a == 0 || b == 0)
        {
            result = 0;
            return true;
        }
        if (a > std::numeric_limits<size_t>::max() / b)
        {
            return false;
        }
        result = a * b;
        return true;
    }

    static bool safe_add(const size_t a, const size_t b, size_t& result)
    {
        if (a > std::numeric_limits<size_t>::max() - b)
        {
            return false;
        }
        result = a + b;
        return true;
    }

    void normalize_frequencies(const std::unordered_map<char, size_t>& frequencies)
    {
        size_t total_freq = 0;
        for (const auto& [sym, freq] : frequencies)
        {
            total_freq += freq;
        }

        if (total_freq == 0)
        {
            throw std::invalid_argument("Total frequency cannot be zero!");
        }

        size_t assigned = 0;
        std::vector<std::pair<char, double>> scaled_freqs;

        for (const auto& [sym, freq] : frequencies)
        {
            double scaled = static_cast<double>(freq) * TABLE_SIZE / static_cast<double>(total_freq);
            auto normalized = static_cast<size_t>(std::round(scaled));
            if (normalized == 0)
            {
                normalized = 1;
            }

            normalized_freq[sym] = normalized;
            assigned += normalized;
            scaled_freqs.push_back({sym, scaled});
        }

        while (assigned != TABLE_SIZE)
        {
            if (assigned < TABLE_SIZE)
            {
                char best_sym = scaled_freqs[0].first;
                double best_diff = 0;
                for (const auto& [sym, scaled] : scaled_freqs)
                {
                    if (const double diff = scaled - normalized_freq[sym]; diff > best_diff)
                    {
                        best_diff = diff;
                        best_sym = sym;
                    }
                }
                normalized_freq[best_sym]++;
                assigned++;
            }
            else
                {
                char best_sym = scaled_freqs[0].first;

                double best_diff = std::numeric_limits<double>::max();
                for (const auto& [sym, scaled] : scaled_freqs)
                {
                    if (normalized_freq[sym] > 1)
                    {
                        if (const double diff = normalized_freq[sym] - scaled; diff < best_diff)
                        {
                            best_diff = diff;
                            best_sym = sym;
                        }
                    }
                }
                if (normalized_freq[best_sym] > 1)
                {
                    normalized_freq[best_sym]--;
                    assigned--;
                }
                else
                {
                    break;
                }
            }
        }
    }

    void build_tables()
    {
        symbols.clear();
        for (const auto& [sym, freq] : normalized_freq)
        {
            symbols.push_back(sym);
        }
        std::sort(symbols.begin(), symbols.end());

        size_t cumulative = 0;
        for (char sym : symbols)
        {
            cumulative_freq[sym] = cumulative;
            cumulative += normalized_freq[sym];
        }

        decode_table.resize(TABLE_SIZE);
        for (char sym : symbols)
        {
            const size_t start = cumulative_freq[sym];
            const size_t count = normalized_freq[sym];
            for (size_t i = 0; i < count; i++)
            {
                decode_table[start + i] = sym;
            }
        }

        next_state.resize(TABLE_SIZE * symbols.size());
        for (size_t i = 0; i < symbols.size(); i++)
        {
            char sym = symbols[i];
            const size_t freq = normalized_freq[sym];
            const size_t base = cumulative_freq[sym];

            for (size_t s = 0; s < TABLE_SIZE; s++)
            {
                const size_t state_idx = i * TABLE_SIZE + s;
                next_state[state_idx] = ((s / freq) << STATE_BITS) + base + (s % freq);
            }
        }
    }

    static void write_bits(std::vector<uint8_t>& stream, const size_t value, const int bits)
    {
        for (int i = bits - 8; i >= 0; i -= 8)
        {
            stream.push_back((value >> i) & 0xFF);
        }
        if (bits % 8 != 0)
        {
            stream.push_back((value & ((1 << (bits % 8)) - 1)) << (8 - bits % 8));
        }
    }

    static size_t read_bits(const std::vector<uint8_t>& stream, size_t& pos, const int bits)
    {
        size_t result = 0;
        for (int i = 0; i < bits; i++)
        {
            if (pos / 8 >= stream.size())
            {
                throw std::runtime_error("Unexpected end of stream");
            }

            const size_t byte_idx = pos / 8;
            const size_t bit_idx = pos % 8;
            const size_t bit = (stream[byte_idx] >> (7 - bit_idx)) & 1;
            result = (result << 1) | bit;
            pos++;
        }
        return result;
    }

public:
    explicit uANS(const std::unordered_map<char, size_t>& frequencies)
        : original_freq(frequencies)
    {
        if (frequencies.empty())
        {
            throw std::invalid_argument("Error! You must provide at least one frequency!");
        }

        for (const auto& [sym, freq] : frequencies)
        {
            if (freq == 0)
            {
                throw std::invalid_argument("Frequency cannot be zero!");
            }
        }

        normalize_frequencies(frequencies);
        build_tables();
    }

    EncodedData encode(const std::string& message) const
    {
        if (message.empty())
        {
            return {{}, MIN_STATE};
        }

        std::vector<uint8_t> stream;
        size_t state = MIN_STATE;

        for (auto it = message.rbegin(); it != message.rend(); ++it)
        {
            char sym = *it;

            auto freq_it = normalized_freq.find(sym);
            if (freq_it == normalized_freq.end())
            {
                throw std::invalid_argument("Symbol '" + std::string(1, sym) + "' is not in the table");
            }

            const size_t freq = freq_it->second;
            const size_t base = cumulative_freq.at(sym);

            const size_t max_state = ((MAX_STATE + 1) / freq) * freq;
            while (state >= max_state)
            {
                stream.push_back(state & 0xFF);
                state >>= 8;
            }

            const size_t quotient = state / freq;
            const size_t remainder = state % freq;
            state = quotient * TABLE_SIZE + base + remainder;
        }

        return {stream, state};
    }

    std::string decode(const EncodedData& encoded_data) const
    {
        if (encoded_data.final_state < MIN_STATE)
        {
            throw std::invalid_argument("Invalid final state");
        }

        std::vector<char> message;
        size_t state = encoded_data.final_state;
        size_t stream_pos = encoded_data.stream.size();

        while (state > MIN_STATE || stream_pos > 0)
        {
            const size_t slot = state % TABLE_SIZE;
            if (slot >= decode_table.size())
            {
                throw std::runtime_error("Invalid decode slot");
            }

            char sym = decode_table[slot];
            const size_t freq = normalized_freq.at(sym);
            const size_t base = cumulative_freq.at(sym);

            if (slot < base)
            {
                throw std::runtime_error("Invalid decode state");
            }

            state = freq * (state / TABLE_SIZE) + (slot - base);

            message.push_back(sym);

            while (state < MIN_STATE && stream_pos > 0)
            {
                stream_pos--;
                state = (state << 8) | encoded_data.stream[stream_pos];
            }
        }

        std::reverse(message.begin(), message.end());
        return {message.begin(), message.end()};
    }

    size_t encode_simple(const std::string& message) const
    {
        auto [stream, final_state] = encode(message);
        return final_state;
    }

    std::string decode_simple(const size_t state) const
    {
        if (state == 0)
        {
            throw std::invalid_argument("State cannot be 0!");
        }

        const EncodedData data = {{}, state};
        return decode(data);
    }

    static size_t get_table_size() { return TABLE_SIZE; }
    const std::unordered_map<char, size_t>& get_original_frequencies() const { return original_freq; }
    const std::unordered_map<char, size_t>& get_normalized_frequencies() const { return normalized_freq; }

    void print_statistics() const
    {
        std::cout << "Table size: " << TABLE_SIZE << std::endl;
        std::cout << "State range: [" << MIN_STATE << ", " << MAX_STATE << "]" << std::endl;
        std::cout << "\nSymbol frequencies:" << std::endl;

        for (char sym : symbols)
        {
            const size_t orig = original_freq.at(sym);
            const size_t norm = normalized_freq.at(sym);
            const double prob = static_cast<double>(norm) / TABLE_SIZE;

            std::cout << "'" << sym << "': " << orig << " -> " << norm
                      << " (prob: " << prob << ")" << std::endl;
        }
    }
};
