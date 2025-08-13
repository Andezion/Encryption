#pragma once

class scytale
{
    std::string input{};
    size_t cols{};
    bool reverse{};

public:
    explicit scytale(std::string text, const size_t columns = 5, const bool reverse_order = false)
        : cols(columns), reverse(reverse_order)
    {
        for (const char c : text)
        {
            if (std::isalpha(static_cast<unsigned char>(c)))
            {
                input.push_back(std::tolower(static_cast<unsigned char>(c)));
            }
        }
    }

    std::string encode() const
    {
        std::vector<std::vector<char>> matrix;
        std::vector<char> row;

        for (char c : input)
        {
            row.push_back(c);
            if (row.size() == cols)
            {
                matrix.push_back(row);
                row.clear();
            }
        }

        if (!row.empty())
        {
            while (row.size() < cols)
            {
                row.push_back('x');
            }
            matrix.push_back(row);
        }

        std::string output;
        if (reverse)
        {
            for (int j = static_cast<int>(cols) - 1; j >= 0; --j)
            {
                for (size_t i = 0; i < matrix.size(); ++i)
                {
                    output.push_back(std::toupper(matrix[i][j]));
                }
            }
        }
        else
        {
            for (size_t j = 0; j < cols; ++j)
            {
                for (size_t i = 0; i < matrix.size(); ++i)
                {
                    output.push_back(std::toupper(matrix[i][j]));
                }
            }
        }

        return output;
    }

    static std::string decode(const std::string& encoded, const size_t columns, const bool reverse_order = false)
    {
        const size_t rows = encoded.size() / columns;
        std::vector<std::vector<char>> matrix(rows, std::vector<char>(columns));

        size_t index = 0;
        if (reverse_order)
        {
            for (int col = static_cast<int>(columns) - 1; col >= 0; col--)
            {
                for (size_t row = 0; row < rows; ++row)
                {
                    matrix[row][col] = std::tolower(static_cast<unsigned char>(encoded[index++]));
                }
            }
        }
        else
        {
            for (size_t col = 0; col < columns; col++)
            {
                for (size_t row = 0; row < rows; row++)
                {
                    matrix[row][col] = std::tolower(static_cast<unsigned char>(encoded[index++]));
                }
            }
        }

        std::string output;
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < columns; j++)
            {
                output.push_back(matrix[i][j]);
            }
        }

        while (!output.empty() && output.back() == 'x')
        {
            output.pop_back();
        }

        return output;
    }
};
