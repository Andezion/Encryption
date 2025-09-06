#include <gtest/gtest.h>
#include "Vertical_Transposition.h"

TEST(VerticalTranspositionTest, EncodeSimple) {
    vertical_transposition vt("HELLOWORLD", {0,1,2,3,4});
    std::string encoded = vt.encode();
    EXPECT_EQ(encoded, "HELLOWORLD");
}

TEST(VerticalTranspositionTest, EncodeWithPermutation) {
    vertical_transposition vt("HELLOWORLD", {4,3,2,1,0});
    std::string encoded = vt.encode();
    EXPECT_EQ(encoded, "OELHWDLORL");
}

TEST(VerticalTranspositionTest, DecodeBackToOriginal) {
    vertical_transposition vt("HELLOWORLD", {4,3,2,1,0});
    std::string encoded = vt.encode();
    std::string decoded = vt.decode(encoded);
    EXPECT_EQ(decoded, "HELLOWORLD");
}

TEST(VerticalTranspositionTest, IgnoreNonLetters) {
    vertical_transposition vt("H3LLO, W0RLD!", {0,1,2,3,4});
    std::string encoded = vt.encode();
    EXPECT_EQ(encoded, "HELLOWORLD");
}

TEST(VerticalTranspositionTest, DifferentIndexes) {
    vertical_transposition vt("ABCDE12345FGHIJ", {2,0,4,1,3});
    std::string encoded = vt.encode();
    std::string decoded = vt.decode(encoded);
    EXPECT_EQ(decoded, "ABCDEFGHIJ");
}

TEST(VerticalTranspositionTest, EmptyInput) {
    vertical_transposition vt("", {0,1,2,3,4});
    EXPECT_EQ(vt.encode(), "");
    EXPECT_EQ(vt.decode(""), "");
}
