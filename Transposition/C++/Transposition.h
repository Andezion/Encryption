#pragma once

class transposition
{
    std::string input{};
public:
    explicit transposition(const std::string &input) : input(std::move(input)) {}

    std::string encode() const
    {
        std::string output;

        std::vector<std::vector<char>> matrix;
        std::vector<char> row;
        for (int i = 0; i < input.size(); i++)
        {
            if (std::isalpha(input[i]))
            {
                row.push_back(std::tolower(input[i]));
            }
            if (row.size() == 5)
            {
                matrix.push_back(row);
                row.clear();
            }
        }

        for (int j = matrix[0].size() - 1; j >= 0; j--)
        {
            for (int i = 0; i < matrix.size(); i++)
            {
                output.push_back(std::toupper(matrix[i][j]));
            }
        }

        return output;
    }

    static std::string decode(const std::string& encoded)
    {
        std::string output;

        const int rows = encoded.size() / 5;
        std::vector matrix(rows, std::vector<char>(5));

        int index = 0;
        for (int col = 4; col >= 0; col--)
        {
            for (int row = 0; row < rows; row++)
            {
                matrix[row][col] = encoded[index++];
            }
        }

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                output.push_back(matrix[i][j]);
            }
        }

        return output;
    }
};
