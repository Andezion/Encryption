#include <gtest/gtest.h>
#include "affine.h"

TEST(AffineTest, GcdCalculation)
{
    EXPECT_EQ(affine::gcd(26, 13), 13);
    EXPECT_EQ(affine::gcd(35, 10), 5);
    EXPECT_EQ(affine::gcd(7, 3), 1);
}

TEST(AffineTest, InvalidAThrows)
{
    EXPECT_THROW(affine("hello", 13, 5), std::invalid_argument);
    EXPECT_THROW(affine("world", 2, 7), std::invalid_argument);
}

TEST(AffineTest, EncodeBasic)
{
    const affine cipher("hello", 5, 8);
    EXPECT_EQ(cipher.encode(), "rclla");
}

TEST(AffineTest, DecodeBasic)
{
    const affine cipher("hello", 5, 8);
    const std::string encoded = cipher.encode();
    const std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded, "hello");
}

TEST(AffineTest, EncodeUppercase)
{
    const affine cipher("HELLO", 5, 8);
    EXPECT_EQ(cipher.encode(), "RCLLA");
}

TEST(AffineTest, DecodeUppercase)
{
    const affine cipher("HELLO", 5, 8);
    const std::string encoded = cipher.encode();
    EXPECT_EQ(cipher.decode(encoded), "HELLO");
}

TEST(AffineTest, ModInverseCorrect)
{
    EXPECT_EQ(affine::mod_inverse(5, 26), 21);
    EXPECT_THROW(affine::mod_inverse(2, 26), std::invalid_argument);
}


