#pragma once

#include <string>
#include <vector>
#include <utility>
#include <random>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using Position = std::pair<size_t, size_t>;

class CardanGrille
{
    std::vector<Position> grille_positions;
    size_t grid_size;
    std::vector<std::vector<char>> grid;
public:
    CardanGrille(const size_t size, const std::vector<Position>& positions)
        : grille_positions(positions), grid_size(size)
    {
        grid.resize(grid_size, std::vector(grid_size, ' '));
        if (!validatePositions())
        {
            throw std::invalid_argument("Invalid grille positions!");
        }
    }

    explicit CardanGrille(const size_t size, const size_t num_holes = 8)
        : grid_size(size)
    {
        grid.resize(grid_size, std::vector(grid_size, ' '));
        generateRandomPositions(num_holes);
    }

    std::string encrypt(const std::string& message)
    {
        for (auto& row : grid)
        {
            std::fill(row.begin(), row.end(), ' ');
        }

        size_t msg_idx = 0;
        for (const auto& [x, y] : grille_positions)
        {
            if (msg_idx < message.length())
            {
                grid[x][y] = message[msg_idx++];
            }
        }

        fillRandomChars();

        std::string result;
        for (const auto& row : grid)
        {
            for (const char c : row)
            {
                result += c;
            }
        }
        return result;
    }

    std::string decrypt(const std::vector<std::vector<char>>& encrypted_grid)
    {
        std::string message;
        for (const auto& [x, y] : grille_positions)
        {
            message += encrypted_grid[x][y];
        }
        return message;
    }

    std::string decrypt(const std::string& ciphertext)
    {
        if (ciphertext.length() != grid_size * grid_size)
        {
            throw std::invalid_argument("Invalid ciphertext length!");
        }

        std::vector temp_grid(grid_size, std::vector<char>(grid_size));
        size_t idx = 0;
        for (size_t i = 0; i < grid_size; ++i)
        {
            for (size_t j = 0; j < grid_size; ++j)
            {
                temp_grid[i][j] = ciphertext[idx++];
            }
        }

        return decrypt(temp_grid);
    }

    std::vector<std::vector<char>> getGrid() const
    {
        return grid;
    }

    std::vector<Position> getGrillePositions() const
    {
        return grille_positions;
    }

    void setGrillePositions(const std::vector<Position>& positions)
    {
        grille_positions = positions;
        if (!validatePositions())
        {
            throw std::invalid_argument("Invalid grille positions!");
        }
    }

    void printGrid() const
    {
        for (const auto& row : grid)
        {
            for (const char c : row)
            {
                std::cout << c << ' ';
            }
            std::cout << '\n';
        }
    }

private:
    void fillRandomChars()
    {
        static constexpr char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

        for (size_t i = 0; i < grid_size; ++i)
        {
            for (size_t j = 0; j < grid_size; ++j)
            {
                if (grid[i][j] == ' ')
                {
                    grid[i][j] = charset[dis(gen)];
                }
            }
        }
    }

    bool validatePositions() const
    {
        for (const auto& [x, y] : grille_positions)
        {
            if (x >= grid_size || y >= grid_size)
            {
                return false;
            }
        }
        return true;
    }

    void generateRandomPositions(const size_t num_holes)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, grid_size - 1);

        grille_positions.clear();
        while (grille_positions.size() < num_holes)
        {
            if (Position pos = {dis(gen), dis(gen)};
                std::find(grille_positions.begin(), grille_positions.end(), pos)
                                                     == grille_positions.end())
            {
                grille_positions.push_back(pos);
            }
        }
    }
};
