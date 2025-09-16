#include <gtest/gtest.h>
#include "affine.h"
#include <string>
#include <cstring>
#include <cstdlib>

static std::string check_and_free(char *actual)
{
    std::string result(actual);
    free(actual);
    return result;
}

TEST(AffineTest, BasicEncodingDecoding) {
    Affine aff = affine_create((char*)"hello", 5, 8);

    char *encoded = affine_encode(&aff);
    EXPECT_EQ(check_and_free(encoded), "raxxy");

    char *decoded = affine_decode(&aff, "raxxy");
    EXPECT_EQ(check_and_free(decoded), "hello");
}

TEST(AffineTest, UppercaseEncodingDecoding) {
    Affine aff = affine_create((char*)"HELLO", 5, 8);

    char *encoded = affine_encode(&aff);
    EXPECT_EQ(check_and_free(encoded), "RAXXY");

    char *decoded = affine_decode(&aff, "RAXXY");
    EXPECT_EQ(check_and_free(decoded), "HELLO");
}

TEST(AffineTest, MixedTextWithSpacesDigitsPunct) {
    Affine aff = affine_create((char*)"Hi World 123!", 7, 3);

    char *encoded = affine_encode(&aff);
    EXPECT_EQ(check_and_free(encoded), "Ot Dzscw 123!");

    char *decoded = affine_decode(&aff, "Ot Dzscw 123!");
    EXPECT_EQ(check_and_free(decoded), "Hi World 123!");
}

TEST(AffineTest, InvalidAParameter) {
    EXPECT_EXIT({
        affine_create((char*)"test", 13, 5);
    }, ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(AffineTest, InvalidCharactersInInput) {
    Affine aff = affine_create((char*)"test", 5, 8);

    EXPECT_EXIT({
        char bad_input[] = "teßt"; 
        aff.input = bad_input;
        char *out = affine_encode(&aff);
        free(out);
    }, ::testing::ExitedWithCode(EXIT_FAILURE), ".*");
}


