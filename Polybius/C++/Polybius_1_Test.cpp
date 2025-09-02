#include "Polybius_method1.h"
#include <gtest/gtest.h>

TEST(PolybiusTest, EncodeSingleLetter) 
{
    polybius_method1 cipher("A");
    EXPECT_EQ(cipher.encode(), "F");  
}

TEST(PolybiusTest, EncodeSimpleWord) 
{
    polybius_method1 cipher("HELLO");
    EXPECT_EQ(cipher.encode(), "MFQQT");  
}

TEST(PolybiusTest, DecodeSingleLetter) 
{
    polybius_method1 cipher("F");
    EXPECT_EQ(cipher.decode("F"), "A");  
}

TEST(PolybiusTest, EncodeDecodeReversibility) 
{
    std::string text = "SECRET";
    polybius_method1 cipher(text);
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded, "SECRET");
}

TEST(PolybiusTest, HandlesLetterJ) 
{
    polybius_method1 cipher("JUMP");
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded.find('J'), std::string::npos); 
}

TEST(PolybiusTest, NonAlphabetCharacters) 
{
    polybius_method1 cipher("HI!");
    std::string encoded = cipher.encode();
    std::string decoded = cipher.decode(encoded);
    EXPECT_EQ(decoded, "HI"); 
}
