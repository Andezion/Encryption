#include "gtest/gtest.h"
#include "Huffman.h"
#include <fstream>
#include <filesystem>

static std::string readFile(const std::string& path) 
{
    std::ifstream in(path, std::ios::binary);
    return std::string((std::istreambuf_iterator<char>(in)),
                        std::istreambuf_iterator<char>());
}

static void writeFile(const std::string& path, const std::string& content) 
{
    std::ofstream out(path, std::ios::binary);
    out.write(content.data(), content.size());
}

TEST(HuffmanTest, EmptyFileEncodeDecode) 
{
    std::string inFile = "/tmp/empty.txt";
    std::string encodedFile = "/tmp/empty.huff";
    std::string decodedFile = "/tmp/empty_out.txt";

    writeFile(inFile, "");

    Huffman hEnc(inFile, encodedFile);
    EXPECT_NO_THROW(hEnc.encode());

    Huffman hDec(encodedFile, decodedFile);
    EXPECT_NO_THROW(hDec.decode());

    EXPECT_EQ(readFile(decodedFile), "");
}

TEST(HuffmanTest, SmallStringEncodeDecode) 
{
    std::string input = "hello huffman";
    std::string inFile = "/tmp/in.txt";
    std::string encodedFile = "/tmp/in.huff";
    std::string decodedFile = "/tmp/out.txt";

    writeFile(inFile, input);

    Huffman hEnc(inFile, encodedFile);
    hEnc.encode();

    Huffman hDec(encodedFile, decodedFile);
    hDec.decode();

    std::string decoded = readFile(decodedFile);
    EXPECT_EQ(decoded, input);
}

TEST(HuffmanTest, BinaryDataEncodeDecode) 
{
    std::string input;
    for (int i = 0; i < 256; i++) 
    {
        input.push_back((char)i);
    }

    std::string inFile = "/tmp/bin.dat";
    std::string encodedFile = "/tmp/bin.huff";
    std::string decodedFile = "/tmp/bin_out.dat";

    writeFile(inFile, input);

    Huffman hEnc(inFile, encodedFile);
    hEnc.encode();

    Huffman hDec(encodedFile, decodedFile);
    hDec.decode();

    std::string decoded = readFile(decodedFile);
    EXPECT_EQ(decoded, input);
}

TEST(HuffmanTest, BinaryConversion) 
{
    EXPECT_EQ(Huffman::decimalToBinary(5), "00000101");
    EXPECT_EQ(Huffman::decimalToBinary(255), "11111111");

    EXPECT_EQ(Huffman::binaryToDecimal("00000101"), 5);
    EXPECT_EQ(Huffman::binaryToDecimal("11111111"), 255);
}

