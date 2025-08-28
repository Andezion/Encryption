#include "gtest/gtest.h"
#include "Atbash.h"

TEST(AtbashTest, EncodeLowercase)
{
    const atbash cipher("abcxyz");
    EXPECT_EQ(cipher.encode(), "zyxcba");
}

TEST(AtbashTest, EncodeUppercase)
{
    const atbash cipher("ABCXYZ");
    EXPECT_EQ(cipher.encode(), "ZYXCBA");
}

TEST(AtbashTest, EncodeMixedCase)
{
    const atbash cipher("AbC");
    EXPECT_EQ(cipher.encode(), "ZyX");
}

TEST(AtbashTest, EncodeWithSpacesDigitsPunct)
{
    const atbash cipher("Hello, World! 123");
    EXPECT_EQ(cipher.encode(), "Svool, Dliow! 123");
}

TEST(AtbashTest, DecodeIsInverseOfEncode)
{
    const atbash cipher("SecretMessage");
    const std::string encoded = cipher.encode();
    EXPECT_EQ(cipher.decode(encoded), "SecretMessage");
}

TEST(AtbashTest, InvalidCharacterThrows)
{
    const atbash cipher("Привет"); 
    EXPECT_THROW(cipher.encode(), std::invalid_argument);
}
