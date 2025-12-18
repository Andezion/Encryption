

Caesar Cipher
=============

Overview
--------
The Caesar cipher is a simple substitution cipher that shifts each letter in the plaintext by a fixed number of positions in the alphabet. This repository contains reference implementations and tests in C, C++ and Rust.

Contents
--------
- C: Caesar.c, unit test: Caesar_Test.cpp
- C++: Caesar.h, CaesarTest.cpp
- Rust: Caesar_Test.rs, Caesar.rs

Usage
-----
Typical usage encrypts or decrypts ASCII alphabetic characters by shifting them with a chosen key (an integer). Non-alphabetic characters are usually left unchanged. To build or run tests, compile the corresponding test files in the language of your choice using your normal build tool or IDE.

Example (conceptual)
--------------------
Encrypting with shift 3: HELLO -> KHOOR

Complexity
----------
- Time: O(n) where n is the length of the input text
- Space: O(1) additional memory (in-place) or O(n) if producing a separate output

See also
--------
For implementation details open the source files in the `C`, `C++`, and `Rust` subfolders.

