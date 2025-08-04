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

    int get_n() const
    {
        return n;
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

    static void get_cofactor(const std::vector<std::vector<int>> & mat,
                         std::vector<std::vector<int>> & temp,
                         const int p, const int q, const int n)
    {
        int i = 0, j = 0;

        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                if (row != p && col != q)
                {
                    temp[i][j++] = mat[row][col];
                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    static int determinant_of_matrix(const std::vector<std::vector<int>> &mat, const int n)
    {
        if (n == 1)
        {
            return mat[0][0];
        }

        int D = 0;
        int sign = 1;

        for (int f = 0; f < n; f++)
        {
            std::vector temp(n - 1, std::vector<int>(n - 1));
            get_cofactor(mat, temp, 0, f, n);
            D += sign * mat[0][f] * determinant_of_matrix(temp, n - 1);
            sign = -sign;
        }

        return D;
    }

    static int mod26(const int x)
    {
        return (x % 26 + 26) % 26;
    }

    static int mod_inverse(int a, const int m)
    {
        a = mod26(a);
        for (int x = 1; x < m; ++x)
        {
            if ((a * x) % m == 1)
            {
                return x;
            }
        }
        throw std::runtime_error("No modular inverse exists.");
    }

    static std::vector<std::vector<int>> helper(const std::vector<std::vector<int>> &mat, const int n)
    {
        std::vector adj(n, std::vector<int>(n));

        if (n == 1)
        {
            adj[0][0] = 1;
            return adj;
        }

        int sign{};
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::vector temp(n - 1, std::vector<int>(n - 1));
                get_cofactor(mat, temp, i, j, n);

                if ((i + j) % 2 == 0)
                {
                    sign = 1;
                }
                else
                {
                    sign = -1;
                }
                adj[j][i] = mod26(sign * determinant_of_matrix(temp, n - 1));
            }
        }
        return adj;
    }

    std::string decode(const std::string& encoded) const
    {
        std::string output;

        for (const char c : encoded)
        {
            if (std::isalpha(c))
            {
                output += std::toupper(c);
            }
        }

        if (output.size() % n != 0)
        {
            throw std::invalid_argument("Encoded text size is not divisible by matrix size");
        }

        const int det = mod26(determinant_of_matrix(matrix, n));
        const int det_inv = mod_inverse(det, 26);
        const std::vector<std::vector<int>> adj = helper(matrix, n);

        std::vector inverse(n, std::vector<int>(n));
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                inverse[i][j] = mod26(adj[i][j] * det_inv);
            }
        }

        std::string result;
        for (size_t i = 0; i < output.size(); i += n)
        {
            std::vector<int> vec(n);
            for (int j = 0; j < n; j++)
            {
                vec[j] = output[i + j] - 'A';
            }

            std::vector res(n, 0);
            for (int row = 0; row < n; ++row)
            {
                for (int col = 0; col < n; ++col)
                {
                    res[row] += inverse[row][col] * vec[col];
                }
            }

            for (const int val : res)
            {
                result += static_cast<char>(mod26(val) + 'A');
            }
        }

        return result;
    }
};
