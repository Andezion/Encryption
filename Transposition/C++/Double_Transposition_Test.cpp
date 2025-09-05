#include <gtest/gtest.h>
#include "double_transposition.h"

TEST(DoubleTranspositionTest, BasicEncodeDecode) {
    std::string input = "HELLOWORLD";
    std::vector<size_t> row_perm = {1, 0};   
    std::vector<size_t> col_perm = {1, 0, 2, 3, 4}; 

    double_transposition cipher(input, row_perm, col_perm);
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);

    EXPECT_EQ(decoded.substr(0, input.size()), "helloworld");
}

TEST(DoubleTranspositionTest, EncodeAddsPadding) {
    std::string input = "TEST";
    std::vector<size_t> row_perm = {0, 1};
    std::vector<size_t> col_perm = {0, 1, 2};

    double_transposition cipher(input, row_perm, col_perm);
    std::string encoded = cipher.encode();

    EXPECT_EQ(encoded.size() % col_perm.size(), 0);
    EXPECT_TRUE(encoded.find('X') != std::string::npos);
}

TEST(DoubleTranspositionTest, EncodeSkipsNonAlphabetic) {
    std::string input = "ABC 123 !!! DEF";
    std::vector<size_t> row_perm = {0, 1};
    std::vector<size_t> col_perm = {0, 1, 2};

    double_transposition cipher(input, row_perm, col_perm);
    std::string encoded = cipher.encode();

    for (char c : encoded) {
        EXPECT_TRUE(isupper(c));
    }
}

TEST(DoubleTranspositionTest, RoundtripWithCustomPermutations) {
    std::string input = "CRYPTOGRAPHY";
    std::vector<size_t> row_perm = {2, 0, 1};        
    std::vector<size_t> col_perm = {2, 0, 1, 3};      

    double_transposition cipher(input, row_perm, col_perm);
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);

    EXPECT_EQ(decoded.substr(0, input.size()), "cryptography");
}
