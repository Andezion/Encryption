#include <gtest/gtest.h>
#include "playfair.h"

TEST(PlayfairTest, MatrixConstruction)
{
    const playfair pf("KEYWORD", "");
    auto [row, col] = pf.find_position('K');
    EXPECT_NE(row, -1);
    EXPECT_NE(col, -1);

    auto [rowJ, colJ] = pf.find_position('J');
    EXPECT_EQ(rowJ, -1);
    EXPECT_EQ(colJ, -1);

    auto [rowI, colI] = pf.find_position('I');
    EXPECT_NE(rowI, -1);
    EXPECT_NE(colI, -1);
}

TEST(PlayfairTest, EncodeBasic)
{
    const playfair pf("PLAYFAIR EXAMPLE", "HIDETHEGOLDINTHETREESTUMP");
    const std::string encoded = pf.encode();

    EXPECT_EQ(encoded, "BMODZBXDNABEKUDMUIXMMOUVIF");
}

TEST(PlayfairTest, EncodeHandlesRepeatedLetters)
{
    const playfair pf("KEY", "BALLOON");
    const std::string encoded = pf.encode();

    EXPECT_FALSE(encoded.empty());
    EXPECT_EQ(encoded.size() % 2, 0);
}

TEST(PlayfairTest, EncodeAddsPaddingX)
{
    const playfair pf("KEY", "HELLOX");
    const std::string encoded = pf.encode();
    EXPECT_EQ(encoded.size() % 2, 0);
}

TEST(PlayfairTest, DecodeBasic)
{
    const playfair pf("PLAYFAIR EXAMPLE", "");
    const std::string decoded = pf.decode("BMODZBXDNABEKUDMUIXMMOUVIF");

    EXPECT_EQ(decoded, "HIDETHEGOLDINTHETREXESTUMP");
}

TEST(PlayfairTest, EncodeDecodeSymmetry)
{
    const std::string msg = "SECRET MESSAGE";
    const playfair pf("SECRETKEY", msg);
    const std::string encoded = pf.encode();

    const playfair pf2("SECRETKEY", "");
    const std::string decoded = pf2.decode(encoded);

    EXPECT_TRUE(decoded.find("SECRET") != std::string::npos);
}
