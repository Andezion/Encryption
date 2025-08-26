#include <gtest/gtest.h>
#include "scytale.h"

TEST(ScytaleTest, BasicEncode)
{
    const scytale cipher("HELLOWORLD", 5);
    const std::string encoded = cipher.encode();
    EXPECT_EQ(encoded, "HWEOLRLLOD");
}

TEST(ScytaleTest, BasicDecode)
{
    const std::string encoded = "HWEOLRLLOD";
    const std::string decoded = scytale::decode(encoded, 5);
    EXPECT_EQ(decoded, "helloworld");
}

TEST(ScytaleTest, ReverseEncodeDecode)
{
    const scytale cipher("ATTACKATDAWN", 4, true);
    const std::string encoded = cipher.encode();
    EXPECT_EQ(encoded, "ATNTAWTKAACD");
    const std::string decoded = scytale::decode(encoded, 4, true);
    EXPECT_EQ(decoded, "attackatdawn");
}

TEST(ScytaleTest, PaddingX)
{
    const scytale cipher("ABC", 4);
    const std::string encoded = cipher.encode();
    EXPECT_EQ(encoded.size(), 4);
    const std::string decoded = scytale::decode(encoded, 4);
    EXPECT_EQ(decoded, "abc");
}

TEST(ScytaleTest, IgnoreNonAlpha)
{
    const scytale cipher("Hello, World! 123", 4);
    const std::string encoded = cipher.encode();
    const std::string decoded = scytale::decode(encoded, 4);
    EXPECT_EQ(decoded, "helloworld");
}


TEST(ScytaleTest, EncodeDecodeSymmetry)
{
    const std::string text = "cryptography";
    const scytale cipher(text, 3);
    const std::string encoded = cipher.encode();
    const std::string decoded = scytale::decode(encoded, 3);
    EXPECT_EQ(decoded, text);
}
