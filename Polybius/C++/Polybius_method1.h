#pragma once

class polybius_method1
{
    std::string input{};
    char storage[5][5]{};
public:
    explicit polybius_method1(std::string input)
        : input(std::move(input))
    {
        char letter = 'A';
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                if (letter == 'J')
                {
                    letter++;
                }
                storage[i][j] = letter;
                letter++;
            }
        }
    }

    std::string encode() const
    {
        std::string output{};
        for (char c : input)
        {
            if (std::isalpha(c))
            {
                c = toupper(c);
            }

            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (c == storage[i][j])
                    {
                        if (i == 4)
                        {
                            output.push_back(storage[0][j]);
                        }
                        else
                        {
                            output.push_back(storage[i + 1][j]);
                        }
                    }
                }
            }
        }
        return output;
    }

    std::string decode(const std::string & encoded_version) const
    {
        std::string output{};
        for (char c : encoded_version)
        {
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (c == storage[i][j])
                    {
                        if (i == 0)
                        {
                            output.push_back(storage[4][j]);
                        }
                        else
                        {
                            output.push_back(storage[i - 1][j]);
                        }
                    }
                }
            }
        }
        return output;
    }
};
