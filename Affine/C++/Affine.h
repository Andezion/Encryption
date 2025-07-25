#pragma once
#include <string>
#include <utility>

class affine
{
    size_t a{};
    size_t b{};
    std::string input{};
public:
    affine(std::string input, const size_t a, const size_t b)
    : a(a), b(b), input(std::move(input))
    {
        if (gcd(static_cast<int>(a), 26) != 1)
        {
            throw std::invalid_argument("Parameter 'a' must be coprime with 26");
        }

        for (char i = 'a'; i <= 'z'; i++)
        {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }

    static int gcd(const int a, const int b)
    {
        return b == 0 ? a : gcd(b, a % b);
    }

    [[nodiscard]] std::string encode() const
    {
        std::string output = input;
        for (char & c : output)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>(((c - 'a') * a + b) % 26 + 'a');
            }
            else if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>(((c - 'A') * a + b) % 26 + 'A');
            }
            else if (isspace(c) || isdigit(c) || ispunct(c) || iswspace(c))
            {
                //c = static_cast<char>('0' + (c - '0' + key) % 10);
            }
            else
            {
                throw std::invalid_argument("Invalid character");
            }
        }
        return output;
    }

    static int mod_inverse(int a, const int m)
    {
        a = a % m;
        for (int x = 1; x < m; ++x)
        {
            if (a * x % m == 1)
            {
                return x;
            }
        }
        throw std::invalid_argument("No modular inverse exists for given 'a'");
    }

    [[nodiscard]] std::string decode(const std::string & encoded_version) const
    {
        std::string output = encoded_version;
        const int a_inv = mod_inverse(static_cast<int>(a), 26);
        for (char & c : output)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>(a_inv * (c - 'a' - b + 26) % 26 + 'a');
            }
            else if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>(a_inv * (c - 'A' - b + 26) % 26 + 'A');
            }
            else if (isspace(c) || isdigit(c) || ispunct(c) || iswspace(c))
            {
                //c = static_cast<char>('0' + (c - '0' + key) % 10);
            }
            else
            {
                throw std::invalid_argument("Invalid character");
            }
        }
        return output;
    }
};
