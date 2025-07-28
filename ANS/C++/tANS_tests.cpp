#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

#include "tANS.h"
#include "tANS_tests.h"

std::string read_text_file(const std::string& filepath)
{
    const std::ifstream file(filepath);
    if (!file)
    {
        std::cerr << "Error: Cannot open file: " << filepath << std::endl;
        return {};
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

template <typename ExceptionType, typename Func>
bool expect_exception(Func func, const std::string& test_name)
{
    try
    {
        func();
        std::cout << "Test " << test_name << ": FAILED (Expected exception, but none was thrown)." << std::endl;
        return false;
    }
    catch (const ExceptionType& e)
    {
        std::cout << "Test " << test_name << ": PASSED (Caught expected exception: " << e.what() << ")" << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cout << "Test " << test_name << ": FAILED (Caught unexpected exception: " << e.what() << ")" << std::endl;
        return false;
    }
    catch (...)
    {
        std::cout << "Test " << test_name << ": FAILED (Caught unknown exception)." << std::endl;
        return false;
    }
}

void run_correctness_test(const std::string& test_name,
                          const std::string& labeling,
                          const std::string& original_message,
                          const size_t initial_state_val) {
    try
    {
        const auto start = std::chrono::high_resolution_clock::now();
        const t_ans tans(labeling);
        const t_ans::EncodeResult encode_result = tans.encode(original_message, initial_state_val);
        const t_ans::DecodeResult decode_result = tans.decode(encode_result.state, encode_result.bitstream);
        const auto end = std::chrono::high_resolution_clock::now();
        const std::chrono::duration<double> elapsed_seconds = end - start;

        if (original_message == decode_result.message)
        {
            std::cout << "Test " << test_name << ": correct! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
        }
        else
        {
            std::cout << "Test " << test_name << ": FAILED (Decoded message mismatch). Original: '" << original_message
                      << "', Decoded: '" << decode_result.message << "' Took: " << elapsed_seconds.count() << " seconds." << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cout << "Test " << test_name << ": FAILED (Caught unexpected exception during correctness test: " << e.what() << ")" << std::endl;
    }
    catch (...)
    {
        std::cout << "Test " << test_name << ": FAILED (Caught unknown exception during correctness test)." << std::endl;
    }
}

void test_constructor_errors()
{
    std::cout << "\n<--- Constructor Error Tests --->" << std::endl;

    expect_exception<std::invalid_argument>([]
    {
        t_ans tans("");
    }, "1.1 (Constructor) Empty labeling string");

    expect_exception<std::invalid_argument>([]
    {
        const std::string large_labeling(1048576 + 1, 'a');
        t_ans tans(large_labeling);
    }, "1.2 (Constructor) Very large labeling string");

    try
    {
        t_ans tans("aaaaa");
        std::cout << "Test 1.3 (Constructor) Single char labeling: PASSED" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "Test 1.3 (Constructor) Single char labeling: FAILED (Caught unexpected exception: " << e.what() << ")" << std::endl;
    }
}

void test_encode_errors()
{
    std::cout << "\n<--- Tests for encode --->" << std::endl;

    const t_ans tans("abcdefg");

    expect_exception<std::invalid_argument>([&]
    {
        tans.encode("");
    }, "2.1 (Encode) Empty message");

    expect_exception<std::invalid_argument>([&]
    {
        tans.encode("xyz");
    }, "2.2 (Encode) Message with unknown symbol");

    expect_exception<std::invalid_argument>([&]
    {
        tans.encode("abcx");
    }, "2.3 (Encode) Message with unknown last symbol");

    expect_exception<std::invalid_argument>([&]
    {
        tans.encode("abc", 1);
    }, "2.4 (Encode) initial_state out of bounds");

}

void test_decode_errors()
{
    std::cout << "\n--- Tests for decode ---" << std::endl;

    const t_ans tans("abc");
    const std::string original = "abc";
    const t_ans::EncodeResult encoded_result = tans.encode(original, 0);

    expect_exception<std::runtime_error>([&]
    {
        tans.decode(encoded_result.state, encoded_result.bitstream + "2");
    }, "3.1 (Decode) Invalid char in bitstream");

    expect_exception<std::runtime_error>([&]
    {
        tans.decode(0, "101010");
    }, "3.2 (Decode) State underflow");

    expect_exception<std::out_of_range>([&]
    {
        tans.decode(tans.get_block_size() * 2, "");
    }, "3.3 (Decode) Decoding table index out of bounds (high state)");


    expect_exception<std::runtime_error>([&]
    {
        std::string long_bitstream(1000000, '0');
        tans.decode(encoded_result.state, long_bitstream);
    }, "3.4 (Decode) Max iterations reached");
}

void test_edge_cases()
{
    std::cout << "\n<--- Tests of uncomfortable data --->" << std::endl;
    run_correctness_test("4.1 Single char message", "abc", "a");
    run_correctness_test("4.2 Repeated single char message", "abc", "abc");
    run_correctness_test("4.3 Single unique char labeling", "aaaaa", "a");
    run_correctness_test("4.4 Non-zero initial_state", "aaabbbccc", "aaa", 1);
    run_correctness_test("4.5 Non-zero initial_state (another)", "aaabbbccc", "ccc", 2);
    run_correctness_test("4.6 Last symbol is only in labeling", "a", "a");
}

void test_performance_and_correctness()
{
    std::cout << "\n<--- Tests with real data --->" << std::endl;

    std::string full_byte_alphabet(256, '\0');
    for (int i = 0; i < 256; ++i)
    {
        full_byte_alphabet[i] = static_cast<char>(i);
    }
    run_correctness_test("5.1 Full byte alphabet (small)", full_byte_alphabet, "Hello, world!");

    const std::string medium_message = "This is a longer test message to check performance and correctness with a full byte alphabet. "
                                 "It contains various characters, numbers, and symbols: !@#$%^&*()_+{}[]\\|;:'\"<>,.?/~` "
                                 "and also some repeated patterns like aaaaaaaaaaaaaaaaaaaaa.";
    run_correctness_test("5.2 Full byte alphabet (medium)", full_byte_alphabet, medium_message);

    if (const std::string example_file_content = read_text_file("example.txt"); example_file_content.empty())
    {
        std::cout << "Test 5.3: example.txt is empty or not found. Skipping." << std::endl;
    }
    else
    {
        std::cout << "Encoding message of size: " << example_file_content.size() << std::endl;
        run_correctness_test("5.3 Large file content (example.txt)", full_byte_alphabet, example_file_content);
    }

    if (const std::string example_file_content = read_text_file("example1.txt"); example_file_content.empty())
    {
        std::cout << "Test 5.4: example1.txt is empty or not found. Skipping." << std::endl;
    }
    else
    {
        std::cout << "Encoding message of size: " << example_file_content.size() << std::endl;
        run_correctness_test("5.4 Large file content (example1.txt)", full_byte_alphabet, example_file_content);
    }

    const std::string low_entropy_message(10000, 'X');
    run_correctness_test("5.5 Low entropy message", full_byte_alphabet, low_entropy_message);

    std::string high_entropy_message;
    high_entropy_message.reserve(10000);
    for (size_t i = 0; i < 10000; ++i)
    {
        high_entropy_message += static_cast<char>(rand() % 256);
    }
    run_correctness_test("5.6 High entropy message", full_byte_alphabet, high_entropy_message);
}


void tANS_test()
{
    test_constructor_errors();
    test_encode_errors();
    test_decode_errors();
    test_edge_cases();
    test_performance_and_correctness();

    std::cout << "\nAll tests finished." << std::endl;
}
