#include <gtest/gtest.h>
#include "Vigenere.h"

TEST(VigenereTest, EncodeDecodeBasic) 
{
    vigenere v("attackatdawn", "lemon");
    std::string encoded = v.encode();
    EXPECT_EQ(encoded, "lxfopvefrnhr");
    EXPECT_EQ(v.decode(encoded), "attackatdawn");
}

TEST(VigenereTest, EncodeWithUppercase) 
{
    vigenere v("AttackAtDawn", "LEMON");
    std::string encoded = v.encode();
    EXPECT_EQ(encoded, "LxfopvEfRnhr");
    EXPECT_EQ(v.decode(encoded), "AttackAtDawn");
}

TEST(VigenereTest, EncodeWithSpacesAndPunctuation) 
{
    vigenere v("hello, world!", "key");
    std::string encoded = v.encode();
    EXPECT_EQ(encoded, "rijvs, uyvjn!");
    EXPECT_EQ(v.decode(encoded), "hello, world!");
}

TEST(VigenereTest, LongKeyShortText) 
{
    vigenere v("hi", "superlongkey");
    std::string encoded = v.encode();
    EXPECT_EQ(encoded, "zj");
    EXPECT_EQ(v.decode(encoded), "hi");
}

TEST(VigenereTest, SingleLetterKey) 
{
    vigenere v("abcxyz", "b"); 
    std::string encoded = v.encode();
    EXPECT_EQ(encoded, "bcdyza");
    EXPECT_EQ(v.decode(encoded), "abcxyz");
}

TEST(VigenereTest, EmptyInput) 
{
    vigenere v("", "key");
    EXPECT_EQ(v.encode(), "");
    EXPECT_EQ(v.decode(""), "");
}

TEST(VigenereTest, MixedCaseAndSymbols) 
{
    vigenere v("TeSt123!", "key");
    std::string encoded = v.encode();
    EXPECT_EQ(encoded, "DiCx123!");
    EXPECT_EQ(v.decode(encoded), "TeSt123!");
}
