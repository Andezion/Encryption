#pragma once

class substitution
{
    std::string input{};
    std::string key{};
public:
    substitution(std::string input, std::string key)
        : input(std::move(input)), key(std::move(key))
    {
        for (size_t i = 0; i < key.size(); i++)
        {
            std::cout << key[i] << " ";
        }
        std::cout << std::endl;
    }

    std::string encode() const
    {
        std::string output = input;

        for (size_t i = 0; i < input.size(); i++)
        {
            if (std::islower(input[i]))
            {
                output[i] = key[input[i] - 'a'];
            }
            else if(std::isupper(input[i]))
            {
                output[i] = key[input[i] - 'A'];
            }
            else
            {
                output[i] = input[i];
            }
        }

        return output;
    }

    std::string decode(const std::string& encoded_version) const
    {
        std::string output = encoded_version;

        for (size_t i = 0; i < encoded_version.size(); i++)
        {
            if (std::islower(encoded_version[i]))
            {
                if (const size_t pos = key.find(encoded_version[i]); pos != std::string::npos)
                {
                    output[i] = 'a' + pos;
                }
            }
            else if(std::isupper(encoded_version[i]))
            {
                if (const size_t pos = key.find(encoded_version[i]); pos != std::string::npos)
                {
                    output[i] = 'A' + pos;
                }
            }
            else
            {
                output[i] = encoded_version[i];
            }
        }

        return output;
    }
};
