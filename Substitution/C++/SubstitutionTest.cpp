#include <gtest/gtest.h>
#include "Substitution.h"

const std::string key = "bcdefghijklmnopqrstuvwxyza";

TEST(SubstitutionTest, EncodeSimpleLowercase)
{
    const substitution s("abc", key);
    EXPECT_EQ(s.encode(), "bcd");
}

TEST(SubstitutionTest, DecodeSimpleLowercase)
{
    const substitution s("abc", key);
    const std::string encoded = s.encode();
    EXPECT_EQ(s.decode(encoded), "abc");
}

TEST(SubstitutionTest, EncodeUppercase)
{
    const substitution s("XYZ", key);
    EXPECT_EQ(s.encode(), "yza");
}

TEST(SubstitutionTest, DecodeUppercase)
{
    const substitution s("XYZ", key);
    const std::string encoded = s.encode();
    EXPECT_EQ(s.decode(encoded), "xyz");
}

TEST(SubstitutionTest, EncodeWithSpacesAndSymbols)
{
    const substitution s("hello world!", key);
    EXPECT_EQ(s.encode(), "ifmmp xpsme!");
}

TEST(SubstitutionTest, DecodeWithSpacesAndSymbols)
{
    const substitution s("hello world!", key);
    const std::string encoded = s.encode();
    EXPECT_EQ(s.decode(encoded), "hello world!");
}

TEST(SubstitutionTest, EmptyString)
{
    const substitution s("", key);
    EXPECT_EQ(s.encode(), "");
    EXPECT_EQ(s.decode(""), "");
}
