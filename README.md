# Encryption
Experiments with encryption, information compression and algorithms

## Content
* [ANS](#ans) - ANS-based compression and coding algorithms.
  * [C++ tANS](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/tANS.h) - Implementation of tANS in C++ (finished + tests)
  * [C++ uANS](https://github.com/Andezion/Encryption/blob/main/ANS/C%2B%2B/uANS.h) - Implementation of uANS in C++ (finished) -> tests 
  * [Rust](#rust_tans) - Implementation of tANS in Rust (not finished) -> implementation + tests
  * [Rust](#rust_uans) - Implementation of uANS in Rust! (not finished) -> implementation + tests
* [Affine](#affine) - is a special case of the more general monoalphabetic substitution cipher.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Affine/C%2B%2B/Affine.h) - Implementation of Affine in C++ (finished) -> tests
  * [Rust](#rust_affine) - Implementation of Affine in Rust (not finished) -> implementation + tests
* [Atbash](#atbash) - is a monoalphabetic substitution cipher originally used to encrypt the Hebrew alphabet.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Atbash/C%2B%2B/Atbash.h) - Implementation of Atbash in C++ (finished) -> tests 
  * [Rust](#rust_atbash) - Implementation of Atbash in Rust (not finished) -> implementation + tests
* [Caesar](#caesar) - is one of the simplest and most widely known encryption techniques.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Caesar/C%2B%2B/Caesar.h) - Implementation of Caesar in C++ (finished) -> tests 
  * [Rust](https://github.com/Andezion/Encryption/blob/main/Caesar/Rust/main.rs) - Implementation of Caesar in Rust (not finished) -> implementation + tests
* [ROT13](#rot_13) - is a simple letter substitution cipher that replaces a letter with the 13th letter after it in the Latin alphabet.
  * [C++](https://github.com/Andezion/Encryption/blob/main/ROT13/C%2B%2B/ROT13.h) - Implementation of ROT13 in C++ (finished) -> tests 
  * [Rust](#rust_rot_13) - Implementation of ROT13 in Rust (not finished) -> implementation + tests
* [ROT47](#rot_47) - ROT47 is a derivative of ROT13 which, in addition to scrambling the basic letters, treats numbers and common symbols.
  * [C++](https://github.com/Andezion/Encryption/blob/main/ROT47/C%2B%2B/ROT47.h) - Implementation of ROT47 in C++ (finished) -> tests 
  * [Rust](#rust_rot_47) - Implementation of ROT47 in Rust (not finished) -> implementation + tests
* [Vigenere](#vigenere) - Each letter is encrypted with a Caesar cipher shifted by the corresponding key letter.
  * [C++](https://github.com/Andezion/Encryption/blob/main/Vigenere/C%2B%2B/Vigenere.h)  - Implementation of Vigenere in C++ (finished) -> tests 
  * [Rust](#rust_vigenere) - Implementation of Vigenere in Rust (not finished) -> implementation + tests
 
---

## ANS
Asymmetric Numeral Systems (ANS) is an entropy coding method that combines high compression like arithmetic coding with performance close to bitwise algorithms (e.g., Huffman).
In ANS, a single number stores the entire state, and each symbol is encoded by transitioning from one state to another.
- tANS (table-based ANS) - uses a pre-computed table of transitions for fast encoding and decoding.
- uANS (uniform ANS) - uses mathematical formulae without tables, providing compactness and flexibility.

---

## Affine 

---

## Atbash

---

## Caesar

---

## ROT13

---

## ROT47

---

## Vigenere

---
