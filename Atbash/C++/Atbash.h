#pragma once

class atbash
{
    std::string input{};
public:
    explicit atbash(std::string input) :
    input(std::move(input)) {}

    std::string encode() const
    {
        std::string output = input;
        for (char & c : output)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>('z' - (c - 'a'));
            }
            else if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>('Z' - (c - 'A'));
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

    std::string decode(const std::string & encoded_version) const
    {
        std::string output = encoded_version;
        for (char & c : output)
        {
            if (c >= 'a' && c <= 'z')
            {
                c = static_cast<char>('z' - (c - 'a'));
            }
            else if (c >= 'A' && c <= 'Z')
            {
                c = static_cast<char>('Z' - (c - 'A'));
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
