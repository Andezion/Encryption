#pragma once

class morse
{
    std::string input{};
    std::vector<std::pair<std::pair<char, char>, std::string>> morse_storage = {
        {{'A', 'a'}, "._"},
        {{'B', 'b'}, "_..."},
        {{'C', 'c'}, "_._."},
        {{'D', 'd'}, "_.."},
        {{'E', 'e'}, "."},
        {{'F', 'f'}, ".._."},
        {{'G', 'g'}, "__."},
        {{'H', 'h'}, "...."},
        {{'I', 'i'}, ".."},
        {{'J', 'j'}, ".___"},
        {{'K', 'k'}, "_._"},
        {{'L', 'l'}, "._.."},
        {{'M', 'm'}, "__"},
        {{'N', 'n'}, "_."},
        {{'O', 'o'}, "___"},
        {{'P', 'p'}, ".__."},
        {{'Q', 'q'}, "__._"},
        {{'R', 'r'}, "._."},
        {{'S', 's'}, "..."},
        {{'T', 't'}, "_"},
        {{'U', 'u'}, ".._"},
        {{'V', 'v'}, "..._"},
        {{'W', 'w'}, ".__"},
        {{'X', 'x'}, "_.._"},
        {{'Y', 'y'}, "_.__"},
        {{'Z', 'z'}, "__.."},
        {{'0', '0'}, "_____"},
        {{'1', '1'}, ".____"},
        {{'2', '2'}, "..___"},
        {{'3', '3'}, "...__"},
        {{'4', '4'}, "...._"},
        {{'5', '5'}, "....."},
        {{'6', '6'}, "_...."},
        {{'7', '7'}, "__..."},
        {{'8', '8'}, "___.."},
        {{'9', '9'}, "____."},
        {{'.', '.'}, "._._._"},
        {{',', ','}, "__..__"},
        {{'?', '?'}, "..__.."},
        {{'!', '!'}, "_._.__"},
        {{' ', ' '}, "/"} // разделитель слов
    };

public:
    explicit morse(std::string input)
        : input(std::move(input)) {}

    std::string encode() const
    {
        std::string output{};

        for (const char & i : input)
        {
            for (const auto &[fst, snd] : morse_storage)
            {
                if (fst.first == i || fst.second == i)
                {
                    output += snd;
                    output += ' ';
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
            if (code == "/")
            {
                output += ' ';
                continue;
            }

            for (const auto &[fst, snd] : morse_storage)
            {
                if (snd == code)
                {
                    output += fst.first;
                    break;
                }
            }
        }

        return output;
    }
};
