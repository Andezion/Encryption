#include <gtest/gtest.h>
#include "Morse.h"


TEST(MorseEncodeTest, SingleLetterUppercase)
{
    const morse m("A");
    EXPECT_EQ(m.encode(), "._ ");
}

TEST(MorseEncodeTest, SingleLetterLowercase)
{
    const morse m("b");
    EXPECT_EQ(m.encode(), "_... ");
}

TEST(MorseEncodeTest, WordWithSpaces)
{
    const morse m("HI");
    EXPECT_EQ(m.encode(), ".... .. ");
}

TEST(MorseEncodeTest, SentenceWithSpace)
{
    const morse m("HELLO WORLD");
    EXPECT_EQ(m.encode(),
              ".... . ._.. ._.. ___ / .__ ___ ._. ._.. _.. ");
}

TEST(MorseEncodeTest, DigitsAndSymbols)
{
    const morse m("SOS 123!");
    EXPECT_EQ(m.encode(),
              "... ___ ... / .____ ..___ ...__ _._.__ ");
}

TEST(MorseDecodeTest, SingleLetter)
{
    const morse m("");
    EXPECT_EQ(m.decode("._ "), "A");
}

TEST(MorseDecodeTest, Word)
{
    const morse m("");
    EXPECT_EQ(m.decode(".... . ._.. ._.. ___ "), "HELLO");
}

TEST(MorseDecodeTest, WithSpaceSeparator)
{
    const morse m("");
    EXPECT_EQ(m.decode(".... . ._.. ._.. ___ / .__ ___ ._. ._.. _.. "),
              "HELLO WORLD");
}

TEST(MorseDecodeTest, DigitsAndSymbols)
{
    const morse m("");
    EXPECT_EQ(m.decode("... ___ ... / .____ ..___ ...__ _._.__ "),
              "SOS 123!");
}

TEST(MorseRoundTripTest, EncodeDecodeSymmetry)
{
    const std::string text = "C IS COOL!";
    const morse m(text);
    const std::string encoded = m.encode();

    const morse m2("");
    const std::string decoded = m2.decode(encoded);

    EXPECT_EQ(decoded, "C IS COOL!");
}

TEST(MorseRoundTripTest, OnlySupportedSymbols)
{
    const std::string text = "HELLO 123";
    const morse m(text);
    const std::string encoded = m.encode();

    const morse m2("");
    const std::string decoded = m2.decode(encoded);

    EXPECT_EQ(decoded, "HELLO 123");
}
