#pragma once

class double_transposition
{
    std::string input{};
    std::vector<size_t> index_row{};
    std::vector<size_t> index_col{};
public:
    double_transposition(std::string input, const std::vector<size_t> & index_row,
                         const std::vector<size_t> & index_col)
    : input(std::move(input)), index_row(index_row), index_col(index_col) {}

    std::string encode() const
    {
        std::string output;

        std::vector<std::vector<char>> matrix;
        std::vector<char> row;
        for (const char c : input)
        {
            if (std::isalpha(static_cast<unsigned char>(c)))
            {
                row.push_back(std::tolower(c));
            }

            if (row.size() == index_col.size())
            {
                matrix.push_back(row);
                row.clear();
            }
        }

        if (!row.empty())
        {
            while (row.size() < index_col.size())
            {
                row.push_back('x');
            }
            matrix.push_back(row);
        }

        std::vector<std::vector<char>> col_perm(matrix.size(), std::vector<char>(index_col.size()));
        for (size_t i = 0; i < matrix.size(); i++)
        {
            for (size_t j = 0; j < index_col.size(); j++)
            {
                col_perm[i][j] = matrix[i][index_col[j]];
            }
        }

        std::vector<std::vector<char>> row_perm(index_row.size(), std::vector<char>(index_col.size()));
        for (size_t i = 0; i < index_row.size(); i++)
        {
            row_perm[i] = col_perm[index_row[i]];
        }

        for (auto &r : row_perm)
        {
            for (const char c : r)
            {
                output.push_back(std::toupper(c));
            }
        }

        return output;
    }

    std::string decode(const std::string & encoded) const
    {
        std::string output;

        const int rows = encoded.size() / index_col.size();
        const int cols = index_col.size();

        std::vector<std::vector<char>> matrix(rows, std::vector<char>(cols));

        int idx = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                matrix[i][j] = std::tolower(encoded[idx++]);
            }
        }

        std::vector<std::vector<char>> row_perm(rows, std::vector<char>(cols));
        for (size_t i = 0; i < index_row.size(); i++)
        {
            row_perm[index_row[i]] = matrix[i];
        }

        std::vector<std::vector<char>> original(rows, std::vector<char>(cols));
        for (size_t j = 0; j < index_col.size(); j++)
        {
            for (size_t i = 0; i < rows; i++)
            {
                original[i][index_col[j]] = row_perm[i][j];
            }
        }

        for (const auto & r : original)
        {
            for (const char c : r)
            {
                output.push_back(c);
            }
        }

        return output;
    }

};
