#pragma once

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <stdexcept>

class ChessboardCipher
{
    size_t board_size;
    std::vector<std::vector<char>> board;

public:
    explicit ChessboardCipher(const size_t size = 8)
        : board_size(size)
    {
        board.resize(board_size, std::vector(board_size, ' '));
    }

    std::string encrypt(const std::string& message)
    {
        const std::string padded = padMessage(message);

        for (auto& row : board)
        {
            std::fill(row.begin(), row.end(), ' ');
        }

        size_t msg_idx = 0;

        for (size_t i = 0; i < board_size; ++i)
        {
            for (size_t j = 0; j < board_size; ++j)
            {
                if (isBlackSquare(i, j))
                {
                    if (msg_idx < 32)
                    {
                        board[i][j] = padded[msg_idx++];
                    }
                }
            }
        }

        for (size_t i = 0; i < board_size; ++i)
        {
            for (size_t j = 0; j < board_size; ++j)
            {
                if (!isBlackSquare(i, j))
                {
                    if (msg_idx < 64)
                    {
                        board[i][j] = padded[msg_idx++];
                    }
                }
            }
        }

        std::string result;
        for (const auto& row : board)
        {
            for (const char c : row)
            {
                result += c;
            }
        }
        return result;
    }

    std::string decrypt(const std::string& ciphertext)
    {
        if (ciphertext.length() != board_size * board_size)
        {
            throw std::invalid_argument("Invalid ciphertext length!");
        }

        size_t idx = 0;
        for (size_t i = 0; i < board_size; ++i)
        {
            for (size_t j = 0; j < board_size; ++j)
            {
                board[i][j] = ciphertext[idx++];
            }
        }

        std::string message;

        for (size_t i = 0; i < board_size; ++i)
        {
            for (size_t j = 0; j < board_size; ++j)
            {
                if (isBlackSquare(i, j))
                {
                    message += board[i][j];
                }
            }
        }

        for (size_t i = 0; i < board_size; ++i)
        {
            for (size_t j = 0; j < board_size; ++j)
            {
                if (!isBlackSquare(i, j))
                {
                    message += board[i][j];
                }
            }
        }

        return message;
    }

    std::vector<std::vector<char>> getBoard() const
    {
        return board;
    }

    void printBoard() const
    {
        for (const auto& row : board)
        {
            for (const char c : row)
            {
                std::cout << c << ' ';
            }
            std::cout << '\n';
        }
    }

private:
    std::string padMessage(const std::string& message) const
    {
        std::string result = message;
        const size_t total = board_size * board_size;
        while (result.length() < total)
        {
            result += 'X';
        }
        return result;
    }

    static bool isBlackSquare(const size_t i, const size_t j)
    {
        return (i + j) % 2 == 0;
    }
};
