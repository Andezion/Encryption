#pragma once

#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <bitset>
#include <iostream>
#include <cstdint>

class Huffman
{
    struct Node
    {
        unsigned char id;
        uint64_t freq;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(const unsigned char ch = 0, const uint64_t f = 0)
            : id(ch), freq(f), left(nullptr), right(nullptr) {}
    };

    struct Compare
    {
        bool operator()(const std::unique_ptr<Node>& a, const std::unique_ptr<Node>& b) const
        {
            return a->freq > b->freq;
        }
    };

    std::string inputFileName;
    std::string outputFileName;

    std::unique_ptr<Node> root;
    std::unordered_map<unsigned char, std::string> codes;
    uint64_t freqTable[256] = {};

    void countFrequencies()
    {
        std::ifstream in(inputFileName, std::ios::binary);
        if (!in)
        {
            throw std::runtime_error("Ошибка: не удалось открыть файл " + inputFileName);
        }

        unsigned char ch;
        while (in.read(reinterpret_cast<char*>(&ch), 1))
        {
            freqTable[ch]++;
        }
    }

    std::unique_ptr<Node> buildTree()
    {
        std::priority_queue<
            std::unique_ptr<Node>,
            std::vector<std::unique_ptr<Node>>,
            Compare
        > pq;

        for (int i = 0; i < 256; ++i)
        {
            if (freqTable[i] > 0)
            {
                pq.push(std::make_unique<Node>((unsigned char)i, freqTable[i]));
            }
        }

        if (pq.empty())
        {
            return nullptr;
        }

        if (pq.size() == 1)
        {
            auto single = std::move(pq.top()); pq.pop();
            auto parent = std::make_unique<Node>(0, single->freq);
            parent->left = std::move(single);
            pq.push(std::move(parent));
        }

        while (pq.size() > 1)
        {
            auto left = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();
            auto right = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();

            auto parent = std::make_unique<Node>(0, left->freq + right->freq);
            parent->left = std::move(left);
            parent->right = std::move(right);

            pq.push(std::move(parent));
        }

        return std::move(const_cast<std::unique_ptr<Node>&>(pq.top()));
    }

    void generateCodes(const Node* node, const std::string& currentCode)
    {
        if (!node) return;

        if (!node->left && !node->right)
        {
            codes[node->id] = currentCode.empty() ? "0" : currentCode; 
        }
        generateCodes(node->left.get(), currentCode + "0");
        generateCodes(node->right.get(), currentCode + "1");
    }

    static std::string decimalToBinary(const int value) {
        return std::bitset<8>(value).to_string();
    }

    static int binaryToDecimal(const std::string& binary)
    {
        return std::stoi(binary, nullptr, 2);
    }

public:
    Huffman(const std::string& input, const std::string& output)
        : inputFileName(input), outputFileName(output) {}

    void encode()
    {
        countFrequencies();
        root = buildTree();
        if (!root)
        {
            std::cerr << "Файл пустой, нечего кодировать\n";
            return;
        }

        generateCodes(root.get(), "");

        std::ifstream in(inputFileName, std::ios::binary);
        if (!in)
        {
            throw std::runtime_error("Ошибка: не удалось открыть файл " + inputFileName);
        }
        std::ofstream out(outputFileName, std::ios::binary);
        if (!out)
        {
            throw std::runtime_error("Ошибка: не удалось создать файл " + outputFileName);
        }

        out.write(reinterpret_cast<const char*>(freqTable), sizeof(freqTable));

        in.seekg(0, std::ios::end);
        uint64_t originalSize = in.tellg();
        in.seekg(0, std::ios::beg);
        out.write(reinterpret_cast<const char*>(&originalSize), sizeof(originalSize));

        std::string bitBuffer;
        unsigned char ch;
        while (in.read(reinterpret_cast<char*>(&ch), 1))
        {
            bitBuffer += codes[ch];
            while (bitBuffer.size() >= 8)
            {
                std::string byteStr = bitBuffer.substr(0, 8);
                unsigned char byte = static_cast<unsigned char>(binaryToDecimal(byteStr));
                out.put(byte);
                bitBuffer.erase(0, 8);
            }
        }

        if (!bitBuffer.empty())
        {
            bitBuffer.append(8 - bitBuffer.size(), '0');
            unsigned char byte = static_cast<unsigned char>(binaryToDecimal(bitBuffer));
            out.put(byte);
        }
    }

    void decode()
    {
        std::ifstream in(inputFileName, std::ios::binary);
        if (!in)
        {
            throw std::runtime_error("Ошибка: не удалось открыть файл " + inputFileName);
        }
        std::ofstream out(outputFileName, std::ios::binary);
        if (!out)
        {
            throw std::runtime_error("Ошибка: не удалось создать файл " + outputFileName);
        }

        in.read(reinterpret_cast<char*>(freqTable), sizeof(freqTable));

        uint64_t originalSize = 0;
        in.read(reinterpret_cast<char*>(&originalSize), sizeof(originalSize));

        root = buildTree();
        if (!root)
        {
            std::cerr << "Файл пустой, нечего декодировать\n";
            return;
        }

        Node* current = root.get();
        uint64_t decodedCount = 0;

        char byte;
        while (in.get(byte))
        {
            std::string bits = decimalToBinary(static_cast<unsigned char>(byte));
            for (char bit : bits)
            {
                current = bit == '0' ? current->left.get() : current->right.get();

                if (current && !current->left && !current->right)
                {
                    out.put(static_cast<char>(current->id));
                    decodedCount++;
                    current = root.get();

                    if (decodedCount >= originalSize)
                    {
                        return; 
                    }
                }
            }
        }
    }
};
