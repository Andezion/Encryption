#include "LZ77.h"
#include <gtest/gtest.h>
#include <cstring>
#include <fstream>

void testCompressionCycle(const char* input, size_t inLen)
{
    LZ77 lz;

    std::vector<LZ77::U8> compressed(inLen * 2 + 1000);
    std::vector<LZ77::U8> decompressed(inLen + 1000);

    const size_t compLen = lz.compressBuffer(
        reinterpret_cast<const LZ77::U8*>(input), 
        inLen, 
        compressed.data()
    );
    
    ASSERT_GT(compLen, 0u);

    const size_t decLen = LZ77::decompressBuffer(
        compressed.data(), 
        compLen, 
        decompressed.data(), 
        decompressed.size()
    );
    
    ASSERT_EQ(decLen, inLen);
    ASSERT_EQ(memcmp(input, decompressed.data(), inLen), 0);
}

TEST(LZ77Test, EncodeDecodeSimple)
{
    const auto input = "hello world";
    testCompressionCycle(input, strlen(input));
}

TEST(LZ77Test, EncodeDecodeWithRepeats)
{
    const auto input = "hello hello world world world";
    const size_t inLen = strlen(input);
    
    LZ77 lz;
    std::vector<LZ77::U8> compressed(inLen * 2);

    const size_t compLen = lz.compressBuffer(
        reinterpret_cast<const LZ77::U8*>(input), 
        inLen, 
        compressed.data()
    );
    
    EXPECT_LT(compLen, inLen);
    testCompressionCycle(input, inLen);
}

TEST(LZ77Test, EncodeDecodeShortString)
{
    const auto input = "abc";
    testCompressionCycle(input, strlen(input));
}

TEST(LZ77Test, EncodeDecodeEmptyString)
{
    const auto input = "";
    testCompressionCycle(input, 0);
}

TEST(LZ77Test, EncodeDecodeSingleChar)
{
    const auto input = "A";
    testCompressionCycle(input, 1);
}

TEST(LZ77Test, EncodeDecodeHighCompression)
{
    const std::vector<LZ77::U8> input(10000, 'A');
    
    LZ77 lz;
    std::vector<LZ77::U8> compressed(input.size() * 2);
    std::vector<LZ77::U8> decompressed(input.size() + 1000);

    const size_t compLen = lz.compressBuffer(
        input.data(), 
        input.size(), 
        compressed.data()
    );
    
    EXPECT_LT(compLen, input.size() / 10);

    const size_t decLen = LZ77::decompressBuffer(
        compressed.data(), 
        compLen, 
        decompressed.data(), 
        decompressed.size()
    );
    
    ASSERT_EQ(decLen, input.size());
    ASSERT_EQ(memcmp(input.data(), decompressed.data(), input.size()), 0);
}

TEST(LZ77Test, EncodeDecodePattern)
{
    const char* input = "abcabcabcabcabc";
    testCompressionCycle(input, strlen(input));
}

TEST(LZ77Test, EncodeDecodeLongText)
{
    const char* input = "The quick brown fox jumps over the lazy dog. "
                       "The quick brown fox jumps over the lazy dog. "
                       "The quick brown fox jumps over the lazy dog.";
    testCompressionCycle(input, strlen(input));
}

TEST(LZ77Test, CompressDecompressFile)
{
    LZ77 lz;
    const char* testData = "The quick brown fox jumps over the lazy dog. "
                          "The quick brown fox jumps over the lazy dog.";

    std::ofstream out("test_input.txt", std::ios::binary);
    out.write(testData, strlen(testData));
    out.close();

    ASSERT_NO_THROW(lz.compressFile("test_input.txt", "test_compressed.ulz"));

    ASSERT_NO_THROW(LZ77::decompressFile("test_compressed.ulz", "test_output.txt"));

    std::ifstream in("test_output.txt", std::ios::binary);
    std::string result((std::istreambuf_iterator<char>(in)), 
                       std::istreambuf_iterator<char>());
    in.close();
    
    ASSERT_EQ(result, testData);

    std::remove("test_input.txt");
    std::remove("test_compressed.ulz");
    std::remove("test_output.txt");
}

TEST(LZ77Test, DecompressInvalidMagic)
{
    std::ofstream out("test_invalid.ulz", std::ios::binary);
    LZ77::U32 badMagic = 0x12345678;
    out.write(reinterpret_cast<char*>(&badMagic), sizeof(badMagic));
    out.close();
    
    ASSERT_THROW(LZ77::decompressFile("test_invalid.ulz", "test_out.txt"), 
                 std::runtime_error);
    
    std::remove("test_invalid.ulz");
}
