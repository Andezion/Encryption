#include <gtest/gtest.h>
#include "Caesar.h" 

Caesar make_caesar(const char* str) 
{
    Caesar c{};
    c.input = strdup(str);
    c.len = strlen(c.input) % 26;
    return c;
}

TEST(CaesarTest, SimpleLowercaseEncoding) 
{
    auto c = make_caesar("abc");
    char* encoded = c.encode();
    EXPECT_STREQ(encoded, "nop"); 
    free(encoded);
    free(c.input);
}

TEST(CaesarTest, SimpleUppercaseEncoding) 
{
    auto c = make_caesar("XYZ");
    char* encoded = c.encode();
    EXPECT_STREQ(encoded, "KLM");
    free(encoded);
    free(c.input);
}

TEST(CaesarTest, EncodeDecodeSymmetry) 
{
    auto c = make_caesar("HelloWorld");
    char* encoded = c.encode();
    char* decoded = c.decode(encoded);
    EXPECT_STREQ(decoded, "HelloWorld");
    free(encoded);
    free(decoded);
    free(c.input);
}

TEST(CaesarTest, NonAlphabeticCharactersStaySame) 
{
    auto c = make_caesar("123!@#");
    char* encoded = c.encode();
    EXPECT_STREQ(encoded, "123!@#");
    free(encoded);
    free(c.input);
}

TEST(CaesarTest, EmptyString) 
{
    auto c = make_caesar("");
    char* encoded = c.encode();
    EXPECT_STREQ(encoded, "");
    free(encoded);
    free(c.input);
}

TEST(CaesarTest, LongStringModuloLength) 
{
    std::string input(30, 'a'); 
    auto c = make_caesar(input.c_str());
    char* encoded = c.encode();
    EXPECT_EQ(strlen(encoded), 4);
    free(encoded);
    free(c.input);
}

TEST(CaesarTest, DecodeKnownValue) 
{
    auto c = make_caesar("Test");
    char* encoded = c.encode();
    char* decoded = c.decode(encoded);
    EXPECT_STREQ(decoded, "Test");
    free(encoded);
    free(decoded);
    free(c.input);
}
