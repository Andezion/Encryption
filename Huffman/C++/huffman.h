#pragma once

#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <memory>
#include <unordered_map>
#include <bitset>
#include <iostream>

class Huffman {
    struct Node {
        char id;
        int freq;
        std::string code;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(const char ch = '\0', const int f = 0) : id(ch), freq(f), left(nullptr), right(nullptr) {}
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
    std::unordered_map<char, std::string> codes;
    int freqTable[128] = {};

    void countFrequencies()
    {
        std::ifstream in(inputFileName, std::ios::binary);
        char ch;
        while (in.get(ch))
        {
            freqTable[static_cast<unsigned char>(ch)]++;
        }
    }

    void buildTree()
    {
        std::priority_queue<std::unique_ptr<Node>, std::vector<std::unique_ptr<Node>>, Compare> pq;

        for (int i = 0; i < 128; ++i)
        {
            if (freqTable[i] > 0)
            {
                pq.push(std::make_unique<Node>(static_cast<char>(i), freqTable[i]));
            }
        }

        while (pq.size() > 1)
        {
            auto left = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();
            auto right = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();

            auto parent = std::make_unique<Node>('\0', left->freq + right->freq);
            parent->left = std::move(left);
            parent->right = std::move(right);
            pq.push(std::move(parent));
        }

        if (!pq.empty())
        {
            root = std::move(const_cast<std::unique_ptr<Node>&>(pq.top())); pq.pop();
        }
    }

    void generateCodes(const Node* node, const std::string& currentCode)
    {
        if (!node)
        {
            return;
        }
        if (!node->left && !node->right)
        {
            codes[node->id] = currentCode;
        }
        generateCodes(node->left.get(), currentCode + "0");
        generateCodes(node->right.get(), currentCode + "1");
    }

    static std::string decimalToBinary(const int value)
    {
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
        buildTree();
        generateCodes(root.get(), "");

        std::ifstream in(inputFileName, std::ios::binary);
        std::ofstream out(outputFileName, std::ios::binary);

        for (int i = 0; i < 128; ++i)
        {
            out.write(reinterpret_cast<const char*>(&freqTable[i]), sizeof(int));
        }

        std::string bitBuffer;
        char ch;
        while (in.get(ch))
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

        in.close();
        out.close();
    }

    void decode()
    {
        std::ifstream in(inputFileName, std::ios::binary);
        std::ofstream out(outputFileName, std::ios::binary);

        for (int i = 0; i < 128; ++i)
        {
            in.read(reinterpret_cast<char*>(&freqTable[i]), sizeof(int));
        }

        buildTree();

        std::string bitStream;
        char byte;
        while (in.get(byte))
        {
            bitStream += decimalToBinary(static_cast<unsigned char>(byte));
        }

        Node* current = root.get();
        for (char bit : bitStream)
        {
            if (!current)
            {
                break;
            }
            current = bit == '0' ? current->left.get() : current->right.get();

            if (current && !current->left && !current->right)
            {
                out.put(current->id);
                current = root.get();
            }
        }

        in.close();
        out.close();
    }
};
