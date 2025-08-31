#include <gtest/gtest.h>
#include "Caesar.h"

TEST(CaesarTest, EncodeBasicLowercase)
{
    caesar c("abc", 3);
    EXPECT_EQ(c.encode(), "def");
}

TEST(CaesarTest, EncodeBasicUppercase)
{
    caesar c("XYZ", 3);
    EXPECT_EQ(c.encode(), "ABC");
}

TEST(CaesarTest, DecodeBasicLowercase)
{
    caesar c("def", 3);
    EXPECT_EQ(c.decode("def"), "abc");
}

TEST(CaesarTest, DecodeBasicUppercase)
{
    caesar c("ABC", 3);
    EXPECT_EQ(c.decode("ABC"), "XYZ");
}

TEST(CaesarTest, MixedCaseEncodingDecoding)
{
    caesar c("HelloWorld", 5);
    std::string encoded = c.encode();
    EXPECT_EQ(encoded, "MjqqtBtwqi");
    EXPECT_EQ(c.decode(encoded), "HelloWorld");
}

TEST(CaesarTest, PreserveSpacesAndDigits)
{
    caesar c("hello 123!", 4);
    EXPECT_EQ(c.encode(), "lipps 123!");
    EXPECT_EQ(c.decode("lipps 123!"), "hello 123!");
}

TEST(CaesarTest, InvalidCharacterThrows)
{
    caesar c("Привет", 3); 
    EXPECT_THROW(c.encode(), std::invalid_argument);
}

TEST(CaesarTest, KeyWrapAround)
{
    caesar c("z", 1);
    EXPECT_EQ(c.encode(), "a");

    caesar c2("a", 25);
    EXPECT_EQ(c2.encode(), "z");
}
