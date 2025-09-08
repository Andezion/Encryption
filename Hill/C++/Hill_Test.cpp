#include <gtest/gtest.h>
#include "Hill.h"

TEST(HillTest, EncodeDecode2x2) 
{
    hill h(2, "TEST", "GYBNQKURP"); 
    std::string encoded = h.encode();
    std::string decoded = h.decode(encoded);

    EXPECT_EQ(decoded, "TEST");
}

TEST(HillTest, PaddingAdded) 
{
    hill h(2, "HELLO", "GYBNQKURP");
    std::string encoded = h.encode();
    EXPECT_EQ(encoded.size() % 2, 0);
}

TEST(HillTest, InvalidKeyLengthThrows) 
{
    EXPECT_THROW(hill(2, "TEST", "ABC"), std::invalid_argument);
}

TEST(HillTest, InvalidCharacterInKeyThrows)
{
    EXPECT_THROW(hill(2, "TEST", "AB1D"), std::invalid_argument);
}

TEST(HillTest, DecodeInvalidSizeThrows) 
{
    hill h(2, "HI", "GYBNQKURP");
    std::string encoded = h.encode();
    encoded.pop_back();
    EXPECT_THROW(h.decode(encoded), std::invalid_argument);
}

TEST(HillTest, Mod26Works) 
{
    EXPECT_EQ(hill::mod26(27), 1);
    EXPECT_EQ(hill::mod26(-1), 25);
    EXPECT_EQ(hill::mod26(52), 0);
}

TEST(HillTest, ModInverseWorks) 
{
    EXPECT_EQ(hill::mod_inverse(3, 26), 9); 
    EXPECT_THROW(hill::mod_inverse(2, 26), std::runtime_error); 
}
