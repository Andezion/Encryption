#include <iostream>
#include <chrono>
#include "tANS.h"

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
    const t_ans tans("qwedsfdqwdasqwertyuiioopasdfghjkklzxcvbnm");

    const std::string original_message = "qwerty";
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

int main()
{
    try
    {
        test1();
        test2();
        test3();
        test4();
        test5();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
