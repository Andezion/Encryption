#pragma once
#include <string>
#include <cctype>
#include <iostream>

class vigenere
{
    std::string input{};
    std::string key{};
    char table[26][26]{};

public:
    vigenere(std::string input, std::string key)
        : input(std::move(input)), key(std::move(key))
    {
        for (int i = 0; i < 26; i++)
        {
            for (int j = 0; j < 26; j++)
            {
                table[i][j] = 'a' + (i + j) % 26;
            }
        }
    }

    std::string encode() const
    {
        std::string output = input;
        const size_t key_len = key.size();
        size_t key_index = 0;

        for (size_t i = 0; i < input.size(); i++)
        {
            if (const char c = input[i]; std::islower(c))
            {
                const int row = c - 'a';
                const int col = std::tolower(key[key_index % key_len]) - 'a';
                output[i] = table[row][col];
                key_index++;
            }
            else if (std::isupper(c))
            {
                const int row = std::tolower(c) - 'a';
                const int col = std::tolower(key[key_index % key_len]) - 'a';
                output[i] = std::toupper(table[row][col]);
                key_index++;
            }
            else
            {
                output[i] = c;
            }
        }

        return output;
    }

    std::string decode(const std::string& encoded_version) const
    {
        std::string output = encoded_version;
        const size_t key_len = key.size();
        size_t key_index = 0;

        for (size_t i = 0; i < encoded_version.size(); i++)
        {
            if (const char c = encoded_version[i]; std::islower(c))
            {
                const int row = std::tolower(key[key_index % key_len]) - 'a';
                for (int col = 0; col < 26; col++)
                {
                    if (table[row][col] == c)
                    {
                        output[i] = 'a' + col;
                        break;
                    }
                }
                key_index++;
            }
            else if (std::isupper(c))
            {
                const int row = std::tolower(key[key_index % key_len]) - 'a';
                const char lower_c = std::tolower(c);
                for (int col = 0; col < 26; col++)
                {
                    if (table[row][col] == lower_c)
                    {
                        output[i] = std::toupper('a' + col);
                        break;
                    }
                }
                key_index++;
            }
            else
            {
                output[i] = c;
            }
        }

        return output;
    }
};
