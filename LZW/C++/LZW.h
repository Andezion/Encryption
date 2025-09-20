#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <unordered_map>

class LZW {
public:
    static std::string encode(const std::string& input) {
        const std::vector<unsigned int> indices = encoding(input);
        std::string output(BYTES_PER_INDEX * indices.size(), '\0');

        for (size_t i = 0; i < indices.size(); i++) {
            unsigned int index = indices[i];
            for (uint8_t j = 0; j < BYTES_PER_INDEX; j++) {
                const char outChar = index % 256;
                index >>= 8;
                output[i * BYTES_PER_INDEX + j] = outChar;
            }
        }

        return output;
    }

    static std::string decode(const std::string& input) {
        std::vector<unsigned int> indices(input.size() / BYTES_PER_INDEX);

        for (size_t i = 0; i < input.size(); i += BYTES_PER_INDEX) {
            unsigned int newIndex = 0;
            for (size_t j = 0; j < BYTES_PER_INDEX; j++) {
                newIndex <<= 8;
                newIndex += static_cast<unsigned char>(input[i + 2 - j]);
            }
            indices[i / BYTES_PER_INDEX] = newIndex;
        }

        return decoding(indices);
    }

private:
    static constexpr int BYTES_PER_INDEX = 3;

    static std::vector<unsigned int> encoding(const std::string& s) {
        std::unordered_map<std::string, unsigned int> dictionary;
        std::vector<unsigned int> output;

        for (int i = 0; i <= 255; i++) {
            dictionary[std::string(1, static_cast<char>(i))] = i;
        }

        std::string oldChars(1, s[0]);
        unsigned int code = 256;

        for (size_t i = 0; i < s.length(); i++) {
            std::string newChars;
            if (i != s.length() - 1) {
                newChars += s[i + 1];
            }

            if (dictionary.find(oldChars + newChars) != dictionary.end()) {
                oldChars += newChars;
            } else {
                output.push_back(dictionary[oldChars]);
                dictionary[oldChars + newChars] = code++;
                oldChars = newChars;
            }
        }

        output.push_back(dictionary[oldChars]);
        return output;
    }

    static std::string decoding(const std::vector<unsigned int>& indices) {
        std::unordered_map<unsigned int, std::string> dictionary;
        for (int i = 0; i <= 255; i++) {
            dictionary[i] = std::string(1, static_cast<char>(i));
        }

        int old = indices[0];
        std::string s = dictionary[old];
        std::string c(1, s[0]);
        std::string decoded = s;

        int nextCode = 256;
        for (size_t i = 0; i < indices.size() - 1; i++) {
            const int currentIndex = indices[i + 1];
            if (dictionary.find(currentIndex) == dictionary.end()) {
                s = dictionary[old] + c;
            } else {
                s = dictionary[currentIndex];
            }

            decoded += s;
            c = std::string(1, s[0]);
            dictionary[nextCode++] = dictionary[old] + c;
            old = currentIndex;
        }

        return decoded;
    }
};
