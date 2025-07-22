#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <cassert>

class uANS
{
    std::unordered_map<char, size_t> freq;
    size_t total = 0;
    std::vector<char> symbols;
    std::unordered_map<char, size_t> base;
    std::vector<char> decode_table;

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

public:
    explicit uANS(const std::unordered_map<char, size_t>& frequencies)
    {
        if (frequencies.empty())
        {
            throw std::invalid_argument("Error! You must provide at least one frequency!");
        }

        freq = frequencies;
        total = 0;

        for (const auto& [sym, f] : freq)
        {
            if (f == 0)
            {
                throw std::invalid_argument("Frequency is zero!");
            }

            size_t new_total;
            if (!safe_add(total, f, new_total))
            {
                throw std::overflow_error("Overflow!");
            }
            total = new_total;
            symbols.push_back(sym);
        }

        if (total == 0)
        {
            throw std::invalid_argument("Sum can't be 0!");
        }

        std::sort(symbols.begin(), symbols.end());

        size_t b = 0;
        for (char sym : symbols)
        {
            base[sym] = b;
            size_t new_b;
            if (!safe_add(b, freq[sym], new_b))
            {
                throw std::overflow_error("Overflow while creating");
            }
            b = new_b;
        }

        decode_table.reserve(total);
        for (char sym : symbols)
        {
            const size_t count = freq[sym];
            for (size_t i = 0; i < count; i++)
            {
                decode_table.push_back(sym);
            }
        }

        assert(decode_table.size() == total);
    }

    size_t encode(const std::string& message) const
    {
        if (message.empty())
        {
            return 1;
        }

        size_t x = 1;

        for (auto it = message.rbegin(); it != message.rend(); ++it)
        {
            char sym = *it;

            auto freq_it = freq.find(sym);
            if (freq_it == freq.end())
            {
                throw std::invalid_argument("Symbol '" + std::string(1, sym) + " is not in the table");
            }

            const size_t f = freq_it->second;
            auto base_it = base.find(sym);
            assert(base_it != base.end());
            const size_t b = base_it->second;

            assert(f > 0);

            const size_t x_div_f = x / f;
            const size_t x_mod_f = x % f;

            size_t temp1;
            if (!safe_multiply(x_div_f, total, temp1))
            {
                throw std::overflow_error("Overflow while encode");
            }

            size_t temp2;
            if (!safe_add(temp1, b, temp2))
            {
                throw std::overflow_error("Overflow while encode");
            }

            size_t new_x;
            if (!safe_add(temp2, x_mod_f, new_x))
            {
                throw std::overflow_error("Overflow while encode");
            }

            x = new_x;
        }

        return x;
    }

    std::string decode(size_t x) const
    {
        if (x == 0)
        {
            throw std::invalid_argument("x can't be 0!");
        }

        std::vector<char> message;

        while (x > 1)
        {
            const size_t r = x % total;

            if (r >= decode_table.size())
            {
                throw std::runtime_error("Incorrect decode");
            }

            char sym = decode_table[r];

            auto freq_it = freq.find(sym);
            auto base_it = base.find(sym);

            assert(freq_it != freq.end() && base_it != base.end());

            const size_t f = freq_it->second;
            const size_t b = base_it->second;

            if (r < b)
            {
                throw std::runtime_error("Incorrect decode");
            }

            const size_t x_div_total = x / total;
            const size_t r_minus_b = r - b;

            size_t temp;
            if (!safe_multiply(f, x_div_total, temp))
            {
                throw std::overflow_error("Overflow while decode");
            }

            size_t new_x;
            if (!safe_add(temp, r_minus_b, new_x))
            {
                throw std::overflow_error("Overflow while decode");
            }

            x = new_x;
            message.push_back(sym);
        }

        std::reverse(message.begin(), message.end());
        return std::string(message.begin(), message.end());
    }

    size_t get_total() const { return total; }
    const std::unordered_map<char, size_t>& get_frequencies() const { return freq; }
};
