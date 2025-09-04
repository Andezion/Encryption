#include <gtest/gtest.h>
#include "Polybius_method3.h"

TEST(PolybiusMethod3Test, EncodeSimpleWord) {
    polybius_method3 cipher("HELLO");
    std::string encoded = cipher.encode();
    EXPECT_FALSE(encoded.empty());
    EXPECT_NE(encoded, "HELLO"); 
}

TEST(PolybiusMethod3Test, DecodeSimpleWord) {
    polybius_method3 cipher("HELLO");
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded, "HELLO");  
}

TEST(PolybiusMethod3Test, EncodeWithJ) {
    polybius_method3 cipher("JULIA");
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded, "IULIA");
}

TEST(PolybiusMethod3Test, IgnoreNonAlphabetic) {
    polybius_method3 cipher("HELLO 123 !!!");
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded, "HELLO");  
}

TEST(PolybiusMethod3Test, EmptyInput) {
    polybius_method3 cipher("");
    std::string encoded = cipher.encode();
    EXPECT_TRUE(encoded.empty());
    std::string decoded = cipher.decode(encoded);
    EXPECT_TRUE(decoded.empty());
}

TEST(PolybiusMethod3Test, EncodeDecodeSymmetry) {
    std::string text = "CRYPTOGRAPHY";
    polybius_method3 cipher(text);
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded, text);
}
