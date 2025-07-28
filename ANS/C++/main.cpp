#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "tANS.h"
#include "Caesar.h"
#include "ROT13.h"
#include "ROT47.h"
#include "uANS.h"
#include "Affine.h"
#include "Atbash.h"
#include "Vigenere.h"

#include "tANS_tests.h"


void test1()
{
    const t_ans tans("abcabcbacbacbab");

    const std::string original_message = "abc";
    // std::cout << "Sentence before: " << original_message << std::endl;

    const auto start = std::chrono::high_resolution_clock::now();
    //// Кодирование
    const auto encode_result = tans.encode(original_message, 0);
    //std::cout << "State: " << encode_result.state << std::endl;
    //std::cout << "Bitstream: " << encode_result.bitstream << std::endl;

    //// Декодирование
    const auto decode_result = tans.decode(encode_result.state, encode_result.bitstream);
    //std::cout << "Decoded sentence: " << decode_result.message << std::endl;

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;

    //// Проверка корректности
    if (original_message == decode_result.message)
    {
        std::cout << "Test 1: correct! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
    else
    {
        std::cout << "Test 1: failed! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
}
void test2()
{
    const t_ans tans("aaabbbcccaaabbbccc");

    const std::string original_message = "abcabc";
    //std::cout << "Sentence before: " << original_message << std::endl;

    const auto start = std::chrono::high_resolution_clock::now();;
    //// Кодирование
    const auto encode_result = tans.encode(original_message, 0);
    //std::cout << "State: " << encode_result.state << std::endl;
    //std::cout << "Bitstream: " << encode_result.bitstream << std::endl;

    //// Декодирование
    const auto decode_result = tans.decode(encode_result.state, encode_result.bitstream);
    //std::cout << "Decoded sentence: " << decode_result.message << std::endl;

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;

    //// Проверка корректности
    if (original_message == decode_result.message)
    {
        std::cout << "Test 2: correct! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
    else
    {
        std::cout << "Test 2: failed! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
}
void test3()
{
    const t_ans tans("abc");

    const std::string original_message = "abc";
    //std::cout << "Sentence before: " << original_message << std::endl;

    const auto start = std::chrono::high_resolution_clock::now();;
    //// Кодирование
    const auto encode_result = tans.encode(original_message, 0);
    //std::cout << "State: " << encode_result.state << std::endl;
    //std::cout << "Bitstream: " << encode_result.bitstream << std::endl;

    //// Декодирование
    const auto decode_result = tans.decode(encode_result.state, encode_result.bitstream);
    //std::cout << "Decoded sentence: " << decode_result.message << std::endl;

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;

    //// Проверка корректности
    if (original_message == decode_result.message)
    {
        std::cout << "Test 3: correct! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
    else
    {
        std::cout << "Test 3: failed! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
}
void test4()
{
    const t_ans tans("abcdefg");

    const std::string original_message = "abc";
    //std::cout << "Sentence before: " << original_message << std::endl;

    const auto start = std::chrono::high_resolution_clock::now();;
    //// Кодирование
    const auto encode_result = tans.encode(original_message, 0);
    //std::cout << "State: " << encode_result.state << std::endl;
    //std::cout << "Bitstream: " << encode_result.bitstream << std::endl;

    //// Декодирование
    const auto decode_result = tans.decode(encode_result.state, encode_result.bitstream);
    //std::cout << "Decoded sentence: " << decode_result.message << std::endl;

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;

    //// Проверка корректности
    if (original_message == decode_result.message)
    {
        std::cout << "Test 4: correct! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
    else
    {
        std::cout << "Test 4: failed! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
}
void test5()
{
    std::string full_byte_alphabet(256, '\0');
    for (int i = 0; i < 256; ++i) full_byte_alphabet[i] = static_cast<char>(i);


    const t_ans tans(full_byte_alphabet);

    const std::string original_message = read_text_file("example.txt");
    //std::cout << "File content: [" << original_message << "]" << std::endl;
    //std::cout << "Sentence before: " << original_message << std::endl;

    const auto start = std::chrono::high_resolution_clock::now();;
    //// Кодирование
    const auto encode_result = tans.encode(original_message, 0);
    //std::cout << "State: " << encode_result.state << std::endl;
    //std::cout << "Bitstream: " << encode_result.bitstream << std::endl;

    //// Декодирование
    const auto decode_result = tans.decode(encode_result.state, encode_result.bitstream);
    //std::cout << "Decoded sentence: " << decode_result.message << std::endl;

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;

    //// Проверка корректности
    if (original_message == decode_result.message)
    {
        std::cout << "Test 5: correct! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
    else
    {
        std::cout << "Test 5: failed! Took: " << elapsed_seconds.count() << " seconds." << std::endl;
    }
}

void test6()
{
    const std::string original_message = read_text_file("example.txt");

    std::cout << original_message << std::endl;

    const caesar caesar(original_message, 3);
    const std::string encoded_message = caesar.encode();

    std::cout << encoded_message << std::endl;
    std::cout << caesar.decode(encoded_message) << std::endl;
}
void test7()
{
    const std::string original_message = read_text_file("example.txt");

    const rot13 rot13(original_message);
    const std::string encoded_message = rot13.encode();

    std::cout << encoded_message << std::endl;
    std::cout << rot13::decode(encoded_message) << std::endl;
}
void test8()
{
    const std::string original_message = read_text_file("example.txt");

    const rot47 rot47(original_message);
    const std::string encoded_message = rot47.encode();

    std::cout << encoded_message << std::endl;
    std::cout << rot47::decode(encoded_message) << std::endl;
}

void test9()
{
    const std::string original_message = read_text_file("example.txt");
    const affine a(original_message, 3, 4);
    const std::string encoded_message = a.encode();

    std::cout << encoded_message << std::endl;
    std::cout << a.decode(encoded_message) << std::endl;
}

void test10()
{
    const std::string original_message = read_text_file("example.txt");
    const atbash a(original_message);
    const std::string encoded_message = a.encode();

    std::cout << encoded_message << std::endl;
    std::cout << a.decode(encoded_message) << std::endl;
}

void test11()
{
    const vigenere v("ATTACKATDAWN", "LEMONLEMONLE");
    const std::string encoded_message = v.encode();

    std::cout << encoded_message << std::endl;
    std::cout << v.decode(encoded_message) << std::endl;
}

int main()
{
    tANS_test();


    // std::unordered_map<char, size_t> freq = {{'a', 50}, {'b', 30}, {'c', 20}};
    // uANS coder(freq);
    //
    // coder.print_statistics();
    //
    // std::string text = "abacaba";
    // auto encoded = coder.encode(text);
    //
    // std::string decoded = coder.decode(encoded);


    return 0;
}
