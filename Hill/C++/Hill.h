#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>

class hill
{
    int n{};
    std::vector<std::vector<int>> matrix{};
    std::string input{};
public:
    hill(const int n, std::string input, const std::string &key)
        : n(n), input(std::move(input))
    {
        if (key.size() != n * n)
        {
            throw std::invalid_argument("Invalid key length");
        }

        matrix.resize(n, std::vector<int>(n));
        for (int i = 0; i < n * n; i++)
        {
            const char c = std::toupper(key[i]);
            if (c < 'A' || c > 'Z')
            {
                throw std::invalid_argument("Invalid character in key");
            }
            matrix[i / n][i % n] = c - 'A';
        }
    }

    std::string encode() const
    {
        std::string clean_input;
        for (const char c : input)
        {
            if (std::isalpha(c))
            {
                clean_input += std::toupper(c);
            }
        }

        while (clean_input.size() % n != 0)
        {
            clean_input += 'X';
        }

        std::string result;
        for (size_t i = 0; i < clean_input.size(); i += n)
        {
            std::vector<int> vec(n);
            for (int j = 0; j < n; j++)
            {
                vec[j] = clean_input[i + j] - 'A';
            }

            std::vector res(n, 0);
            for (int row = 0; row < n; ++row)
            {
                for (int col = 0; col < n; ++col)
                {
                    res[row] += matrix[row][col] * vec[col];
                }
            }

            for (const int val : res)
            {
                result += static_cast<char>(val % 26 + 'A');
            }
        }

        return result;
    }

    std::string decode(const std::string& encoded) const
    {
        
    }
};
