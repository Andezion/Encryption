#include <gtest/gtest.h>
extern "C" {
    #include "c_rot47.h"
}

TEST(Rot47Test, EncodeDecodeRoundTrip) {
    rot47 r;
    rot47_init(&r, "Hello World!");

    char *encoded = rot47_encode(&r);
    char *decoded = rot47_decode(&r, encoded);

    EXPECT_STREQ(decoded, "Hello World!");

    free(encoded);
    free(decoded);
    free(r.input);
}

TEST(Rot47Test, EmptyString) {
    rot47 r;
    rot47_init(&r, "");

    char *encoded = rot47_encode(&r);
    char *decoded = rot47_decode(&r, encoded);

    EXPECT_STREQ(encoded, "");
    EXPECT_STREQ(decoded, "");

    free(encoded);
    free(decoded);
    free(r.input);
}

TEST(Rot47Test, SpecialCharacters) {
    rot47 r;
    rot47_init(&r, "!@#$%^&*()");

    char *encoded = rot47_encode(&r);
    char *decoded = rot47_decode(&r, encoded);

    EXPECT_STREQ(decoded, "!@#$%^&*()");

    free(encoded);
    free(decoded);
    free(r.input);
}

TEST(Rot47Test, LongString) {
    std::string longStr(500, 'A');
    rot47 r;
    rot47_init(&r, longStr.c_str());

    char *encoded = rot47_encode(&r);
    char *decoded = rot47_decode(&r, encoded);

    EXPECT_STREQ(decoded, longStr.c_str());

    free(encoded);
    free(decoded);
    free(r.input);
}

TEST(Rot47Test, EncodeChangesString) {
    rot47 r;
    rot47_init(&r, "ABC");

    char *encoded = rot47_encode(&r);

    EXPECT_STRNE(encoded, r.input);

    free(encoded);
    free(r.input);
}
