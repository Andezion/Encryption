#pragma once

std::string read_text_file(const std::string& filepath);

template <typename ExceptionType, typename Func>
bool expect_exception(Func func, const std::string& test_name);

void run_correctness_test(const std::string& test_name,
                          const std::string& labeling,
                          const std::string& original_message,
                          size_t initial_state_val = 0);

void test_constructor_errors();

void test_encode_errors();

void test_decode_errors();

void test_edge_cases();

void test_performance_and_correctness();

void tANS_test();
