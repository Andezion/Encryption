#include "gtest/gtest.h"
#include "Transposition.h"

TEST(TranspositionTest, EncodeDecodeSimple)
{
    const transposition t("HELLO WORLD");
    const std::string encoded = t.encode();
    const std::string decoded = transposition::decode(encoded);

    EXPECT_EQ(encoded, "ODLLLREOHW"); 
    EXPECT_EQ(decoded, "HELLOWORLD");
}

TEST(TranspositionTest, ExactFiveLetters)
{
    const transposition t("ABCDE");
    const std::string encoded = t.encode();
    const std::string decoded = transposition::decode(encoded);

    EXPECT_EQ(encoded, "EDCBA");
    EXPECT_EQ(decoded, "ABCDE");
}

TEST(TranspositionTest, IgnoreNonAlpha)
{
    const transposition t("A!B@C#D$E%");
    const std::string encoded = t.encode();
    const std::string decoded = transposition::decode(encoded);

    EXPECT_EQ(encoded, "EDCBA");
    EXPECT_EQ(decoded, "ABCDE");
}

TEST(TranspositionTest, TwoRowsEncoding)
{
    const transposition t("ABCDEFGHIJ");
    const std::string encoded = t.encode();
    const std::string decoded = transposition::decode(encoded);

    EXPECT_EQ(encoded, "EJDICHBGAF");
    EXPECT_EQ(decoded, "ABCDEFGHIJ");
}

TEST(TranspositionTest, EmptyInput)
{
    const transposition t("");
    const std::string encoded = t.encode();
    EXPECT_EQ(encoded, "");

    const std::string decoded = transposition::decode(encoded);
    EXPECT_EQ(decoded, "");
}

TEST(TranspositionTest, LessThanFiveLetters)
{
    const transposition t("AB");
    const std::string encoded = t.encode();
    EXPECT_EQ(encoded, "");

    const std::string decoded = transposition::decode(encoded);
    EXPECT_EQ(decoded, "");
}

TEST(TranspositionTest, MixedCaseInput)
{
    const transposition t("AbCdEfGhIj");
    const std::string encoded = t.encode();
    const std::string decoded = transposition::decode(encoded);

    EXPECT_EQ(encoded, "EJDICHBGAF");
    EXPECT_EQ(decoded, "ABCDEFGHIJ");
}
