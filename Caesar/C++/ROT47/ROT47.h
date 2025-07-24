#pragma once

#include <string>
#include <utility>

class rot47
{
    std::string input{};
public:
    explicit rot47(std::string  input)
        : input(std::move(input))
    {
        for (char i = 33; i <= 126; i++)
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
            if (c >= 33 && c <= 126)
            {
                c = static_cast<char>(33 + (c - 33 + 47) % 94);
            }
        }
        return output;
    }

    static std::string decode(const std::string & encoded_version)
    {
        std::string output = encoded_version;
        for (char & c : output)
        {
            if (c >= 33 && c <= 126)
            {
                c = static_cast<char>(33 + (c - 33 + 47) % 94);
            }
        }
        return output;
    }
};
