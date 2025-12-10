#pragma once

#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using Position = std::pair<size_t, size_t>;

class FleissnerGrille
{
    size_t grid_size;
    std::vector<std::vector<int>> grille;
    std::vector<std::vector<char>> grid;
    bool clockwise;

public:
    explicit FleissnerGrille(const size_t size, const bool rotate_clockwise = true)
        : grid_size(size), clockwise(rotate_clockwise)
    {
        if (!isValidSize())
        {
            throw std::invalid_argument("Grid size must be even!");
        }

        grid.resize(grid_size, std::vector(grid_size, ' '));
        grille.resize(grid_size, std::vector(grid_size, -1));

        initializeGrille();
    }
    
    std::string encrypt(const std::string& message)
    {
        for (auto& row : grid)
        {
            std::fill(row.begin(), row.end(), ' ');
        }

        std::string padded_message = message;
        const size_t total_cells = grid_size * grid_size;
        while (padded_message.length() < total_cells)
        {
            padded_message += 'X';
        }
        
        size_t msg_idx = 0;
        std::vector<int> rotations = clockwise ? 
            std::vector{0, 1, 2, 3} : std::vector{0, 3, 2, 1};


        for (int rotation : rotations)
        {
            for (size_t i = 0; i < grid_size; ++i)
            {
                for (size_t j = 0; j < grid_size; ++j)
                {
                    if (grille[i][j] == rotation)
                    {
                        auto [x, y] = getRotatedPosition(i, j, rotation);
                        if (msg_idx < padded_message.length())
                        {
                            grid[x][y] = padded_message[msg_idx++];
                        }
                    }
                }
            }
        }

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
    
    std::string decrypt(const std::string& ciphertext)
    {
        if (ciphertext.length() != grid_size * grid_size)
        {
            throw std::invalid_argument("Invalid ciphertext length!");
        }

        size_t idx = 0;
        for (size_t i = 0; i < grid_size; ++i)
        {
            for (size_t j = 0; j < grid_size; ++j)
            {
                grid[i][j] = ciphertext[idx++];
            }
        }
        
        std::string message;
        std::vector<int> rotations = clockwise ? 
            std::vector{0, 1, 2, 3} : std::vector{0, 3, 2, 1};
        
        for (const int rotation : rotations)
        {
            for (size_t i = 0; i < grid_size; ++i)
            {
                for (size_t j = 0; j < grid_size; ++j)
                {
                    if (grille[i][j] == rotation)
                    {
                        auto [x, y] = getRotatedPosition(i, j, rotation);
                        message += grid[x][y];
                    }
                }
            }
        }
        
        return message;
    }
    
    std::vector<std::vector<char>> getGrid() const
    {
        return grid;
    }
    
    void printGrille() const
    {
        std::cout << "Grille pattern (rotation numbers):\n";
        for (const auto& row : grille)
        {
            for (const int val : row)
            {
                if (val == -1)
                {
                    std::cout << ". ";
                }
                else
                {
                    std::cout << val << ' ';
                }
            }
            std::cout << '\n';
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
    void initializeGrille()
    {
        const size_t half = grid_size / 2;
        std::vector<Position> quadrant_positions;

        for (size_t i = 0; i < half; ++i)
        {
            for (size_t j = 0; j < half; ++j)
            {
                quadrant_positions.push_back({i, j});
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(quadrant_positions.begin(), quadrant_positions.end(), gen);

        for (size_t idx = 0; idx < quadrant_positions.size(); ++idx)
        {
            auto [i, j] = quadrant_positions[idx];
            const int rotation = idx % 4;

            for (int rot = 0; rot < 4; ++rot)
            {
                if (auto [x, y] = getRotatedPosition(i, j, rot); grille[x][y] == -1)
                {
                    grille[x][y] = rotation;
                }
            }
        }
    }
    
    Position getRotatedPosition(size_t i, size_t j, const int rotation) const
    {
        switch (rotation)
        {
            case 0:
                return {i, j};
            case 1:
                return {j, grid_size - 1 - i};
            case 2:
                return {grid_size - 1 - i, grid_size - 1 - j};
            case 3:
                return {grid_size - 1 - j, i};
            default:
                return {i, j};
        }
    }
    
    bool isValidSize() const
    {
        return grid_size > 0 && grid_size % 2 == 0;
    }
};
