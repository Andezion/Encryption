#include <gtest/gtest.h>
#include "Polybius_method2.h"

TEST(PolybiusMethod2Test, EncodeSimpleWord) {
    polybius_method2 cipher("HELLO");
    EXPECT_EQ(cipher.encode(), "23 15 31 31 34 ");
}

TEST(PolybiusMethod2Test, DecodeSimpleWord) {
    polybius_method2 cipher("");
    EXPECT_EQ(cipher.decode("23 15 31 31 34 "), "HELLO");
}

TEST(PolybiusMethod2Test, EncodeJtoI) {
    polybius_method2 cipher("JUMP");
    EXPECT_EQ(cipher.encode(), cipher.decode(cipher.encode()));
    EXPECT_EQ(cipher.decode(cipher.encode()), "IUMP"); 
}

TEST(PolybiusMethod2Test, IgnoreNonAlphabet) {
    polybius_method2 cipher("HI!123");
    EXPECT_EQ(cipher.encode(), "23 24 ");
}

TEST(PolybiusMethod2Test, CaseInsensitive) {
    polybius_method2 cipher("HeLLo");
    EXPECT_EQ(cipher.encode(), "23 15 31 31 34 ");
}

TEST(PolybiusMethod2Test, EncodeDecodeRoundtrip) {
    std::string text = "POLYBIUS";
    polybius_method2 cipher(text);
    auto encoded = cipher.encode();
    auto decoded = cipher.decode(encoded);

    EXPECT_EQ(decoded, "POLYBIUS");
}
