#include <gtest/gtest.h>
extern "C" {
#include "Atbash.c"
}

TEST(AtbashTest, EncodeLowercase) {
    atbash c{};
    c.input = strdup("abc");
    c.len = strlen(c.input);

    char *res = c.encoded();
    EXPECT_STREQ(res, "zyx");
    free(res);
    c.destroy();
}

TEST(AtbashTest, EncodeUppercase) {
    atbash c{};
    c.input = strdup("ABC");
    c.len = strlen(c.input);

    char *res = c.encoded();
    EXPECT_STREQ(res, "ZYX");
    free(res);
    c.destroy();
}

TEST(AtbashTest, EncodeMixed) {
    atbash c{};
    c.input = strdup("AbC xYz!");
    c.len = strlen(c.input);

    char *res = c.encoded();
    EXPECT_STREQ(res, "ZyX cBa!");
    free(res);
    c.destroy();
}

TEST(AtbashTest, DecodeBack) {
    atbash c{};
    c.input = strdup("Hello");
    c.len = strlen(c.input);

    char *enc = c.encoded();
    char *dec = c.decode(enc);

    EXPECT_STREQ(dec, "Hello");

    free(enc);
    free(dec);
    c.destroy();
}

TEST(AtbashTest, HandlesDigitsAndSpaces) {
    atbash c{};
    c.input = strdup("abc 123");
    c.len = strlen(c.input);

    char *res = c.encoded();
    EXPECT_STREQ(res, "zyx 123");
    free(res);
    c.destroy();
}

TEST(AtbashTest, InvalidCharacter) {
    atbash c{};
    c.input = strdup("abc😀");  // emoji
    c.len = strlen(c.input);

    char *res = c.encoded();
    EXPECT_EQ(res, nullptr);
    c.destroy();
}
