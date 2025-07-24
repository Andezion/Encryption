#pragma once
#include <string>
#include <utility>

class caesar
{
    size_t key{};
    std::string input{};
public:
    caesar(std::string input, const size_t key)
        : key(key % 26), input(std::move(input))
    {
        for (char i = 'a'; i <= 'z'; i++)
        {
            std::cout << i << ' ';
        }
        std::cout << '\n';
    }

    [[nodiscard]] std::string encode() const
    {
        std::string output = input;
        for (char & c : output)
        {
            if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>('A' + (c - 'A' + key) % 26);
            }
            else if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>('a' + (c - 'a' + key) % 26);
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

    [[nodiscard]] std::string decode(const std::string & encoded_version) const
    {
        std::string output = encoded_version;
        for (char & c : output)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>('a' + (c - 'a' - key + 26) % 26);
            }
            else if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>('A' + (c - 'A' - key + 26) % 26);
            }
            else if (isspace(c) || isdigit(c) || ispunct(c) || iswspace(c))
            {
                //c = static_cast<char>('0' + (c - '0' - key + 10) % 10);
            }
            else
            {
                throw std::invalid_argument("Invalid character");
            }
        }
        return output;
    }
};
