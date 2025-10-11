#include <gtest/gtest.h>
#include "SHA512.h"
#include <fstream>

TEST(SHA512, EmptyString) {
    EXPECT_EQ(sha512(""), 
        "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
        "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
}

TEST(SHA512, ABC) {
    EXPECT_EQ(sha512("abc"), 
        "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a"
        "2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");
}

TEST(SHA512, QuickFox) {
    EXPECT_EQ(sha512("The quick brown fox jumps over the lazy dog"), 
        "07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb64"
        "2e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6");
}

TEST(SHA512, QuickFoxDot) {
    EXPECT_EQ(sha512("The quick brown fox jumps over the lazy dog."), 
        "91ea1245f20d46ae9a037a989f54f1f790f0a47607eeb8a14d12890cea77a1bb"
        "c6c7ed9cf205e67b7f2b8fd4c7dfd3a7a8617e45f3c463d481c7e586c39ac1ed");
}

TEST(SHA512, FileHash) {
    std::ofstream("tmp.txt") << "data for test";
    EXPECT_EQ(sha512file("tmp.txt"), sha512("data for test"));
    std::remove("tmp.txt");
}

TEST(SHA512, MillionA) {
    const std::string long_input(1000000, 'a');
    EXPECT_EQ(sha512(long_input),
        "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973eb"
        "de0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b");
}
