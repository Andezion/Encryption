#include <gtest/gtest.h>
extern "C" {
    #include "Transposition.h"  
}

static Transposition make_transposition(const char* text) 
{
    Transposition t{};
    t.len = strlen(text);
    t.input = (char*)malloc(t.len + 1);
    strcpy(t.input, text);
    return t;
}

TEST(TranspositionTest, EncodeDecodeBasic) 
{
    Transposition t = make_transposition("HELLOWORLD");
    char* encoded = encode(&t);
    char* decoded = decode(encoded);

    EXPECT_EQ(std::string(decoded), "HELLOWORLD");

    free(t.input);
    free(encoded);
    free(decoded);
}

TEST(TranspositionTest, EncodeLowercaseInput) 
{
    Transposition t = make_transposition("helloworld");
    char* encoded = encode(&t);
    char* decoded = decode(encoded);

    EXPECT_EQ(std::string(decoded), "HELLOWORLD");

    free(t.input);
    free(encoded);
    free(decoded);
}

TEST(TranspositionTest, EmptyString) 
{
    Transposition t = make_transposition("");
    char* encoded = encode(&t);
    char* decoded = decode(encoded);

    EXPECT_EQ(std::string(encoded), "");
    EXPECT_EQ(std::string(decoded), "");

    free(t.input);
    free(encoded);
    free(decoded);
}

TEST(TranspositionTest, ShortString) 
{
    Transposition t = make_transposition("abc");
    char* encoded = encode(&t);
    char* decoded = decode(encoded);

    EXPECT_EQ(std::string(encoded), "");
    EXPECT_EQ(std::string(decoded), "");

    free(t.input);
    free(encoded);
    free(decoded);
}

TEST(TranspositionTest, IgnoreNonAlphabetic) 
{
    Transposition t = make_transposition("he!!o123world");
    char* encoded = encode(&t);
    char* decoded = decode(encoded);

    EXPECT_EQ(std::string(decoded), "HELLOWORLD");

    free(t.input);
    free(encoded);
    free(decoded);
}

TEST(TranspositionTest, EncodedLengthMultipleOf5) 
{
    Transposition t = make_transposition("HELLOWORLDTEST");
    char* encoded = encode(&t);

    EXPECT_EQ(strlen(encoded) % 5, 0u);

    free(t.input);
    free(encoded);
}

TEST(TranspositionTest, RoundTripRandom) 
{
    std::string input = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Transposition t = make_transposition(input.c_str());
    char* encoded = encode(&t);
    char* decoded = decode(encoded);

    EXPECT_EQ(std::string(decoded), input);

    free(t.input);
    free(encoded);
    free(decoded);
}
