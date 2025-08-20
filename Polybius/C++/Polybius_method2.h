class polybius_method2
{
    std::string input{};
    char storage[5][5]{};
public:
    explicit polybius_method2(std::string input) : input(std::move(input))
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
                        const int number = (i + 1) * 10 + (j + 1);
                        output += std::to_string(number);
                        output.push_back(' ');
                    }
                }
            }
        }

        return output;
    }

    std::string decode(const std::string & encoded_version) const
    {
        std::string output{};

        std::istringstream iss(encoded_version);
        std::string code;

        while (iss >> code)
        {
            output.push_back(storage[code[0] - '0' - 1][code[1] - '0' - 1]);
        }

        return output;
    }
};
