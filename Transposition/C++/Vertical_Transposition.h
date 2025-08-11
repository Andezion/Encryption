#pragma once

class vertical_transposition
{
    std::string input{};
    std::vector<size_t> indexes{};
public:
    vertical_transposition(std::string  input, const std::vector<size_t> & indexes)
    : input(std::move(input)), indexes(indexes) {}

    std::string encode() const
    {
        std::string output;

        std::vector<std::vector<char>> matrix;
        std::vector<char> row;

        for (const char i : input)
        {
            if (std::isalpha(i))
            {
                row.push_back(std::tolower(i));
            }
            if (row.size() == 5)
            {
                matrix.push_back(row);
                row.clear();
            }
        }

        for (auto & index : indexes)
        {
            for (auto & i : matrix)
            {
                output.push_back(std::toupper(i[index]));
            }
        }

        return output;
    }

    std::string decode(const std::string & encoded)
    {
        std::string output;

        const int rows = encoded.size() / 5;
        std::vector<std::vector<char>> matrix(rows, std::vector<char>(5));

        int index = 0;
        for (int col = 0; col < indexes.size(); col++)
        {
            for (int row = 0; row < rows; row++)
            {
                matrix[row][indexes[col]] = encoded[index++];
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
