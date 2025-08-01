#pragma once
#include <set>
#include <unordered_set>

class playfair
{
    std::string key{};
    std::string input{};

    char matrix[5][5] = {};
public:
    playfair(std::string key, std::string input)
        : key(std::move(key)), input(std::move(input))
    {
        std::string combined;
        std::unordered_set<char> seen;

        for (char c : this->key)
        {
            c = std::toupper(c);
            if (c == 'J')
            {
                c = 'I';
            }
            if (!seen.count(c) && std::isalpha(c))
            {
                combined += c;
                seen.insert(c);
            }
        }

        for (char c = 'A'; c <= 'Z'; c++)
        {
            if (c == 'J')
            {
                continue;
            }
            if (!seen.count(c))
            {
                combined += c;
                seen.insert(c);
            }
        }

        for (int i = 0; i < 25; i++)
        {
            matrix[i / 5][i % 5] = combined[i];
        }

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                std::cout << matrix[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }

    std::pair<int, int> find_position(const char c) const
    {
        for (int row = 0; row < 5; ++row)
        {
            for (int col = 0; col < 5; ++col)
            {
                if (matrix[row][col] == c)
                {
                    return {row, col};
                }
            }
        }
        return {-1, -1}; // error
    }

    std::string encode() const
    {
        std::string cleaned;
        for (char c : input)
        {
            c = std::toupper(c);
            if (c == 'J')
            {
                c = 'I';
            }
            if (std::isalpha(c))
            {
                cleaned += c;
            }
        }

        for (size_t i = 0; i < cleaned.size(); i += 2)
        {
            if (i + 1 == cleaned.size())
            {
                break;
            }
            if (cleaned[i] == cleaned[i + 1])
            {
                cleaned.insert(cleaned.begin() + i + 1, 'X');
            }
        }

        if (cleaned.size() % 2 != 0)
        {
            cleaned += 'X';
        }

        std::string output;

        for (size_t i = 0; i < cleaned.size(); i += 2)
        {
            const char a = cleaned[i];
            const char b = cleaned[i + 1];

            auto [row1, col1] = find_position(a);
            if (auto [row2, col2] = find_position(b); row1 == row2)
            {
                output += matrix[row1][(col1 + 1) % 5];
                output += matrix[row2][(col2 + 1) % 5];
            }
            else if (col1 == col2)
            {
                output += matrix[(row1 + 1) % 5][col1];
                output += matrix[(row2 + 1) % 5][col2];
            }
            else
            {
                output += matrix[row1][col2];
                output += matrix[row2][col1];
            }
        }

        return output;
    }

    std::string decode(const std::string& encoded_version) const
    {
        std::string output;

        for (size_t i = 0; i < encoded_version.size(); i += 2)
        {
            const char a = encoded_version[i];
            const char b = encoded_version[i + 1];

            auto [row1, col1] = find_position(a);
            if (auto [row2, col2] = find_position(b); row1 == row2)
            {
                output += matrix[row1][(col1 + 4) % 5];
                output += matrix[row2][(col2 + 4) % 5];
            }
            else if (col1 == col2)
            {
                output += matrix[(row1 + 4) % 5][col1];
                output += matrix[(row2 + 4) % 5][col2];
            }
            else
            {
                output += matrix[row1][col2];
                output += matrix[row2][col1];
            }
        }

        return output;
    }
};
