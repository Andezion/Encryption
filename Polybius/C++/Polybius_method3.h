class polybius_method3
{
    std::string input{};
    char storage[5][5]{};
public:
    explicit polybius_method3(std::string input) : input(std::move(input))
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
        std::vector<int> pairs;
        for (char c : input)
        {
            if (std::isalpha(static_cast<unsigned char>(c)))
            {
                c = std::toupper(static_cast<unsigned char>(c));
                if (c == 'J')
                {
                    c = 'I';
                }
            }
            else
            {
                continue;
            }

            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (c == storage[i][j])
                    {
                        pairs.push_back(i);
                        pairs.push_back(j);
                    }
                }
            }
        }

        std::vector<int> shifted(pairs.size());

        for (size_t i = 0; i < pairs.size(); i++)
        {
            shifted[i] = pairs[(i + 1) % pairs.size()];
            //std::cout << shifted[i] << ' ';
        }

        //std::cout << std::endl;

        std::string output{};
        for (size_t i = 0; i < shifted.size(); i += 2)
        {
            output += storage[shifted[i]][shifted[i + 1]];
        }

        return output;
    }

    std::string decode(const std::string & encoded_version) const
    {
        std::vector<int> pairs;
        for (char c : encoded_version)
        {
            if (std::isalpha(static_cast<unsigned char>(c)))
            {
                c = std::toupper(static_cast<unsigned char>(c));
                if (c == 'J')
                {
                    c = 'I';
                }
            }
            else
            {
                continue;
            }

            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 5; j++)
                {
                    if (c == storage[i][j])
                    {
                        pairs.push_back(i);
                        pairs.push_back(j);
                    }
                }
            }
        }

        std::vector<int> shifted(pairs.size());

        for (size_t i = 0; i < pairs.size(); i++)
        {
            shifted[i] = pairs[(i - 1) % pairs.size()];
            //std::cout << shifted[i] << ' ';
        }

        std::string output{};
        for (size_t i = 0; i < shifted.size(); i += 2)
        {
            output += storage[shifted[i]][shifted[i + 1]];
        }

        return output;
    }
};

