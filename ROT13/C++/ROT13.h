#pragma once
#include <string>

class rot13
{
    std::string input{};
public:
    explicit rot13(std::string input)
        : input(std::move(input))
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
        for (char& c : output)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>('a' + (c - 'a' + 13) % 26);
            }
            else if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>('A' + (c - 'A' + 13) % 26);
            }
        }
        return output;
    }

    [[nodiscard]] static std::string decode(const std::string & encoded_version)
    {
        std::string output = encoded_version;
        for (char& c : output)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>('a' + (c - 'a' + 13) % 26);
            }
            else if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>('A' + (c - 'A' + 13) % 26);
            }
        }
        return output;
    }
};
